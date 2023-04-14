#ifndef __5E_MESH_FRAME_H__
#define __5E_MESH_FRAME_H__

// ---   *   ---   *   ---
// deps

  #include <memory>

  #include "bitter/kvrnel/Style.hpp"
  #include "bitter/ff/CRK.hpp"

  #include "mesh/Mesh.hpp"
  #include "mesh/Sprite.hpp"

// ---   *   ---   *   ---
// info

class Meshes {

public:

  VERSION   "v0.00.8b";
  AUTHOR    "IBN-3DILA";

  typedef std::vector<uint32_t> iArray;

// ---   *   ---   *   ---
// consts

private:

  // so we're clear on what the
  // size is actually measuring
  cx8  TRIANGLES = 3;
  cx16 BUFF_SZ   = 0x1000 * TRIANGLES;

  // ^max instances independent of size
  cx16 BATCH_SZ  = 256;

  enum {

    VBO,
    IBO,

    NUM_BUFFS

  };

  typedef std::vector<Texture> Textures;
  typedef std::vector<ANS>     Anim_Meta;

// ---   *   ---   *   ---
// attrs

  uint32_t m_vao;
  uint32_t m_buff[NUM_BUFFS];

  Mesh     m_mesh[BATCH_SZ];

  uint16_t m_vcount = 0;
  uint16_t m_icount = 0;

  uint32_t m_top    = 0;
  uint32_t m_pidex  = 0;

  // anti-cpp trap ;>
  bool     m_nitted = false;

//  std::unique_ptr<uint32_t> m_tiles;

  Textures      m_textures;
  Anim_Meta     m_anim_meta;
  Sprite::Anims m_anims;

// ---   *   ---   *   ---
// interface

public:

  // ctrash
  Meshes(void) {};
  ~Meshes(void);

  // cstruc
  void nit(uint32_t pidex=0);

  // make mesh
  uint32_t new_mesh(CRK::Prim& p);

  // load sprite sheet from file
  uint32_t new_sprite(
    std::string& path

  );

  // ^copy
  Sprite ice_sprite(uint32_t src);

  // bind buffers
  inline void use(void) {
    glBindVertexArray(m_vao);

  };

  inline void draw(uint32_t idex) {
    m_mesh[idex].draw();

  };

//  // access tile buffer
//  inline uint32_t& get_tile(
//    uint64_t idex
//
//  ) {return m_tiles.get()[idex];};
//
//  // ^send it to GL
//  void update_tiles(void);

  // getters
  inline uint32_t get_pidex(void) {
    return m_pidex;

  };

  inline Textures& get_textures(void) {
    return m_textures;

  };

  inline Anim_Meta& get_anim_meta(void) {
    return m_anim_meta;

  };

  inline Sprite::Anims& get_anims(void) {
    return m_anims;

  };


};

// ---   *   ---   *   ---

#endif // __5E_MESH_FRAME_H__
