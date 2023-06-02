#ifndef __5E_T3D_H__
#define __5E_T3D_H__

// ---   *   ---   *   ---
// deps

  #include <glm/glm.hpp>
  #include <glm/gtx/quaternion.hpp>
  #include <glm/gtc/matrix_inverse.hpp>

  #include "bitter/kvrnel/Style.hpp"

// ---   *   ---   *   ---
// consts

  CX glm::mat4 IDENTITY(1);

  CX glm::vec3 X_AXIS( 1, 0, 0);
  CX glm::vec3 Y_AXIS( 0, 1, 0);
  CX glm::vec3 Z_AXIS( 0, 0,-1);

  CX glm::vec3 ID_VEC3(0,0,0);
  CX glm::quat ID_QUAT(1,0,0,0);

// ---   *   ---   *   ---
// info

class T3D {

public:

  VERSION   "v2.00.6";
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

  glm::vec3   m_fwd = Z_AXIS;
  glm::vec3   m_up  = Y_AXIS;
  glm::vec3   m_hax = X_AXIS;

public:

// ---   *   ---   *   ---
// iface

  T3D(
    glm::vec3 pos   = {0,0,0},
    glm::quat rot   = {1,0,0,0},
    glm::vec3 scale = {1,1,1}

  ):

  // holy initializers batman
  m_position(pos),
  m_orientation(rot),
  m_scaling(scale) {

    m_parent=NULL;
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

  // selfex
  void rotate(glm::quat delta);

  // update facing vectors
  // accto orientation
  void calc_facing(void);

  // add vel to position
  inline void move(glm::vec3& vel) {
    m_position+=vel;
    m_cache.needs_update=1;

  };

  // ^overwrite
  inline void teleport(glm::vec3& pos) {
    m_position=pos;
    m_cache.needs_update=1;

  };

  // getters
  // careful: these do not trigger update
  inline glm::vec3& position(void) {
    return m_position;

  };

  inline glm::quat& orientation(void) {
    return m_orientation;

  };

  // get facing vectors
  inline glm::vec3& fwd(void) {
    return m_fwd;

  };

  inline glm::vec3& up(void) {
    return m_up;

  };

  inline glm::vec3& hax(void) {
    return m_hax;

  };

};

// ---   *   ---   *   ---

#endif // __5E_T3D_H__
