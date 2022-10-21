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

  #include "bitter/ff/JOJ.hpp"
  #include "shader/Texture.hpp"

// ---   *   ---   *   ---
// make

Texture::Texture(std::string fpath) {

  JOJ src(fpath);
  src.unpack();

  auto pixels = src.to_buff(1);
  m_img_sz    = src.get_img_sz();

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

    GL_FLOAT,pixels.get()

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
// ^destroy

Texture::~Texture(void) {
  glBindTexture(GL_TEXTURE_2D_ARRAY,0);
  glDeleteTextures(1,&m_loc);

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
