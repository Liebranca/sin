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
  #include "bitter/ff/JOJ.hpp"

// ---   *   ---   *   ---
// cstruc

void Texture::nit(std::string fpath) {

  JOJ src(fpath);
  src.unpack();

  auto pixels = src.to_buff(0,JOJ::UNPACK_ATLAS);
  m_img_sz    = src.get_atlas_sz();

// ---   *   ---   *   ---

  glGenTextures(1,&m_loc);
  glActiveTexture(GL_TEXTURE0);

  glBindTexture(
    GL_TEXTURE_2D_ARRAY,
    m_loc

  );

  glTexStorage3D(
    GL_TEXTURE_2D_ARRAY,

    4,GL_RGBA8,

    m_img_sz,m_img_sz,1

  );

  glTexSubImage3D(
    GL_TEXTURE_2D_ARRAY,
    0,0,0,0,

    m_img_sz,m_img_sz,1,
    GL_RGBA,

    GL_FLOAT,&pixels[0]

  );

// ---   *   ---   *   ---

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

  glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
  glBindTexture(GL_TEXTURE_2D_ARRAY,0);

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

void Texture::use(void) {

  glActiveTexture(GL_TEXTURE0+m_slot);
  glBindTexture(
    GL_TEXTURE_2D_ARRAY,
    m_loc

  );

};

// ---   *   ---   *   ---
