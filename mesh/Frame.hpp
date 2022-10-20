#ifndef __MESH_FRAME_H__
#define __MESH_FRAME_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/Style.hpp"
  #include "mesh/Vertex.hpp"

// ---   *   ---   *   ---
// info

class Meshes {

public:

  VERSION   "v0.00.1b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// consts

private:

  cx8 BATCH_SZ=1;

// ---   *   ---   *   ---
// attrs

  uint32_t m_vao;
  uint32_t m_glbuff;

  Mesh     m_buff[BATCH_SZ];

  uint16_t m_vcount;

// ---   *   ---   *   ---
// interface

public:

  // compiler trash
  Meshes(void) {};

  // make and destroy
  void nit(void);
  void del(void);

  // push to gl buff
  void upload(void* data,uint16_t vcount);

  // bind/send to shader
  void use(void);
  void draw(uint32_t idex);

};

// ---   *   ---   *   ---

#endif // __MESH_FRAME_H__
