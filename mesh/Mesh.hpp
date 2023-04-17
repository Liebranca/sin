#ifndef __5E_MESH_H__
#define __5E_MESH_H__

// ---   *   ---   *   ---
// deps

  #include <iostream>

  #include <GL/glew.h>
  #include <glm/vec4.hpp>

  #include "bitter/kvrnel/Style.hpp"

// ---   *   ---   *   ---
// info

class Mesh {

public:

  VERSION   "v0.00.5b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  uint16_t m_vcount;
  uint16_t m_icount;
  uint16_t m_mode;
  uint16_t m_base;

  uint64_t m_offset;

// ---   *   ---   *   ---
// iface

public:

  // compiler trash
  Mesh(void) {};

  // nit
  Mesh(

    uint16_t vcount,
    uint16_t icount,

    uint16_t mode,

    uint16_t base,
    uint16_t offset

  ) {

    m_vcount = vcount;
    m_icount = icount;
    m_mode   = mode;
    m_base   = base;

    m_offset = offset * sizeof(uint16_t);

  };

  inline void draw(void) {
    glDrawElementsBaseVertex(

      m_mode,
      m_icount,

      GL_UNSIGNED_SHORT,
      (void*) m_offset,

      m_base

    );

  };

};

// ---   *   ---   *   ---

#endif // __5E_MESH_H__
