// ---   *   ---   *   ---
// FBO
// How we do offscreen rendering
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include <GL/glew.h>

  #include "shader/Texture.hpp"
  #include "mesh/FBO.hpp"

// ---   *   ---   *   ---
// cstruc

void FBO::nit(uint64_t size) {

  m_size=size;
  glGenFramebuffers(1,&m_loc);

  this->bind();
  this->nit_texture();

  if(! this->validate()) {
    fprintf(stderr,"Bad FBO nit\n");
    exit(1);

  };

};

// ---   *   ---   *   ---
// ^nasty bit of texture setup boiler

void FBO::nit_texture(void) {

  m_texture.nit(m_size);
  uint32_t draw_buff[Texture::MATERIAL_DEPTH];

  for(

    uint8_t i=0;

    i<Texture::MATERIAL_DEPTH;
    i++

  ) {

    draw_buff[i]=GL_COLOR_ATTACHMENT0+i;

    glFramebufferTextureLayer(

      GL_DRAW_FRAMEBUFFER,
      draw_buff[i],

      m_texture.get_loc(),0,i

    );

  };

  glDrawBuffers(4,draw_buff);

};

// ---   *   ---   *   ---
// dstruc

FBO::~FBO(void) {

  if(m_loc) {
    glDeleteFramebuffers(1,&m_loc);

  };

};

// ---   *   ---   *   ---
// check ice is OK

bool FBO::validate(void) {

  bool out=
     glCheckFramebufferStatus(GL_FRAMEBUFFER)
  == GL_FRAMEBUFFER_COMPLETE
  ;

  return out;

};

// ---   *   ---   *   ---
// use/dont use

void FBO::bind(void) {
  glBindFramebuffer(GL_FRAMEBUFFER,m_loc);

};

void unbind(void) {
  glBindFramebuffer(GL_FRAMEBUFFER,0);

};

// ---   *   ---   *   ---
