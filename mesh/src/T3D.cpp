// ---   *   ---   *   ---
// TRANSFORM 3D
// For all your spatial needs
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include <glm/gtx/transform.hpp>
  #include <glm/gtc/quaternion.hpp>

  #include "mesh/T3D.hpp"

// ---   *   ---   *   ---
// recalculates model matrix

glm::mat4 T3D::calc_model(bool igpar) {

  glm::mat4 parmat;

  glm::mat4 wpos=glm::translate(m_position);
  glm::mat4 wrot=glm::mat4_cast(
    glm::normalize(m_orientation)

  );

  glm::mat4 wscale=glm::scale(m_scaling);

  parmat=(m_parent==NULL || igpar)
    ? IDENTITY
    : m_parent->get_model()
    ;

  // TODO: separately frac xforms!
  return wpos * wrot * wscale;

};

// ---   *   ---   *   ---
// ^fetches or recalculates

glm::mat4& T3D::get_model(bool igpar) {

  if(

     m_cache.igpar != igpar
  || m_cache.needs_update

  ) {

    m_cache.model = this->calc_model(igpar);
    m_cache.nmat  = glm::inverseTranspose(
      m_cache.model
    );

    m_cache.needs_update=0;

  };

  return m_cache.model;

};

// ---   *   ---   *   ---
// this one needs tuning ;>

bool T3D::face_to(
  glm::vec3 v,

  float elapsed,
  float rfac,
  bool fullRot

) {

  return false;

//  glm::vec3 vecTo;
//
//  if (!fullRot) {
//    vecTo = -(
//      xzvec(v)-xzvec(this->position)
//
//    );
//
//  } else {
//    vecTo = -(v - this->position);
//
//  };
//
//  glm::quat delta=glm::quatLookAt(vecTo,Y_AXIS);
//  glm::quat& rot=this->orientation;
//
//  if(delta!=rot) {
//    this->tick += elapsed;
//
//    float p=this->tick/rfac;
//    rot=glm::slerp(rot,delta,p);
//    this->rotate({1,0,0,0});
//
//    if(p * rfac * 10.0f > 0.8f) {
//      rot=delta;
//
//    };
//
//    return p * rfac * 10.0f > 0.3f;
//  }
//
//  this->tick=0.0f;
//  return true;

};

// ---   *   ---   *   ---

void T3D::rotate(glm::quat delta) {

  m_orientation=glm::normalize(
    glm::cross(m_orientation,delta)

  );

  m_fwd=

    m_orientation.y
  * m_orientation.w

  * 2
  ;

// ---   *   ---   *   ---
// clamp like a mother

  if(fabs(m_orientation.w) < 0.5) {
    m_dirn.y=-1;

  } else {
    m_dirn.y=1;

  };

  if (fabs(m_orientation.y) > 0.75) {
    m_dirn.x=-1;

  } else {
    m_dirn.x=1;

  };

  m_cache.needs_update=1;

};

// ---   *   ---   *   ---
