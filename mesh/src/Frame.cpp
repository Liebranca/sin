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
  #include "mesh/Frame.hpp"

// ---   *   ---   *   ---

void Meshes::nit(void) {

  // gl alloc
  glGenVertexArrays(1,&m_vao);
  glGenBuffers(1,&m_glbuff);
  glBindVertexArray(m_vao);

  // nit data
  glBindBuffer(GL_ARRAY_BUFFER,m_glbuff);

  glBufferData(
    GL_ARRAY_BUFFER,

    Meshes::BATCH_SZ
  * (sizeof(Mesh::Vertex)*3),

    NULL,
    GL_DYNAMIC_DRAW

  );

  // set attrib pointers
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0,4,GL_FLOAT,GL_FALSE,

    sizeof(Mesh::Vertex),
    (void*) offsetof(Mesh::Vertex,Position)

  );

//  glEnableVertexAttribArray(1);
//  glVertexAttribPointer(
//    1,4,GL_BYTE,GL_FALSE,
//
//    sizeof(Mesh::Vertex),
//    (void*) offsetof(Mesh::Vertex,c)
//
//  );

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
// shader stuff was here

};

// ---   *   ---   *   ---
// destroy

void Meshes::del(void) {
//  glDeleteBuffers(1,&m_ubo);
  glDeleteBuffers(1,&m_glbuff);
  glDeleteVertexArrays(1,&m_vao);

};

// ---   *   ---   *   ---
// pushes geometry to gl buffer

void Meshes::upload(

  void*    data,
  uint16_t vcount

) {

  glBindBuffer(GL_ARRAY_BUFFER,m_glbuff);

  glBufferSubData(
    GL_ARRAY_BUFFER,0,

    vcount
  * sizeof(Mesh::Vertex),

    data

  );

  m_vcount+=vcount;

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

void Meshes::use(void) {
  glBindVertexArray(m_vao);

};

void Meshes::draw(uint32_t idex) {

//  Mesh& me=m_buff[idex];

  glDrawArrays(
    GL_TRIANGLES,
    0,m_vcount

  );

};

// ---   *   ---   *   ---
