#ifndef __5E_MESH_FRAME_H__
#define __5E_MESH_FRAME_H__

// ---   *   ---   *   ---
// deps

  #include <memory>
  #include <glm/glm.hpp>

  #include "bitter/kvrnel/GLM.hpp"
  #include "bitter/ff/CRK.hpp"

  #include "mesh/VAO.hpp"
  #include "mesh/Mesh.hpp"
  #include "mesh/Sprite.hpp"

// ---   *   ---   *   ---
// info

class Meshes {

public:

  VERSION   "v0.01.7b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// helpers

  // maximum matrices loaded onto
  // draw queue ssbo
  cx8 QUEUE_SZ=128;

  // ^matrices per queue
  struct Draw_Queue_Mats {
    mat4 model[QUEUE_SZ];
    mat4 nmat[QUEUE_SZ];

  };

  // ^mats AND mesh ptrs
  struct Draw_Queue {

    Draw_Queue_Mats mats;

    uint32_t meshid[QUEUE_SZ];
    uint32_t cnt;

  };

// ---   *   ---   *   ---
// consts

private:

  // so we're clear on what the
  // size is actually measuring
  cx8  TRIANGLES = 3;
  cx16 BUFF_SZ   = 0x1000 * TRIANGLES;

  // ^max instances independent of size
  cx16 BATCH_SZ  = Texture::MAX_DEPTH;

  typedef std::vector<ANS> Anim_Meta;

// ---   *   ---   *   ---
// attrs

  VAO      m_vao;
  Mesh     m_mesh[BATCH_SZ];

  uint16_t m_vcount = 0;
  uint16_t m_icount = 0;

  uint32_t m_top    = 0;
  uint32_t m_pidex  = 0;

  // anti-cpp trap ;>
  bool     m_nitted = false;

  Texture       m_texture;
  Anim_Meta     m_anim_meta;
  Sprite::Anims m_anims;

// ---   *   ---   *   ---
// guts

  inline void upload(

    uint64_t dst,
    void*    data,

    uint64_t offset,
    uint64_t cnt

  ) {

    auto& gbuff=m_vao.gbuff(dst);
    gbuff.sub_data(data,offset,cnt);

  };

  // ^add to vertex buffer
  inline void push_verts(
    uint64_t cnt,
    void*    data

  ) {

    this->upload(VAO::VBO,data,m_vcount,cnt);
    m_vcount+=cnt;

  };

  // ^add to index buffer
  inline void push_indices(
    uint64_t cnt,
    void*    data

  ) {

    this->upload(VAO::IBO,data,m_icount,cnt);
    m_icount+=cnt;

  };

  // upload CRK verts to buffers
  uint32_t new_mesh(
    CRK::Prim& p,
    uint32_t   mode=GL_TRIANGLES

  );

// ---   *   ---   *   ---
// changing already set sections
// of gl buffers

  inline void repl_verts(

    uint64_t offset,
    uint64_t cnt,

    void*    data

  ) {

    this->upload(VAO::VBO,data,offset,cnt);

  };

  inline void repl_indices(

    uint64_t offset,
    uint64_t cnt,

    void*    data

  ) {

    this->upload(VAO::IBO,data,offset,cnt);

  };

  bool repl_mesh(

    uint32_t   idex,

    CRK::Prim& p,
    uint32_t   mode=GL_TRIANGLES

  );

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  void nit(
    uint32_t texsz=256,
    uint32_t pidex=0

  );

  // ctrash
  Meshes(void) {};

  // loads resource from file
  uint32_t load_asset(

    std::string fpath_base,

    uint8_t     tex_beg,
    uint8_t     tex_type

  );

  // ^fetch
  Sprite ice_asset(uint32_t src);

  // create editable mesh
  uint32_t new_edit(void);

  // overwrite vertex data in slot
  void repl(
    uint32_t  beg,
    CRK::Prim p

  );

  // bind buffers
  inline void use(void) {
    m_vao.bind();
    m_texture.use();

  };

  inline void draw(uint32_t idex) {
    m_mesh[idex].draw();

  };

  // getters
  inline uint32_t get_pidex(void) {
    return m_pidex;

  };

  inline uint32_t matof(uint32_t meshid) {
    return m_texture.matid(meshid);

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
