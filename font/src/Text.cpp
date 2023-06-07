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

  #include "font/Text.hpp"

// ---   *   ---   *   ---
// makes GBuff data from string

void Text::fill_buff(vec2 pos) {

  m_verts.clear();
  m_indices.clear();

  char* s = (char*) m_ct.c_str();

  // make planes for each char
  while(*s++) {

    // prepare render data
    Vertex vert(*s);
    vert.set_pos(pos);
    vert.set_scale(m_scale);
    vert.set_color(m_color);
    vert.set_show_ctl(m_show_ctl);

    // write to buff
    this->emit(vert);

    // move to next square
    if(*s != '\n' && pos.x < m_line_wall) {
      pos.x += CENT_X;

    } else {
      pos.y += CENT_Y;
      pos.x  = m_line_beg;

    };

  };

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
// converts float cord to div16
// encoding used by raster font

static uint16_t f_to_d16(float co) {
  return uint16_t((co * 0x8000) + 0x7FFF);

};

// ---   *   ---   *   ---
// set char position

void Text::Vertex::set_pos(vec2 pos) {

  uint16_t x = f_to_d16(pos.x);
  uint16_t y = f_to_d16(pos.y);

  m_data.x = uint32_t(x) | (uint32_t(y) << 16);

};

// ---   *   ---   *   ---
// set char scale

void Text::Vertex::set_scale(float z) {
  m_data.z=uint32_t(z*0xFF);

};

// ---   *   ---   *   ---
// select character idex

void Text::Vertex::set_char(uint8_t idex) {
  m_data.w &=~ 0xFF;
  m_data.w |=  idex;

};

// ---   *   ---   *   ---
// set fg/bg color

void Text::Vertex::set_color(uint16_t color) {
  m_data.w &=~ 0xFFFF00;
  m_data.w |= uint32_t(color) << 8;

};

// ---   *   ---   *   ---
// enable/disable rendering of
// control characters

void Text::Vertex::set_show_ctl(bool show) {
  m_data.w &=~ 0x01000000;
  m_data.w |= uint32_t(show) << 24;

};

// ---   *   ---   *   ---
// cats four vertices to Q

void Text::emit(
  Text::Vertex& tl

) {

  // base eq top left
  tl.m_data.y=0b00;

  // top right
  Vertex tr=tl;
  tr.m_data.y=0b10;

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

  // make quad
  uint16_t base=m_indices.size();

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
