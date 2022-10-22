#ifndef __5E_MESH_H__
#define __5E_MESH_H__

// ---   *   ---   *   ---
// deps

  #include <glm/vec4.hpp>
  #include "bitter/kvrnel/Style.hpp"

// ---   *   ---   *   ---
// info

class Mesh {

public:

  VERSION   "v0.00.2b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// helper

  typedef struct {

    union {

      struct {

        uint8_t  XYZ[3];
        uint8_t  NTB[3];
        uint8_t  TEX[2];  // 8

        uint16_t ID;      // 10
        uint16_t BONE;    // 12

      };

      uint8_t data[16];

    };

  } Vertex;

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

  inline void draw(void);

};

// ---   *   ---   *   ---

#endif // __5E_MESH_H__
