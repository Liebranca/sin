// ---   *   ---   *   ---
// MESH FRAME
// Container for static meshes
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include <cstddef>
  #include <GL/glew.h>

  #include "bitter/kvrnel/Bytes.hpp"
  #include "bitter/ff/DAF.hpp"
  #include "bitter/ff/ANS.hpp"

  #include "shader/Texture.hpp"
  #include "mesh/Frame.hpp"

// ---   *   ---   *   ---

void Meshes::nit(uint32_t pidex) {

  m_pidex=pidex;

  // gl alloc
  glGenVertexArrays(1,&m_vao);
  glGenBuffers(NUM_BUFFS,&m_buff[0]);
  glBindVertexArray(m_vao);

  // get vertex mem
  glBindBuffer(
    GL_ARRAY_BUFFER,
    m_buff[VBO]

  );

  glBufferData(
    GL_ARRAY_BUFFER,

    Meshes::BUFF_SZ
  * sizeof(CRK::Vertex),

    NULL,
    GL_DYNAMIC_DRAW

  );

  // set attrib pointers
  glEnableVertexAttribArray(0);
  glVertexAttribIPointer(
    0,4,GL_UNSIGNED_INT,

    sizeof(CRK::Vertex),
    (void*) offsetof(CRK::Vertex,data)

  );

  // get idex mem
  glBindBuffer(
    GL_ELEMENT_ARRAY_BUFFER,
    m_buff[IBO]

  );

  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,

    Meshes::BUFF_SZ
  * sizeof(uint16_t),

    NULL,
    GL_DYNAMIC_DRAW

  );

// ---   *   ---   *   ---
// tilefetch buffer

//  glBindBuffer(
//    GL_SHADER_STORAGE_BUFFER,
//    m_buff[TILE_SSBO]
//
//  );
//
//  glBufferData(
//    GL_SHADER_STORAGE_BUFFER,
//
//    Meshes::BUFF_SZ
//  * sizeof(uint32_t),
//
//    NULL,
//    GL_DYNAMIC_DRAW
//
//  );
//
//  glBindBufferBase(
//    GL_SHADER_STORAGE_BUFFER,
//    0,
//
//    m_buff[TILE_SSBO]
//
//  );

  m_nitted=true;

// ---   *   ---   *   ---
// get mem

//  m_tiles=std::unique_ptr<uint32_t>(
//    new uint32_t[Meshes::BUFF_SZ]
//
//  );

};

// ---   *   ---   *   ---
// destroy

Meshes::~Meshes(void) {

  if(m_nitted) {
    glDeleteBuffers(NUM_BUFFS,&m_buff[0]);
    glDeleteVertexArrays(1,&m_vao);

  };

};

// ---   *   ---   *   ---
// wrap around the boiler for
// gl-subdata into m_buff[idex]

void Meshes::upload(

  uint64_t idex,

  uint64_t offset,
  uint64_t sz,

  void*    data

) {

  // TODO: make this a const array fetch
  uint32_t type=(idex==VBO)
    ? GL_ARRAY_BUFFER
    : GL_ELEMENT_ARRAY_BUFFER
    ;

  glBindBuffer(type,m_buff[idex]);
  glBufferSubData(type,offset,sz,data);

};

// ---   *   ---   *   ---
// upload CRK verts to buffers

uint32_t Meshes::new_mesh(
  CRK::Prim& p,
  uint32_t   mode

) {

  void*    verts   = p.verts.data();
  void*    indices = p.indices.data();

  uint16_t vcount  = p.verts.size();
  uint16_t icount  = p.indices.size();

  // fill out struct
  m_mesh[m_top]=Mesh(

    vcount,
    icount,

    mode,

    m_vcount,
    m_icount

  );

  // send data to glbuffs
  this->upload_verts(
    vcount,sizeof(CRK::Vertex),verts

  );

  this->upload_indices(
    icount,sizeof(uint16_t),indices

  );

  return m_top++;

};

// ---   *   ---   *   ---
// load sprite sheet from file

uint32_t Meshes::new_sprite(
  std::string& path

) {

  uint32_t out=m_anims.size();

  DAF daf(path,Bin::READ);
  daf.unpack();

  m_textures.push_back(Texture());
  m_textures.back().nit(path+"e0");

  CRK crk(path+"e1");
  crk.unpack();

  m_anims.push_back(Sprite::Poses());

  auto& poses = m_anims.back();
  auto& me    = crk.data();

  for(auto& p : me) {
    p.gen_qa_indices();
    poses.push_back(this->new_mesh(p));

  };

  m_anim_meta.push_back(ANS());
  m_anim_meta.back().nit(path+"e2");

  return out;

};

// ---   *   ---   *   ---
// ^copy

Sprite Meshes::ice_sprite(uint32_t src) {

  Sprite out;

  out.set_src(src);
  out.set_anim("run");

  return out;

};

// ---   *   ---   *   ---

//void Meshes::update_tiles(void) {
//
//  glBindBuffer(
//    GL_SHADER_STORAGE_BUFFER,
//    m_buff[TILE_SSBO]
//
//  );
//
//  glBufferSubData(
//    GL_SHADER_STORAGE_BUFFER,0,
//
//    Meshes::BUFF_SZ
//  * sizeof(uint32_t),
//
//    m_tiles.get()
//
//  );
//
//};

// ---   *   ---   *   ---

//void Meshes::update_matrices(void) {
//
//  glBindBuffer(GL_UNIFORM_BUFFER,m_ubo);
//  glBufferSubData(
//    GL_UNIFORM_BUFFER,0,
//    sizeof(m_mats),
//    (void*) &m_mats
//
//  );
//
//};

// ---   *   ---   *   ---
