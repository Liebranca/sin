#ifndef __5E_MESH_FRAME_H__
#define __5E_MESH_FRAME_H__

// ---   *   ---   *   ---
// deps

  #include <memory>

  #include "bitter/kvrnel/Style.hpp"

  #include "mesh/Mesh.hpp"
  #include "mesh/Sprite.hpp"

// ---   *   ---   *   ---
// info

class Meshes {

public:

  VERSION   "v0.00.3b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// consts

private:

  // so we're clear on what the
  // size is actually measuring
  cx8  TRIANGLES = 3;
  cx16 BUFF_SZ   = 128 * TRIANGLES;

  // ^max instances independent of size
  cx8  BATCH_SZ  = 2;

  enum {

    VBO,
    IBO,

    TILE_SSBO,

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

  std::unique_ptr<uint32_t> m_tiles;

// ---   *   ---   *   ---
// guts

  void push_quad(

    Mesh::Build& bld,

    uint64_t     desc,
    uint16_t     img_sz,

    float        img_step

  );

  Mesh::Prim make_sprite_frame(
    std::vector<uint64_t>& tab,
    uint64_t& offset

  );

  // ^entry point
  uint32_t make_prim(

    std::vector<uint64_t>& tab,

    uint64_t& offset,
    uint16_t  type

  );

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

  std::vector<uint32_t> make_sprite(
    Texture& atlas,
    uint16_t idex

  );

  // bind buffers
  inline void use(void) {
    glBindVertexArray(m_vao);

  };

  // draw specific mesh
  inline void draw(uint32_t idex) {
    m_mesh[idex].draw();

  };

  // access tile buffer
  inline uint32_t& get_tile(
    uint64_t idex

  ) {return m_tiles.get()[idex];};

  // ^send it to GL
  void update_tiles(void);

};

// ---   *   ---   *   ---

#endif // __5E_MESH_FRAME_H__
