#ifndef __5E_DEBUG_DRAW_H__
#define __5E_DEBUG_DRAW_H__

// ---   *   ---   *   ---
// deps

  #include <GL/glew.h>
  #include "bitter/kvrnel/GLM.hpp"
  #include "mesh/VAO.hpp"

// ---   *   ---   *   ---
// info

class Debug_Draw {

public:

  VERSION   "v0.00.1b";
  AUTHOR    "IBN-3DILA";

  cxr32 CENT     = 1.0f/0x400;
  cxr32 NEGATIVE = CENT*0x8000;

  cx16  DRAW_3D  = 0x0000;
  cx16  DRAW_2D  = 0x0001;

  cx8   DEFCOL   = 0xF1;
  cxr32 DEFSCALE = 1.0f;

// ---   *   ---   *   ---
// helpers

  struct Vertex {

    uvec4 m_data;

    void set_pos(vec3 pos);
    vec3 get_pos(void);

    void set_scale(float scale);
    void set_color(uint8_t color);

    // flip individual flags
    void set_2d(bool enable);

    // ^all at once
    void set_flags(uint16_t flags);

    // cstruc/modify
    void set(

      vec3     pos,
      float    scale  = DEFSCALE,

      uint8_t  color  = DEFCOL,
      uint16_t flags  = DRAW_2D

    );

  };

// ---   *   ---   *   ---
// attrs

private:

  VAO m_vao;

  svec<Vertex>   m_verts;
  svec<uint16_t> m_indices;

  uint32_t m_mode;
  uint32_t m_top;

  uint32_t m_top_i;
  float    m_layer;

// ---   *   ---   *   ---
// guts

// ---   *   ---   *   ---
// iface

public:

  void nit(
    uint64_t size = 0x400,
    uint32_t mode = GL_LINES

  );

  void push_point(

    vec3&    a,

    float    scale=DEFSCALE,
    uint8_t  color=DEFCOL,
    uint16_t flags=DRAW_2D

  );

  void push_line(

    vec3&   a,
    vec3&   b,

    float    scale=DEFSCALE,

    uint8_t  color=DEFCOL,
    uint16_t flags=DRAW_2D

  );

  void upload(void);
  void draw(void);

  inline void clear(void) {
    m_top   = 0;
    m_top_i = 0;

  };

  inline bool ready(void) {
    return m_top != 0;

  };

  inline void set_layer(float z) {
    m_layer=-z;

  };

  inline float get_layer(void) {
    return m_layer;

  };

};

// ---   *   ---   *   ---
// tightness check

CASSERT(

   sizeof(Debug_Draw::Vertex)
== sizeof(uvec4),

  "\n\e[31;22mBad DD::Vertex size\e[0m\n\n"

);

// ---   *   ---   *   ---

#endif // __5E_DEBUG_DRAW_H__
