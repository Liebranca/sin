#ifndef __5E_MESH_FRAME_H__
#define __5E_MESH_FRAME_H__

// ---   *   ---   *   ---
// deps

  #include <memory>
  #include <glm/glm.hpp>

  #include "bitter/kvrnel/Style.hpp"
  #include "bitter/ff/CRK.hpp"

  #include "mesh/Mesh.hpp"
  #include "mesh/Sprite.hpp"

// ---   *   ---   *   ---
// info

class Meshes {

public:

  VERSION   "v0.01.4b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// helpers

  // maximum matrices loaded onto
  // draw queue ssbo
  cx8 QUEUE_SZ=128;

  // ^matrices per queue
  struct Draw_Queue_Mats {
    glm::mat4 model[QUEUE_SZ];
    glm::mat4 nmat[QUEUE_SZ];

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

  enum {

    VBO,
    IBO,

    NUM_BUFFS

  };

  typedef std::vector<ANS> Anim_Meta;

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

  Texture       m_texture;
  Anim_Meta     m_anim_meta;
  Sprite::Anims m_anims;

// ---   *   ---   *   ---
// guts

  // wrap around the boiler for
  // gl-subdata into m_buff[idex]
  void upload(

    uint64_t idex,

    uint64_t offset,
    uint64_t sz,

    void*    data

  );

  // ^ice for vertex buffer
  inline void upload_verts(

    uint64_t cnt,
    uint64_t sz,

    void*    data

  ) {

    this->upload(VBO,m_vcount*sz,cnt*sz,data);
    m_vcount+=cnt;

  };

  // ^ice for index buffer
  inline void upload_indices(

    uint64_t cnt,
    uint64_t sz,

    void*    data

  ) {

    this->upload(IBO,m_icount*sz,cnt*sz,data);
    m_icount+=cnt;

  };

  // upload CRK verts to buffers
  uint32_t new_mesh(
    CRK::Prim& p,
    uint32_t   mode=GL_TRIANGLES

  );

  CRK::Mesh defcube(void);

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
  ~Meshes(void);

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

  // bind buffers
  inline void use(void) {
    glBindVertexArray(m_vao);
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
