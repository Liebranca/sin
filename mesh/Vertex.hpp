#ifndef __VERTEX_H__
#define __VERTEX_H__

// ---   *   ---   *   ---
// deps

  #include <glm/vec4.hpp>
  #include "bitter/kvrnel/Style.hpp"

// ---   *   ---   *   ---
// info

class Mesh {

public:

  VERSION   "v0.00.1b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// helpers

  typedef struct {
    glm::vec4 Position;

  } Vertex;

// ---   *   ---   *   ---
// attrs

private:

  short m_id;
  short m_vcount;

// ---   *   ---   *   ---
// interface

public:

  // compiler trash
  Mesh() {};

  // nit
  Mesh(short id,short vcount) {
    m_id     = id;
    m_vcount = vcount;

  };

  uint16_t beg(void) {return 0;};
  uint16_t end(void) {return 3;};

};

// ---   *   ---   *   ---

#endif // __VERTEX_H__
