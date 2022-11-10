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
  #include "bitter/ff/JOJ.hpp"

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
  * sizeof(Mesh::Vertex),

    NULL,
    GL_DYNAMIC_DRAW

  );

  // set attrib pointers
  glEnableVertexAttribArray(0);
  glVertexAttribIPointer(
    0,4,GL_UNSIGNED_INT,

    sizeof(Mesh::Vertex),
    (void*) offsetof(Mesh::Vertex,data)

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

uint32_t Meshes::nit(

  void*    verts,
  void*    indices,

  uint16_t vcount,
  uint16_t icount

) {

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
  * sizeof(Mesh::Vertex),

    vcount
  * sizeof(Mesh::Vertex),

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

void Meshes::push_quad(

  Mesh::Build& bld,
  uint64_t     desc,

  float        tile_step,
  float        atlas_step

) {

  Mesh::Prim& me = bld.me;

  uint16_t    v  = bld.vert;
  uint16_t    i  = bld.idex;

  uint16_t co[2]={
    uint16_t(desc&0xFFFF),
    uint16_t((desc>>16)&0xFFFF)

  };

  uint16_t uv[2]={
    uint16_t((desc>>32)&0xFFFF),
    uint16_t((desc>>48)&0xFFFF)

  };

// ---   *   ---   *   ---

  float step=
    Frac::STEP[Frac::STEP_8BIT];

  uint8_t nbits=
    Frac::BITS[Frac::SIZE_8BIT];

  float co_f[2]={
    float(co[0])*tile_step,
    float(co[1])*tile_step

  };

  float uv_f[2]={
    float(uv[0])*atlas_step,
    float(uv[1])*atlas_step

  };

  uint16_t id=m_top;

// ---   *   ---   *   ---

  uint8_t co_x[2]={

    frac<uint8_t>(
      co_f[0],step,nbits,Frac::UNSIGNED

    ),

    frac<uint8_t>(
      co_f[0]+tile_step,step,nbits,Frac::UNSIGNED

    )

  };

  uint8_t co_y[2]={

    frac<uint8_t>(
      co_f[1],step,nbits,Frac::UNSIGNED

    ),

    frac<uint8_t>(
      co_f[1]+tile_step,step,nbits,Frac::UNSIGNED

    )

  };

// ---   *   ---   *   ---

  uint8_t uv_x[2]={

    frac<uint8_t>(
      uv_f[0],step,nbits,Frac::UNSIGNED

    ),

    frac<uint8_t>(
      uv_f[0]+atlas_step,step,nbits,Frac::UNSIGNED

    )

  };

  uint8_t uv_y[2]={

    frac<uint8_t>(
      uv_f[1],step,nbits,Frac::UNSIGNED

    ),

    frac<uint8_t>(
      uv_f[1]+atlas_step,step,nbits,Frac::UNSIGNED

    )

  };

// ---   *   ---   *   ---

  // 1,-1
  me.verts[v+0]=(Mesh::Vertex) {

    .XYZ = {co_x[1],co_y[0],0x80},
    .TEX = {uv_x[1],uv_y[0]},

    .ID  = id

  };

  // -1,-1
  me.verts[v+1]=(Mesh::Vertex) {

    .XYZ = {co_x[0],co_y[0],0x80},
    .TEX = {uv_x[0],uv_y[0]},

    .ID  = id

  };

  // -1,1
  me.verts[v+2]=(Mesh::Vertex) {

    .XYZ = {co_x[0],co_y[1],0x80},
    .TEX = {uv_x[0],uv_y[1]},

    .ID  = id

  };

  // 1,1
  me.verts[v+3]=(Mesh::Vertex) {

    .XYZ = {co_x[1],co_y[1],0x80},
    .TEX = {uv_x[1],uv_y[1]},

    .ID  = id

  };

// ---   *   ---   *   ---

  me.indices[i+0]=m_vcount+bld.vert+0;
  me.indices[i+1]=m_vcount+bld.vert+1;
  me.indices[i+2]=m_vcount+bld.vert+2;

  me.indices[i+3]=m_vcount+bld.vert+2;
  me.indices[i+4]=m_vcount+bld.vert+3;
  me.indices[i+5]=m_vcount+bld.vert+0;

  bld.vert+=4;
  bld.idex+=6;

};

// ---   *   ---   *   ---

Mesh::Prim Meshes::make_sprite_frame(
  std::vector<uint64_t>& tab,
  uint64_t& offset

) {

  Mesh::Prim  me;
  Mesh::Build bld={

    .me   = me,

    .vert = 0,
    .idex = 0

  };

  uint64_t tile_cnt = tab[offset+0];
  uint16_t i        = ++offset;

  // scale multipliers for each quad
  uint64_t steps    = tab[1];

  // 4 verts (2 tris) per tile
  // 3 indices per tri
  me.verts.resize(4*tile_cnt);
  me.indices.resize(6*tile_cnt);

  // scales vertex coords
  uint64_t steps_u = steps&0xFFFFFFFF;
  float tile_step  = *((float*) &steps_u);

  // scales texture coords
  steps_u=steps>>32;
  float atlas_step=*((float*) &steps_u);

  while(i<offset+tile_cnt) {

    this->push_quad(
      bld,tab[i++],
      tile_step,
      atlas_step

    );

  };

  offset=i;

  return me;

};

// ---   *   ---   *   ---

uint32_t Meshes::make_prim(

  std::vector<uint64_t>& tab,

  uint64_t& offset,
  uint16_t  type

) {

  uint32_t   out;
  Mesh::Prim me;

  switch(type) {

  case Mesh::SPRITE_FRAME:
    me=this->make_sprite_frame(tab,offset);
    break;

  };

  return this->nit(
    me.verts.data(),
    me.indices.data(),
    me.verts.size(),
    me.indices.size()

  );

};

// ---   *   ---   *   ---

std::vector<uint32_t> Meshes::make_sprite(

  Texture& atlas,
  uint16_t idex

) {

  std::vector<uint32_t> out;
  std::vector<uint64_t>& tab=atlas.get_tab();

  uint64_t cnt=tab[0];
  uint64_t offset=2;

  for(uint64_t i=0;i<cnt;i++) {

    out.push_back(this->make_prim(
      tab,offset,Mesh::SPRITE_FRAME

    ));

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
