#ifndef __5E_MESH_H__
#define __5E_MESH_H__

// ---   *   ---   *   ---
// deps

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

  void*    m_offset;

// ---   *   ---   *   ---
// iface

public:

  // compiler trash
  Mesh(void) {};

  // nit
  Mesh(
    uint16_t id,

    uint16_t offset,
    uint16_t vcount,
    uint16_t icount

  ) {

    m_id     = id;

    m_vcount = vcount;
    m_icount = icount;

    m_offset = (void*) ((uintptr_t) offset);

  };

  inline void draw(void) {

    glDrawElements(

      GL_TRIANGLES,
      m_icount,

      GL_UNSIGNED_SHORT,
      m_offset

    );

  };

};

// ---   *   ---   *   ---

#endif // __5E_MESH_H__
