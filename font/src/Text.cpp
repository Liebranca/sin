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

void Text::fill_buff(void) {

  m_verts.clear();
  m_indices.clear();

  char* s   = m_ct.c_str();
  vec2  pos = m_pos;

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
  Text::Vertex>& tl

) {

  // base eq top left
  tl.m_data.y=0b00;

  // top right
  Vertex tr=*tl;
  tr.m_data.y=0b10;

  // bottom right, bottom left
  Vertex br=*tl;
  Vertex bl=*tl;
  br.m_data.y=0b11;
  bl.m_data.y=0b10;

  // ^commit
  verts.push_back(br);
  verts.push_back(bl);
  verts.push_back(tl);
  verts.push_back(tr);

  // make quad
  uint16_t base=indices.size();

  // tri A
  indices.push_back(base+0);
  indices.push_back(base+1);
  indices.push_back(base+2);

  // tri B
  indices.push_back(base+0);
  indices.push_back(base+2);
  indices.push_back(base+3);

};

// ---   *   ---   *   ---
