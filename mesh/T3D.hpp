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

  VERSION   "v2.00.3";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// helpers

private:

  // spares recalculation of
  // model matrix
  struct Model_Cache {

    glm::mat4 model;
    glm::mat3 nmat;

    union {

      struct {
        uint8_t igpar: 1;
        uint8_t needs_update: 1;

      };

      uint8_t flags;

    };

  };

// ---   *   ---   *   ---
// attrs

  Model_Cache m_cache;

  T3D*        m_parent;

  glm::vec3   m_position;
  glm::quat   m_orientation;
  glm::vec3   m_scaling;

  glm::vec2   m_dirn;

  float       m_fwd  = 0.0f;
  float       m_tick = 0.0f;

public:

// ---   *   ---   *   ---
// iface

  T3D(
    glm::vec4 pos   = {0,0,0,1},
    glm::quat rot   = {1,0,0,0},
    glm::vec3 scale = {1,1,1}

  ):

  // holy initializers batman
  m_position(pos),
  m_orientation(rot),
  m_scaling(scale) {

    m_dirn   = {1,1};
    m_parent = NULL;

    this->rotate(rot);

  };

// ---   *   ---   *   ---

  ~T3D(void) {};

  glm::mat4  calc_model(bool igpar);
  glm::mat4& get_model(bool igpar=false);

  inline glm::mat3& get_nmat(void) {
    return m_cache.nmat;

  };

  inline void set_parent(T3D* par) {
    m_parent=par;
    m_cache.needs_update=1;

  }

  bool face_to(
    glm::vec3 v,
    float     elapsed,

    float     rfac    = 10.0f,
    bool      fullRot = false

  );

  void rotate(glm::quat delta);

  inline void move(glm::vec3 pos) {
    m_position+=pos;
    m_cache.needs_update=1;

  };

};

// ---   *   ---   *   ---

#endif // __24_T3D_H__
