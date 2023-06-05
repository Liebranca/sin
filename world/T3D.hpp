#ifndef __5E_T3D_H__
#define __5E_T3D_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/GLM.hpp"

// ---   *   ---   *   ---
// consts

  CX mat4 IDENTITY(1);

  CX vec3 X_AXIS( 1, 0, 0);
  CX vec3 Y_AXIS( 0, 1, 0);
  CX vec3 Z_AXIS( 0, 0,-1);

  CX vec3 ID_VEC3(0,0,0);
  CX quat ID_QUAT(1,0,0,0);

// ---   *   ---   *   ---
// info

class T3D {

public:

  VERSION   "v2.00.7";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// helpers

private:

  // spares recalculation of
  // model matrix
  struct Model_Cache {

    mat4 model;
    mat3 nmat;

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

  vec3   m_position;
  quat   m_orientation;
  vec3   m_scaling;

  vec3   m_fwd = Z_AXIS;
  vec3   m_up  = Y_AXIS;
  vec3   m_hax = X_AXIS;

public:

// ---   *   ---   *   ---
// iface

  T3D(
    vec3 pos   = {0,0,0},
    quat rot   = {1,0,0,0},
    vec3 scale = {1,1,1}

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

  mat4  calc_model(bool igpar);
  mat4& get_model(bool igpar=false);

  inline mat3& get_nmat(void) {
    return m_cache.nmat;

  };

  inline void set_parent(T3D* par) {
    m_parent=par;
    m_cache.needs_update=1;

  }

  bool face_to(
    vec3 v,
    float     elapsed,

    float     rfac    = 10.0f,
    bool      fullRot = false

  );

  // selfex
  void rotate(quat delta);

  // update facing vectors
  // accto orientation
  void calc_facing(void);

  // add vel to position
  inline void move(vec3& vel) {
    m_position+=vel;
    m_cache.needs_update=1;

  };

  // ^overwrite
  inline void teleport(vec3& pos) {
    m_position=pos;
    m_cache.needs_update=1;

  };

  // getters
  // careful: these do not trigger update
  inline vec3& position(void) {
    return m_position;

  };

  inline quat& orientation(void) {
    return m_orientation;

  };

  // get facing vectors
  inline vec3& fwd(void) {
    return m_fwd;

  };

  inline vec3& up(void) {
    return m_up;

  };

  inline vec3& hax(void) {
    return m_hax;

  };

};

// ---   *   ---   *   ---

#endif // __5E_T3D_H__
