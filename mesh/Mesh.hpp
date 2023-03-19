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

  VERSION   "v0.00.4b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  uint16_t m_id;
  uint16_t m_vcount;
  uint16_t m_icount;

  uint64_t m_offset;

// ---   *   ---   *   ---
// iface

public:

  // compiler trash
  Mesh(void) {};

  // nit
  Mesh(
    uint16_t id,
    uint16_t vcount,
    uint16_t icount,

    uint64_t offset

  ) {

    m_id     = id;

    m_vcount = vcount;
    m_icount = icount;

    m_offset = offset;

  };

  inline void draw(void) {

    glDrawElements(

      GL_TRIANGLES,
      m_icount,

      GL_UNSIGNED_SHORT,
      (void*) m_offset

    );

  };

};

// ---   *   ---   *   ---

#endif // __5E_MESH_H__
