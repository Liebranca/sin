// ---   *   ---   *   ---
// TEXT
// For you to RD/WT
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
  #include "font/Text.hpp"

// ---   *   ---   *   ---
// makes GBuff data from string

void Text::fill_buff(void) {

  m_verts.clear();
  m_indices.clear();

  vec2  pos = m_pos;

  // make planes for each char
  for(uint64_t i=0;i<m_ct.length();i++) {

    // prepare render data
    Vertex vert(m_ct[i]);
    vert.set_pos(pos);
    vert.set_scale(m_scale);
    vert.set_color(m_color);
    vert.set_show_ctl(m_show_ctl);

    // write to buff
    this->emit(vert);

    // move to next square
    if(m_ct[i] != '\n' && pos.x < m_line_wall.x) {
      pos.x += CENT_X;

    } else if(pos.y < m_line_wall.y) {
      pos.y -= CENT_Y;
      pos.x  = m_pos.x;

    } else {
      break;

    };

  };

};

// ---   *   ---   *   ---
// calls sub_data for vao buffs

void Text::upload(void) {

  // trigger update
  auto& verts   = this->get_verts();
  auto& indices = this->get_indices();

  // get handles to buffs
  auto& vbo = m_vao.gbuff(VAO::VBO);
  auto& ibo = m_vao.gbuff(VAO::IBO);

  if(! indices.size()) {
    return;

  };

  // push
  vbo.sub_data(
    (void*) verts.data(),
    0,verts.size()

  );

  ibo.sub_data(
    (void*) indices.data(),
    0,indices.size()

  );

};

// ---   *   ---   *   ---
// makes gl draw call for vao

void Text::draw(void) {

  m_vao.bind();

  glDrawElements(

    GL_TRIANGLES,
    m_indices.size(),

    GL_UNSIGNED_SHORT,
    (void*) 0

  );

  m_vao.unbind();

};

// ---   *   ---   *   ---
// gl boiler

void Text::nit_vao(vec2 dim) {

  // nit buffs
  m_vao.nit(

    GBuff::D_ARRAY,
    GBuff::D_ELEMENT,

    sizeof(Text::Vertex),
    uint64_t(dim.x * dim.y),

    sizeof(uint16_t),
    uint64_t(dim.x * dim.y)

  );

  // ^vertex attrs
  m_vao.vattr(

    VAO::U32_4,

    sizeof(Text::Vertex),
    offsetof(Text::Vertex,m_data)

  );

};

// ---   *   ---   *   ---
// ctrash

Text::Vertex::Vertex(void) {
  this->nit();

};

Text::Vertex::Vertex(uint8_t idex) {
  this->nit(idex);

};

// ---   *   ---   *   ---
// ^cstruc

void Text::Vertex::nit(uint8_t idex) {
  this->set_char(idex);

};

// ---   *   ---   *   ---
// set char position

void Text::Vertex::set_pos(vec2 pos) {

  pos += vec2({NEGATIVE_X,NEGATIVE_Y});

  uint16_t x = uint16_t(pos.x/CENT_X);
  uint16_t y = uint16_t(pos.y/CENT_Y);

  m_data.x = uint32_t(x) | (uint32_t(y) << 16);

};

// ---   *   ---   *   ---
// ^retrieve

vec2 Text::Vertex::get_pos(void) {

  float x=(m_data.x  & 0xFFFF) * CENT_X;
  float y=(m_data.x >>     16) * CENT_Y;

  x -= NEGATIVE_X;
  y -= NEGATIVE_Y;

  return {x,y};

};

// ---   *   ---   *   ---
// set char scale

void Text::Vertex::set_scale(float z) {
  m_data.z=uint32_t(z*0xFF);

};

// ---   *   ---   *   ---
// select character idex

void Text::Vertex::set_char(uint8_t idex) {
  m_data.w &=~ 0x000000FF;
  m_data.w |=  idex;

};

// ---   *   ---   *   ---
// set fg/bg color

void Text::Vertex::set_color(uint16_t color) {
  m_data.w &=~ 0x00FFFF00;
  m_data.w |=  uint32_t(color) << 8;

};

// ---   *   ---   *   ---
// enable/disable rendering of
// control characters

void Text::Vertex::set_show_ctl(bool show) {
  m_data.w &=~ 0x01000000;
  m_data.w |=  uint32_t(show) << 24;

};

// ---   *   ---   *   ---
// cats four vertices to Q

void Text::emit(
  Text::Vertex& tl

) {

  // idex beg
  uint16_t base=m_verts.size();

  // base eq top left
  tl.m_data.y=0b00;
  vec2 pos=tl.get_pos();

  // top right
  Vertex tr=tl;
  tr.m_data.y=0b01;
  tr.set_pos(pos+vec2({CENT_X,0}));

  // bottom right, bottom left
  Vertex br=tl;
  Vertex bl=tl;
  br.m_data.y=0b11;
  bl.m_data.y=0b10;
  br.set_pos(pos+vec2({CENT_X,-CENT_Y}));
  bl.set_pos(pos+vec2({0,-CENT_Y}));

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
