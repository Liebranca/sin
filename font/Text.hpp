#ifndef __5E_TEXT_H__
#define __5E_TEXT_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/Style.hpp"

// ---   *   ---   *   ---
// info

class Text {

public:

  VERSION   "v0.00.1b";
  AUTHOR    "IBN-3DILA";

  cxr32 CENT_X = 8.0f / 1366.0f;
  cxr32 CENT_Y = 9.0f / 768.0f;

// ---   *   ---   *   ---
// helpers

  struct Vertex {

    uvec4 m_data = {0,0,0,0};

    // ctrash
    Vertex(void);
    Vertex(uint8_t idex);

    // ^cstruc
    void nit(uint8_t idex=0x00);

    // copy
    Vertex& operator=(Vertex& other) {
      m_data=other.m_data;
      return *this;

    };

    // ^touch config
    void set_pos(vec2 pos);
    void set_scale(float z);
    void set_char(uint8_t idex);
    void set_color(uint16_t color);
    void set_show_ctl(bool show);

  };

// ---   *   ---   *   ---
// attrs

private:

  std::vector<Vertex>   m_verts;
  std::vector<uint16_t> m_indices;

  std::string m_ct;

  vec2     m_pos   = {-1,-1};

  float    m_scale = 1.0f;
  uint16_t m_color = 0x00F7;

// ---   *   ---   *   ---
// drawable space guides

  vec2  m_line_beg  = {-1,-1};
  float m_line_wall = CENT_X * 48;

// ---   *   ---   *   ---
// flags

  bool m_updated  = false;
  bool m_show_ctl = false;

// ---   *   ---   *   ---
// guts

  void fill_buff(void);
  void emit(Vertex& tl);

// ---   *   ---   *   ---
// iface

public:

  // set string to display
  inline void set_content(std::string s) {
    m_ct      = s;
    m_updated = true;

  };

  // get copy of string to display
  inline std::string get_content(void) {
    return m_ct;

  };

  // get string formatted for GBuff
  inline Vertex* get_buff(void) {

    if(m_updated) {
      this->fill_buff();
      m_updated=false;

    };

    return m_buff.data();

  };

  // ^touch config
  inline void set_pos(vec2 pos) {
    m_pos=pos;

  };

  inline void set_line_beg(vec2 pos) {
    m_line_beg=pos;

  };

  inline void set_line_wall(float x) {
    m_line_wall=x;

  };

  inline void set_scale(float z) {
    m_scale=x;

  };

  inline void set_color(uint16_t color) {
    m_color=color;

  };

  inline void set_show_ctl(bool show) {
    m_show_ctl=show;

  };

};

// ---   *   ---   *   ---

#endif // __5E_TEXT_H__
