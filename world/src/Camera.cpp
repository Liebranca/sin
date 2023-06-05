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

mat4& Camera::get_view(void) {

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

  m_ubo.nit(

    GBuff::D_UNIFORM,

    sizeof(mat4),
    2

  );

  m_ubo.bind_base(idex);

};

// ---   *   ---   *   ---
// ^send

void Camera::update_ubo(bool which) {

  auto& data=(which)
    ? m_view
    : m_proj
    ;

  m_ubo.sub_data((void*) &data[0][0],which,1);

};

// ---   *   ---   *   ---
// cstruc

void Camera::nit(

  const vec3& pos,
  const quat& rot,

  Camera::Lens&    lens,

  uint32_t         bind_idex

) {

  m_lens=lens;

  vec3 npos=pos;
  quat nrot=rot;

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

};

// ---   *   ---   *   ---
