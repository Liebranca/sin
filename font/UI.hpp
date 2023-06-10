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

  VERSION   "v0.00.6b";
  AUTHOR    "IBN-3DILA";

  cxr32 CENT_X = 8.0f / 1366.0f;
  cxr32 CENT_Y = 8.0f / 768.0f;

  cxr32 SIZE_X = 1366.0f / 8.0f;
  cxr32 SIZE_Y = 768.0f / 8.0f;

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
    void set_layer(uint8_t z);

    // ^inspect
    vec2 get_pos(void);

  };

// ---   *   ---   *   ---
// a block of characters

  struct Elem {

  private:

    std::string m_ct;

    vec2     m_pos   = {-1,1};
    vec2     m_dim   = {9,16};
    vec2     m_rdim  = {0,0};

    vec2     m_line_wall;

    float    m_scale = 1.0f;
    uint16_t m_color = 0x00F7;

    bool m_show_ctl = false;

    Gaol::Plane m_plane;


  public:

    Vertex base_vert(
      vec2&   pos,
      uint8_t c

    );

    void emit(UI& dst);

    void nit(

      std::string ct,

      vec2 pos,
      vec3 dim,

      uint16_t color,
      bool show_ctl

    );

    void calc_plane(void);

    inline Gaol::Plane& get_plane(void) {
      return m_plane;

    };

    inline void set_rdim(vec2& dim) {
      m_rdim=dim;

    };

    inline vec2& get_rdim(void) {
      return m_rdim;

    };

  };

// ---   *   ---   *   ---
// attrs

private:

  VAO  m_vao;
  vec2 m_cursor={0,0};

  std::vector<Vertex>   m_verts;
  std::vector<uint16_t> m_indices;

  std::vector<Elem>     m_elems;

  uint32_t m_corners[4]={0,0,0,0};

// ---   *   ---   *   ---
// guts

  bool cmp_corner(

    UI::Vertex& a,
    UI::Vertex& b,

    uint32_t    i

  );

  void get_corners(uint32_t base);

  void reset_corners(void);
  void round_corners(void);

// ---   *   ---   *   ---
// iface

public:

  void nit_vao(uint64_t size);

  // put element in Q
  uint32_t push_text(

    std::string ct,

    vec2 pos,
    vec3 dim,

    uint16_t color,
    bool show_ctl

  );

  // ^plain quad of size X, no chars
  uint32_t push_quad(

    vec2&    pos,
    vec2&    dim,

    uint16_t color

  );

  // ^retrieve
  inline Elem& get_elem(uint32_t i) {
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

  // gets last position draw on
  inline vec2& get_cursor(void) {
    return m_cursor;

  };

  inline void reset_cursor(void) {
    m_cursor={0,0};

  };

};

// ---   *   ---   *   ---

#endif // __5E_UI_H__
