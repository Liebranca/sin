#ifndef __5E_MESH_FRAME_H__
#define __5E_MESH_FRAME_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/Style.hpp"
  #include "mesh/Mesh.hpp"

// ---   *   ---   *   ---
// info

class Meshes {

public:

  VERSION   "v0.00.2b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// consts

private:

  cx8 BATCH_SZ = 2;
  cx8 BUFF_SZ  = 4;

  enum {

    VBO,
    IBO,

    NUM_BUFFS

  };

// ---   *   ---   *   ---
// attrs

  uint32_t m_vao;
  uint32_t m_buff[NUM_BUFFS];

  Mesh     m_mesh[BATCH_SZ];

  uint16_t m_vcount = 0;
  uint16_t m_icount = 0;

  uint32_t m_top    = 0;

// ---   *   ---   *   ---
// interface

public:

  // nit stackslot
  Meshes(void);
  ~Meshes(void);

  // make mesh
  uint32_t nit(

    void*    verts,
    void*    indices,

    uint16_t vcount,
    uint16_t icount

  );

  // bind buffers
  inline void use(void);

  // draw specific mesh
  inline void draw(uint32_t idex) {
    m_mesh[idex].draw();

  };

};

// ---   *   ---   *   ---

#endif // __5E_MESH_FRAME_H__
