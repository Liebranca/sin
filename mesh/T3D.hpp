#ifndef __24_T3D_H__
#define __24_T3D_H__

// ---   *   ---   *   ---
// deps

  #include <glm/glm.hpp>
  #include <glm/gtx/quaternion.hpp>
  #include <glm/gtc/matrix_inverse.hpp>

  #include "bitter/kvrnel/Style.hpp"

// ---   *   ---   *   ---
// consts

  CX glm::mat4 IDENTITY(1);
  CX glm::vec3 Y_AXIS(0,1,0);

// ---   *   ---   *   ---
// info

class T3D {

public:

  VERSION   "v2.00.1";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

  glm::vec3 position;
  glm::quat orientation;
  glm::vec3 scaling;
  glm::vec2 dirn;

  float fwd  = 0.0f;
  float tick = 0.0f;

  T3D* parent;

// ---   *   ---   *   ---
// iface

  T3D(
    glm::vec4 pos={0,0,0,1},
    glm::quat rot={1,0,0,0},
    glm::vec3 scale={1,1,1}

  ):

  // holy initializers batman
  position(pos),
  orientation(rot),
  scaling(scale) {

    dirn={1,1};
    parent=NULL;

    rotate(rot);

  };

// ---   *   ---   *   ---

  ~T3D(void) {};

  glm::mat4 get_model(bool ignoreParent=false);

  inline glm::mat3 get_normal(glm::mat4& model) {
    return glm::inverseTranspose(model);

  };

  inline void set_parent(T3D* par) {
    this->parent=par;

  }

  bool face_to(
    glm::vec3 v,
    float     elapsed,

    float     rfac    = 10.0f,
    bool      fullRot = false

  );

  void rotate(glm::quat delta);

  inline void move(glm::vec3& pos) {
    position+=pos;

  };

};

// ---   *   ---   *   ---

#endif // __24_T3D_H__
