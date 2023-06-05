// ---   *   ---   *   ---
// CAMERA
// Looks your way
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTOS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include <GL/glew.h>
  #include "world/Camera.hpp"

// ---   *   ---   *   ---
// fetch or regenerate view matrix

glm::mat4& Camera::get_view(void) {

  if(m_updated.movement) {

    m_view  = this->calc_view();
    m_stow  = this->calc_stow();

    m_updated.movement = false;

    this->update_frustum();
    this->update_ubo(1);

  };

  return m_view;

};

// ---   *   ---   *   ---
// for uploading mats to shader

void Camera::nit_ubo(uint32_t idex) {

  glGenBuffers(1,&m_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER,m_ubo);

  glBufferData(
    GL_UNIFORM_BUFFER,

    sizeof(glm::mat4)*2,
    NULL,

    GL_DYNAMIC_DRAW

  );

  glBindBufferBase(
    GL_UNIFORM_BUFFER,

    idex,
    m_ubo

  );

};

// ---   *   ---   *   ---
// ^send

void Camera::update_ubo(bool which) {

  uint64_t offset=(which)
    ? sizeof(glm::mat4)
    : 0
    ;

  auto& data=(which)
    ? m_view
    : m_proj
    ;

  glBindBuffer(GL_UNIFORM_BUFFER,m_ubo);
  glBufferSubData(
    GL_UNIFORM_BUFFER,

    offset,
    sizeof(data),

    (void*) &data[0][0]

  );

};

// ---   *   ---   *   ---
// cstruc

void Camera::nit(

  const glm::vec3& pos,
  const glm::quat& rot,

  Camera::Lens&    lens,

  uint32_t         bind_idex

) {

  m_lens=lens;

  glm::vec3 npos=pos;
  glm::quat nrot=rot;

  this->move(npos);
  this->rotate(nrot);

  this->nit_ubo(bind_idex);

  m_frustum.set(

    lens.width,
    lens.height,

    lens.fov,
    lens.near,
    lens.far

  );

}

// ---   *   ---   *   ---

Camera::~Camera(void) {

  if(m_ubo) {
    glDeleteBuffers(1,&m_ubo);

  };

};

// ---   *   ---   *   ---
