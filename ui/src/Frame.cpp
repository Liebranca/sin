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
  #include "ui/Frame.hpp"

// ---   *   ---   *   ---
// setup for single element

void UI::Elem::nit(

  std::string ct,

  vec3 pos,
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
    m_pos.y - m_dim.y * m_scale,

  };

  m_ct       = ct;
  m_color    = color;
  m_show_ctl = show_ctl;

};

// ---   *   ---   *   ---
// calc collision plane for element

void UI::Elem::calc_plane(void) {

  vec3 tl={m_pos.x,m_pos.y,0};
  vec3 tr={m_pos.x+m_rdim.x,m_pos.y,0};
  vec3 bl={m_pos.x,m_pos.y+m_rdim.y,0};
  vec3 br={m_pos.x+m_rdim.x,m_pos.y+m_rdim.y,0};

  m_plane.set(tl,tr,bl,br);

};

// ---   *   ---   *   ---
// generate point from elem

UI::Vertex UI::Elem::base_vert(
  vec3&   pos,
  uint8_t c

) {

  Vertex vert(c);

  vert.set_pos(vec2(pos));
  vert.set_layer(uint8_t(pos.z));
  vert.set_scale(m_scale);
  vert.set_color(m_color);
  vert.set_show_ctl(m_show_ctl);

  return vert;

};

// ---   *   ---   *   ---
// get ref point for drawing
// char of element

void UI::Elem::emit(UI& dst) {

  vec3  pos    = m_pos;
  vec2& cursor = dst.get_cursor();

  float max_x  = 0.0f;

  float step_x = CENT_X * m_scale;
  float step_y =
    (CENT_Y + LINE_SPACE)
  * m_scale
  ;

  m_rdim={0,0};

  // make planes for each char
  for(uint64_t i=0;i<m_ct.length();i++) {

    // TODO: handle ctl chars proper
    bool is_newline=
        m_ct[i] == '\n'
    &&! m_show_ctl
    ;

    bool fits=false;

    // prepare render data
    if(! is_newline) {

      Vertex vert=this->base_vert(
        pos,
        m_ct[i]

      );

      dst.emit(vert);

    };

    // move to next square
    if(! is_newline && pos.x < m_line_wall.x) {

      pos.x    += step_x;
      cursor.x += step_x;

      fits = true;

    // ^move to next line
    } else if(pos.y > m_line_wall.y) {

      pos.y      -= step_y;
      cursor.y   -= step_y;
      m_rdim.y   -= step_y;

      pos.x       = m_pos.x;
      cursor.x    = 0.0f;

      fits = true;

    };

    // get maximum line length
    max_x=(cursor.x > max_x)
      ? cursor.x
      : max_x
      ;

    // early exit if no more space
    if(! fits) {
      break;

    };

  };

  // approximate real dimensions
  // of element as drawn
  m_rdim.x=max_x;
  m_rdim.y=(m_rdim.y >= 0)
    ? -step_y
    :  m_rdim.y
    ;

  this->calc_plane();

  if(cursor.x != 0.0f) {
    cursor.x += step_x;

  };

};

// ---   *   ---   *   ---
// put element in Q

uint32_t UI::push_text(

  std::string ct,

  vec3 pos,
  vec3 dim,

  uint16_t color,
  bool show_ctl

) {

  uint32_t out=m_elems.size();

  m_elems.push_back(Elem());
  auto& e=m_elems.back();

  // re-initialize
  this->reset_corners();

  // generate verts
  e.nit(ct,pos,dim,color,show_ctl);
  e.emit(*this);

  // ^do a backflip
  this->round_corners();

  return out;

};

// ---   *   ---   *   ---
// ^plain quad of size X, no chars

uint32_t UI::push_quad(

  vec3&    pos,
  vec2&    dim,

  uint16_t color

) {

  uint32_t out=m_elems.size();

  // re-initialize
  this->reset_corners();


  // ^generate geometry
  uint16_t base=m_verts.size();

  m_elems.push_back(Elem());
  auto& e=m_elems.back();

  // generate verts
  e.nit(

    " ",pos,
    {dim.x,dim.y,1.0f},

    color,false

  );

  Vertex vert=e.base_vert(pos,' ');
  this->emit(vert);

  // ^get generated verts
  auto& br=m_verts[base+0];
  auto& bl=m_verts[base+1];
  auto& tl=m_verts[base+2];
  auto& tr=m_verts[base+3];

  br.set_layer(uint8_t(pos.z+1));
  bl.set_layer(uint8_t(pos.z+1));
  tl.set_layer(uint8_t(pos.z+1));
  tr.set_layer(uint8_t(pos.z+1));

  // ^adjust to panel size
  br.set_pos({
    pos.x+dim.x+CENT_X*16,
    pos.y-dim.y-CENT_Y*4,

  });

  bl.set_pos({
    pos.x-CENT_X*4,
    pos.y-dim.y-CENT_Y*4,

  });

  tr.set_pos({
    pos.x+dim.x+CENT_X*16,
    pos.y+CENT_Y*4,

  });

  tl.set_pos({
    pos.x-CENT_X*4,
    pos.y+CENT_Y*4,

  });

  // ^do a backflip
  this->round_corners();

  // get collision plane
  e.set_rdim(dim);
  e.calc_plane();

  return out;

};

// ---   *   ---   *   ---
// reset corner cache

void UI::reset_corners(void) {

  m_corners[0]=m_verts.size();
  m_corners[1]=m_verts.size();
  m_corners[2]=m_verts.size();
  m_corners[3]=m_verts.size();

};

// ---   *   ---   *   ---
// sets corresponding flag to
// verts matching corner criteria

void UI::round_corners(void) {

  auto& br=m_verts[m_corners[0]];
  auto& bl=m_verts[m_corners[1]];
  auto& tl=m_verts[m_corners[2]];
  auto& tr=m_verts[m_corners[3]];

  br.m_data.y |= 0b000100;
  bl.m_data.y |= 0b001000;
  tl.m_data.y |= 0b010000;
  tr.m_data.y |= 0b100000;

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
// enable/disable Z-offset

void UI::Vertex::set_layer(uint8_t z) {
  m_data.w &=~ 0xFE000000;
  m_data.w |=  uint32_t(z) << 25;

};

// ---   *   ---   *   ---
// ^makes verts from elem

void UI::emit(UI::Vertex& tl) {

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

  // rounded edges calc
  this->get_corners(base);

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
// find min-max xy points from base

void UI::get_corners(uint32_t base) {

  for(uint32_t i=0;i<4;i++) {

    auto& a=m_verts[base+i];
    auto& b=m_verts[m_corners[i]];

    m_corners[i]=(this->cmp_corner(a,b,i))
      ? base+i
      : m_corners[i]
      ;

  };

};

// ---   *   ---   *   ---
// find A fits corner criteria
// better than B does

bool UI::cmp_corner(

  UI::Vertex& a,
  UI::Vertex& b,

  uint32_t    i

) {

  switch(i) {

  // bottom right
  case 0:
    return
       (a.get_pos().x >= b.get_pos().x)
    && (a.get_pos().y <= b.get_pos().y)
    ;

  // bottom left
  case 1:
    return
       (a.get_pos().x <= b.get_pos().x)
    && (a.get_pos().y <= b.get_pos().y)
    ;

  // top left
  case 2:
    return
       (a.get_pos().x <= b.get_pos().x)
    && (a.get_pos().y >= b.get_pos().y)
    ;

  // top right
  case 3:
    return
       (a.get_pos().x >= b.get_pos().x)
    && (a.get_pos().y >= b.get_pos().y)
    ;

  default:
    return false;

  };

};

// ---   *   ---   *   ---
