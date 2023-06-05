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

void Meshes::nit(
  uint32_t texsz,
  uint32_t pidex

) {

  m_pidex=pidex;

  // gl alloc
  glGenVertexArrays(1,&m_vao);
  glBindVertexArray(m_vao);
  glGenBuffers(NUM_BUFFS,&m_buff[0]);

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

  m_texture.nit(texsz);
  m_nitted=true;

  // unbind
  glBindVertexArray(0);

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

  m_mesh[m_top].set_caps(vcount,icount);

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
// ^overwrite

bool Meshes::repl_mesh(

  uint32_t   idex,
  CRK::Prim& p,

  uint32_t   mode

) {

  void*    verts   = p.verts.data();
  void*    indices = p.indices.data();

  uint16_t vcount  = p.verts.size();
  uint16_t icount  = p.indices.size();

  auto&    me      = m_mesh[idex];

  // exit if slot cant hold data
  if(! me.room_for(vcount,icount)) {
    return false;

  };

  uint16_t voffset = me.get_voffset();
  uint16_t ioffset = me.get_ioffset();

  // fill out struct
  m_mesh[idex]=Mesh(

    vcount,
    icount,

    mode,

    voffset,
    ioffset

  );

  // send data to glbuffs
  this->repl_verts(
    voffset,vcount,sizeof(CRK::Vertex),verts

  );

  this->repl_indices(
    ioffset,icount,sizeof(uint16_t),indices

  );

  return true;

};

// ---   *   ---   *   ---
// load sprite sheet from file

uint32_t Meshes::load_asset(

  std::string fpath_base,

  uint8_t     tex_beg,
  uint8_t     tex_type

) {

  uint32_t out=m_anims.size();

  JOJ joj(fpath_base+".joj");
  CRK crk(fpath_base+".crk");

  // upload textures to glbuff
  m_texture.upload(joj,tex_beg,tex_type);
  m_anims.push_back(Sprite::Poses());

  // upload poses to glbuff
  auto& poses = m_anims.back();
  auto& me    = crk.data();

  for(auto& p : me) {

    if(tex_type==Texture::ATLAS) {
      p.gen_qa_indices();

    };

    poses.push_back(this->new_mesh(p));

  };

  // get animation data
  m_anim_meta.push_back(ANS());
  m_anim_meta.back().nit(fpath_base+".ans");

  return out;

};

// ---   *   ---   *   ---
// ^fetch

Sprite Meshes::ice_asset(uint32_t src) {

  Sprite out;

  out.set_src(src);
  out.set_anim("idle");

  return out;

};

// ---   *   ---   *   ---
// create editable mesh

uint32_t Meshes::new_edit(void) {

  uint32_t out=m_anims.size();

  m_texture.new_matid(out);
  m_anims.push_back(Sprite::Poses());

  // upload poses to glbuff
  auto& poses = m_anims.back();
  auto  p     = CRK::Prim(0x100);

  poses.push_back(this->new_mesh(p));

  // generate anim data ice
  m_anim_meta.push_back(ANS());
  m_anim_meta.back().enit();

  return out;

};

// ---   *   ---   *   ---
// overwrite vertex data in slot

void Meshes::repl(
  uint32_t  beg,
  CRK::Prim p

) {

  if(! this->repl_mesh(beg,p)) {

    fprintf(
      stderr,
      "meshid %08X too small for repl",

      beg

    );

    exit(1);

  };

};

// ---   *   ---   *   ---
