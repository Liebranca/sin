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

glm::mat4 T3D::get_model(bool ignoreParent) {

  glm::mat4 parentMatrix;

  glm::mat4 wPosMatrix=glm::translate(position);
  glm::mat4 wRotMatrix=glm::mat4_cast(
    glm::normalize(orientation)

  );

  glm::mat4 wScaleMatrix=glm::scale(scaling);

  parentMatrix=(parent==NULL || ignoreParent)
    ? IDENTITY
    : parent->get_model()
    ;

  return (

    wPosMatrix
  * wRotMatrix
  * wScaleMatrix

  );

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

  this->orientation=glm::normalize(
    glm::cross(orientation, delta)

  );

  fwd=

    orientation.y
  * orientation.w

  * 2
  ;

// ---   *   ---   *   ---
// clamp like a mother

  if(fabs(orientation.w) < 0.5) {
    dirn.y=-1;

  } else {
    dirn.y=1;

  };

  if (fabs(orientation.y) > 0.75) {
    dirn.x=-1;

  } else {
    dirn.x=1;

  };

};

// ---   *   ---   *   ---
