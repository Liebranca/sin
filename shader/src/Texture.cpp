// ---   *   ---   *   ---
// TEXTURE
// Buncha colors
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb

// ---   *   ---   *   ---
// deps

  #include <GL/glew.h>
  #include "shader/Texture.hpp"

// ---   *   ---   *   ---
// cstruc

void Texture::nit(
  uint64_t sz,
  uint64_t slot

) {

  m_img_sz=sz;

  glGenTextures(1,&m_loc);
  this->bind();

  // alloc
  glTexStorage3D(
    GL_TEXTURE_2D_ARRAY,
    MAX_MIP,GL_RGBA8,

    m_img_sz,m_img_sz,

    MAX_DEPTH

  );

  // ^long list of settings
  glTexParameteri(
    GL_TEXTURE_2D_ARRAY,

    GL_TEXTURE_BASE_LEVEL,
    0

  );

  glTexParameteri(
    GL_TEXTURE_2D_ARRAY,

    GL_TEXTURE_MAX_LEVEL,
    3

  );

  glTexParameteri(
    GL_TEXTURE_2D_ARRAY,

    GL_TEXTURE_WRAP_S,
    GL_REPEAT

  );

  glTexParameteri(
    GL_TEXTURE_2D_ARRAY,

    GL_TEXTURE_WRAP_T,
    GL_REPEAT

  );

  glTexParameterf(
    GL_TEXTURE_2D_ARRAY,

    GL_TEXTURE_MIN_FILTER,
    GL_NEAREST_MIPMAP_NEAREST

  );

  glTexParameterf (
    GL_TEXTURE_2D_ARRAY,

    GL_TEXTURE_MAG_FILTER,
    GL_NEAREST

  );

};

// ---   *   ---   *   ---
// ^dstruc

Texture::~Texture(void) {

  if(m_loc) {
    glBindTexture(GL_TEXTURE_2D_ARRAY,0);
    glDeleteTextures(1,&m_loc);

  };

};

// ---   *   ---   *   ---
// puts data into buff

void Texture::upload(

  JOJ&     joj,

  uint32_t start,
  uint8_t  mode

) {

  uint8_t joj_mode;
  uint8_t step;

  // unpack all maps in file
  if(mode==LAYERS) {
    joj_mode = JOJ::UNPACK_LAYERS;
    step     = 3;

  // ^atlas for spritesheets
  } else {
    joj_mode = JOJ::UNPACK_ATLAS;
    step     = 1;

  };

  auto pixels=joj.to_buff(0,joj_mode);

  Range r={
    .beg=start,
    .end=start+step

  };

  glTexSubImage3D(

    GL_TEXTURE_2D_ARRAY,0,

    0,0,

    r.beg,

    m_img_sz,m_img_sz,MAX_DEPTH,

    GL_RGBA,GL_FLOAT,

    &pixels[0]

  );

  // book-keeping
  if(m_top == r.beg) {
    m_materials.push_back(r);

  };

  m_top=(r.end > m_top)
    ? r.end
    : m_top
    ;

  m_umark.push_back(r);

};

// ---   *   ---   *   ---
// get N layers will fit
// without overwrite

uint32_t Texture::can_push(uint32_t n) {

  uint32_t beg = m_top;
  uint32_t rei = 0;

  bool     re  = false;

  // first try top of stack
  if((beg+n) < MAX_DEPTH) {
    return beg;

  };

  // ^else try reusing
  // orphaned space
  for(auto& r : m_rmark) {
    if(r.end == r.beg+n) {
      re=true;
      break;

    };

    rei++;

  };

  // ^match found
  if(re) {
    auto& r=m_rmark[rei];
    beg=r.beg;

    m_rmark.erase(m_rmark.begin()+rei);

    return beg;

  };

  // signal new texture needed
  return OVERWRITE;

};

// ---   *   ---   *   ---
// use/dont use

void Texture::bind(void) {

  glActiveTexture(GL_TEXTURE0+m_slot);
  glBindTexture(
    GL_TEXTURE_2D_ARRAY,
    m_loc

  );

};

void Texture::unbind(void) {
  glBindTexture(GL_TEXTURE_2D_ARRAY,0);

};

// ---   *   ---   *   ---
