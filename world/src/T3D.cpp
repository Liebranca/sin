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

  #include "bitter/kvrnel/Seph.hpp"

  #include "world/T3D.hpp"

// ---   *   ---   *   ---
// recalculates model matrix

mat4 T3D::calc_model(bool igpar) {

  mat4 parmat;

  mat4 wpos=glm::translate(m_position);
  mat4 wrot=glm::mat4_cast(
    glm::normalize(m_orientation)

  );

  mat4 wscale=glm::scale(m_scaling);

  parmat=(m_parent==NULL || igpar)
    ? IDENTITY
    : m_parent->get_model()
    ;

  // TODO: separately frac xforms!
  return wpos * wrot * wscale;

};

// ---   *   ---   *   ---
// ^fetches or recalculates

mat4& T3D::get_model(bool igpar) {

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
  vec3 v,

  float elapsed,
  float rfac,
  bool fullRot

) {

  return false;

//  vec3 vecTo;
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
//  quat delta=glm::quatLookAt(vecTo,Y_AXIS);
//  quat& rot=this->orientation;
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

void T3D::rotate(quat delta) {

  m_orientation=glm::normalize(
    m_orientation*delta

  );

  m_cache.needs_update=1;

};

// ---   *   ---   *   ---
// ^update facing vectors
// accto orientation

void T3D::calc_facing(void) {

  m_fwd=glm::normalize(
    Z_AXIS * m_orientation

  );

  m_hax=glm::normalize(
    glm::cross(Y_AXIS,m_fwd)

  );

  m_up=glm::normalize(
    glm::cross(m_fwd,m_hax)

  );

};

// ---   *   ---   *   ---
