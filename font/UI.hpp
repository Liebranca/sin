#ifndef __5E_UI_H__
#define __5E_UI_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/GLM.hpp"

  #include "gaoler/Plane.hpp"
  #include "mesh/VAO.hpp"

// ---   *   ---   *   ---
// info

class UI {

public:

  VERSION   "v0.00.3b";
  AUTHOR    "IBN-3DILA";

  cxr32 CENT_X = 8.0f / 1366.0f;
  cxr32 CENT_Y = 8.0f / 768.0f;

  cxr32 LINE_SPACE =  1.0f / 768.0f;

  cxr32 NEGATIVE_X = CENT_X * 256;
  cxr32 NEGATIVE_Y = CENT_Y * 256;

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

    // ^inspect
    vec2 get_pos(void);

  };

// ---   *   ---   *   ---
// a block of characters

  struct Element {

  private:

    std::string m_ct;

    vec2     m_pos   = {-1,1};
    vec2     m_dim   = {9,16};

    vec2     m_line_wall;

    float    m_scale = 1.0f;
    uint16_t m_color = 0x00F7;

    bool m_show_ctl = false;

    Gaol::Plane m_plane;


  public:

    void emit(UI& dst);

    void nit(

      std::string ct,

      vec2 pos,
      vec3 dim,

      uint16_t color,
      bool show_ctl

    );

    inline Gaol::Plane& get_plane(void) {
      return m_plane;

    };

  };

// ---   *   ---   *   ---
// attrs

private:

  VAO m_vao;

  std::vector<Vertex>   m_verts;
  std::vector<uint16_t> m_indices;

  std::vector<Element>  m_elems;

// ---   *   ---   *   ---
// guts

// ---   *   ---   *   ---
// iface

public:

  void nit_vao(uint64_t size);

  // put element in Q
  uint32_t push(

    std::string ct,

    vec2 pos,
    vec3 dim,

    uint16_t color,
    bool show_ctl

  );

  // ^retrieve
  inline Element& get_elem(uint32_t i) {
    return m_elems[i];

  };

  // ^makes verts from elem
  void emit(Vertex& tl);

  // calls sub_data for vao buffs
  void upload(void);

  // makes gl draw call for vao
  void draw(void);

  // empties buffers
  void clear(void);

  // get any elements Q'd
  bool ready(void) {
    return m_elems.size() > 0;

  };

};

// ---   *   ---   *   ---

#endif // __5E_UI_H__
