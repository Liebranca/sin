// ---   *   ---   *   ---
// UI
// Shows you things
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include <GL/glew.h>
  #include "font/UI.hpp"

// ---   *   ---   *   ---
// setup for single element

void UI::Element::nit(

  std::string ct,

  vec2 pos,
  vec3 dim,

  uint16_t color,
  bool show_ctl

) {

  m_pos   = pos;
  m_scale = dim.z;

  m_dim={
    dim.x * CENT_X,
    dim.y * CENT_Y

  };

  m_line_wall={
    m_pos.x + m_dim.x * m_scale,
    m_pos.y + m_dim.y * m_scale,

  };

  m_ct       = ct;
  m_color    = color;
  m_show_ctl = show_ctl;

  // calc collision plane
  vec3 tl={m_pos.x,m_pos.y,0};
  vec3 tr={m_line_wall.x,m_pos.y,0};
  vec3 bl={m_pos.x,m_line_wall.y,0};
  vec3 br={m_line_wall.x,m_line_wall.y,0};

  // ^commit
  m_plane.set(tl,tr,bl,br);

};

// ---   *   ---   *   ---
// get ref point for drawing
// char of element

void UI::Element::emit(UI& dst) {

  vec2 pos=m_pos;

  // make planes for each char
  for(uint64_t i=0;i<m_ct.length();i++) {

    // prepare render data
    Vertex vert(m_ct[i]);
    vert.set_pos(pos);
    vert.set_scale(m_scale);
    vert.set_color(m_color);
    vert.set_show_ctl(m_show_ctl);

    // write to buff
    dst.emit(vert);

    // move to next square
    if(m_ct[i] != '\n' && pos.x < m_line_wall.x) {
      pos.x += CENT_X * m_scale;

    } else if(pos.y < m_line_wall.y) {
      pos.y -= (CENT_Y + LINE_SPACE) * m_scale;
      pos.x  = m_pos.x;

    } else {
      break;

    };

  };

};

// ---   *   ---   *   ---
// put element in Q

uint32_t UI::push(

  std::string ct,

  vec2 pos,
  vec3 dim,

  uint16_t color,
  bool show_ctl

) {

  uint32_t out=m_elems.size();

  m_elems.push_back(Element());
  auto& elem=m_elems.back();

  elem.nit(ct,pos,dim,color,show_ctl);
  elem.emit(*this);

  return out;

};

// ---   *   ---   *   ---
// empties buffers

void UI::clear(void) {
  m_verts.clear();
  m_indices.clear();
  m_elems.clear();

};

// ---   *   ---   *   ---
// calls sub_data for vao buffs

void UI::upload(void) {

  // get handles to buffs
  auto& vbo = m_vao.gbuff(VAO::VBO);
  auto& ibo = m_vao.gbuff(VAO::IBO);

  if(! m_indices.size()) {
    return;

  };

  // push
  vbo.sub_data(
    (void*) m_verts.data(),
    0,m_verts.size()

  );

  ibo.sub_data(
    (void*) m_indices.data(),
    0,m_indices.size()

  );

};

// ---   *   ---   *   ---
// makes gl draw call for vao

void UI::draw(void) {

  m_vao.bind();

  glDrawElements(

    GL_TRIANGLES,
    m_indices.size(),

    GL_UNSIGNED_SHORT,
    (void*) 0

  );

  m_vao.unbind();
  this->clear();

};

// ---   *   ---   *   ---
// gl boiler

void UI::nit_vao(uint64_t size) {

  // nit buffs
  m_vao.nit(

    GBuff::D_ARRAY,
    GBuff::D_ELEMENT,

    sizeof(UI::Vertex),size,
    sizeof(uint16_t),size

  );

  // ^vertex attrs
  m_vao.vattr(

    VAO::U32_4,

    sizeof(UI::Vertex),
    offsetof(UI::Vertex,m_data)

  );

};

// ---   *   ---   *   ---
// ctrash

UI::Vertex::Vertex(void) {
  this->nit();

};

UI::Vertex::Vertex(uint8_t idex) {
  this->nit(idex);

};

// ---   *   ---   *   ---
// ^cstruc

void UI::Vertex::nit(uint8_t idex) {
  this->set_char(idex);

};

// ---   *   ---   *   ---
// set char position

void UI::Vertex::set_pos(vec2 pos) {

  pos += vec2({NEGATIVE_X,NEGATIVE_Y});

  uint16_t x = uint16_t(pos.x/CENT_X);
  uint16_t y = uint16_t(pos.y/CENT_Y);

  m_data.x = uint32_t(x) | (uint32_t(y) << 16);

};

// ---   *   ---   *   ---
// ^retrieve

vec2 UI::Vertex::get_pos(void) {

  float x=(m_data.x  & 0xFFFF) * CENT_X;
  float y=(m_data.x >>     16) * CENT_Y;

  x -= NEGATIVE_X;
  y -= NEGATIVE_Y;

  return {x,y};

};

// ---   *   ---   *   ---
// set char scale

void UI::Vertex::set_scale(float z) {
  m_data.z=uint32_t(z*0xFF);

};

// ---   *   ---   *   ---
// select character idex

void UI::Vertex::set_char(uint8_t idex) {
  m_data.w &=~ 0x000000FF;
  m_data.w |=  idex;

};

// ---   *   ---   *   ---
// set fg/bg color

void UI::Vertex::set_color(uint16_t color) {
  m_data.w &=~ 0x00FFFF00;
  m_data.w |=  uint32_t(color) << 8;

};

// ---   *   ---   *   ---
// enable/disable rendering of
// control characters

void UI::Vertex::set_show_ctl(bool show) {
  m_data.w &=~ 0x01000000;
  m_data.w |=  uint32_t(show) << 24;

};

// ---   *   ---   *   ---
// ^makes verts from elem

void UI::emit(
  UI::Vertex& tl

) {

  // idex beg
  uint16_t base=m_verts.size();

  // top left
  tl.m_data.y=0b00;

  // top right
  Vertex tr=tl;
  tr.m_data.y=0b01;

  // bottom right, bottom left
  Vertex br=tl;
  Vertex bl=tl;
  br.m_data.y=0b11;
  bl.m_data.y=0b10;

  // ^commit
  m_verts.push_back(br);
  m_verts.push_back(bl);
  m_verts.push_back(tl);
  m_verts.push_back(tr);

  // tri A
  m_indices.push_back(base+0);
  m_indices.push_back(base+1);
  m_indices.push_back(base+2);

  // tri B
  m_indices.push_back(base+0);
  m_indices.push_back(base+2);
  m_indices.push_back(base+3);

};

// ---   *   ---   *   ---
