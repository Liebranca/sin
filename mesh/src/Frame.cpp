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
  #include "mesh/Frame.hpp"

// ---   *   ---   *   ---

Meshes::Meshes(void) {

  // gl alloc
  glGenVertexArrays(1,&m_vao);
  glGenBuffers(NUM_BUFFS,&m_buff[0]);
  glBindVertexArray(m_vao);

  // nit data
  glBindBuffer(GL_ARRAY_BUFFER,m_buff[VBO]);

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

// ---   *   ---   *   ---

//  glGenBuffers(1,&m_ubo);
//  glBindBuffer(GL_UNIFORM_BUFFER,m_ubo);
//
//  glBufferData(
//    GL_UNIFORM_BUFFER,
//
//    sizeof(mat4),
//    NULL,
//
//    GL_DYNAMIC_DRAW
//
//  );
//
//  glBindBufferBase(
//    GL_UNIFORM_BUFFER,
//    1,m_ubo
//
//  );

// ---   *   ---   *   ---
// indices

  glBindBuffer(
    GL_ELEMENT_ARRAY_BUFFER,
    m_buff[IBO]

  );

  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,

    Meshes::BUFF_SZ
  * sizeof(uint16_t),

    NULL,
    GL_STATIC_DRAW

  );

// ---   *   ---   *   ---
// tilefetch buffer

  glBindBuffer(
    GL_SHADER_STORAGE_BUFFER,
    m_buff[TILE_SSBO]

  );

  glBufferData(
    GL_SHADER_STORAGE_BUFFER,

    Meshes::BUFF_SZ
  * sizeof(uint32_t),

    NULL,
    GL_DYNAMIC_DRAW

  );

  glBindBufferBase(
    GL_SHADER_STORAGE_BUFFER,
    0,

    m_buff[TILE_SSBO]

  );

// ---   *   ---   *   ---
// get mem

  m_tiles=std::unique_ptr<uint32_t>(
    new uint32_t[Meshes::BUFF_SZ]

  );

};

// ---   *   ---   *   ---
// destroy

Meshes::~Meshes(void) {
  glDeleteBuffers(NUM_BUFFS,&m_buff[0]);
  glDeleteVertexArrays(1,&m_vao);

};

// ---   *   ---   *   ---
// pushes geometry to gl buffer

uint32_t Meshes::nit(CRK::Prim& p) {

  for(auto& i : p.indices) {
    i+=m_vcount;

  };

  void*    verts   = p.verts.data();
  void*    indices = p.indices.data();

  uint16_t vcount  = p.verts.size();
  uint16_t icount  = p.indices.size();

  // fill out struct
  m_mesh[m_top]=Mesh(

    m_top,
    m_icount*sizeof(uint16_t),

    vcount,
    icount

  );

// ---   *   ---   *   ---

  // verts
  glBindBuffer(GL_ARRAY_BUFFER,m_buff[VBO]);
  glBufferSubData(
    GL_ARRAY_BUFFER,

    m_vcount
  * sizeof(CRK::Vertex),

    vcount
  * sizeof(CRK::Vertex),

    verts

  );

  m_vcount+=vcount;

// ---   *   ---   *   ---
// indices

  glBindBuffer(
    GL_ELEMENT_ARRAY_BUFFER,
    m_buff[IBO]

  );

  glBufferSubData(
    GL_ELEMENT_ARRAY_BUFFER,

    m_icount
  * sizeof(uint16_t),

    icount
  * sizeof(uint16_t),

    indices

  );

  m_icount+=icount;

  return m_top++;

};

// ---   *   ---   *   ---

std::vector<uint32_t> Meshes::make_sprite(
  std::string fpath

) {

  std::vector<uint32_t> out;

  CRK crk(fpath);
  crk.unpack();

  auto me=crk.data();

  for(auto& p : me) {
    out.push_back(this->nit(p));

  };

  return out;

};

// ---   *   ---   *   ---

void Meshes::update_tiles(void) {

  glBindBuffer(
    GL_SHADER_STORAGE_BUFFER,
    m_buff[TILE_SSBO]

  );

  glBufferSubData(
    GL_SHADER_STORAGE_BUFFER,0,

    Meshes::BUFF_SZ
  * sizeof(uint32_t),

    m_tiles.get()

  );

};

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
