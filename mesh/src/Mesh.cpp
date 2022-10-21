// ---   *   ---   *   ---
// MESH
// Points and lines come together
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include "mesh/Mesh.hpp"

// ---   *   ---   *   ---

inline void Mesh::draw(void) {

  glDrawElements(

    GL_TRIANGLES,
    m_icount,

    GL_UNSIGNED_SHORT,
    m_offset

  );

};

// ---   *   ---   *   ---
