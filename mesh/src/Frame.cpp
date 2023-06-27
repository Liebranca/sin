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
  uint32_t pidex,
  uint32_t texsz

) {

  m_pidex=pidex;

  // nit buffs
  m_vao.nit(

    GBuff::D_ARRAY,
    GBuff::D_ELEMENT,

    sizeof(CRK::Vertex),
    Meshes::BUFF_SZ,

    sizeof(uint16_t),
    Meshes::BUFF_SZ

  );

  // ^vertex attrs
  m_vao.vattr(

    VAO::U32_4,

    sizeof(CRK::Vertex),
    offsetof(CRK::Vertex,data)

  );

  // reserve texture array
  m_texture.nit(texsz);
  m_nitted=true;

  m_vao.unbind();

};

// ---   *   ---   *   ---
// use/dont use

void Meshes::bind(void) {
  m_vao.bind();
  m_texture.bind();

};

void Meshes::unbind(void) {
  m_vao.unbind();
  m_texture.unbind();

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
  this->push_verts(vcount,verts);
  this->push_indices(icount,indices);

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
  m_mesh[idex].nit(

    vcount,
    icount,

    mode,

    voffset,
    ioffset

  );

  // send data to glbuffs
  this->repl_verts(voffset,vcount,verts);
  this->repl_indices(ioffset,icount,indices);

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

uint32_t Meshes::new_edit(uint64_t size) {

  uint32_t out=m_anims.size();

  m_texture.new_matid(out);
  m_anims.push_back(Sprite::Poses());

  // upload poses to glbuff
  auto& poses = m_anims.back();
  auto  p     = CRK::Prim(size);

  poses.push_back(this->new_mesh(p));

  // generate anim data ice
  m_anim_meta.push_back(ANS());
  m_anim_meta.back().enit();

  return out;

};

// ---   *   ---   *   ---
// pass image data to material

void Meshes::set_texture(
  uint32_t    meshid,
  std::string fpath

) {

  PNG src(fpath);
  m_texture.upload(src,this->matof(meshid));

};

// ---   *   ---   *   ---
// overwrite vertex data in slot

void Meshes::repl(
  uint32_t  beg,
  CRK::Prim p,

  uint32_t  mode

) {

  if(! this->repl_mesh(beg,p,mode)) {

    fprintf(
      stderr,
      "meshid %08X too small for repl\n",

      beg

    );

    exit(1);

  };

};

// ---   *   ---   *   ---
