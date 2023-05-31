#ifndef __5E_NODE_H__
#define __5E_NODE_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/Style.hpp"
  #include "gaoler/Bound.hpp"

  #include "mesh/T3D.hpp"

// ---   *   ---   *   ---
// info

class Node {

public:

  VERSION   "v0.00.5b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// helpers

  enum {

    ANIMATED = 0x01,
    STATIC   = 0x02

  };

  struct Draw_Data {

    uint32_t batch;
    uint32_t mesh;

    uint8_t  type;

  };

  // for cstruc
  struct Bld {
    Draw_Data draw;
    T3D       xform;

  };

// ---   *   ---   *   ---
// defd by SIN

  void draw(void);

// ---   *   ---   *   ---
// attrs

private:

  Draw_Data   m_draw_data;

  T3D         m_xform;

  Gaol::Bound m_bound;

  glm::vec3   m_linvel  = {0,0,0};
  glm::quat   m_angvel  = {1,0,0,0};

  bool        m_still   = true;
  bool        m_visible = false;

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  void nit(Bld& bld);

  // ctrash
  Node(void) {};
  ~Node(void) {};

  // getters
  inline Draw_Data& draw_data(void) {
    return m_draw_data;

  };

  inline T3D& xform(void) {
    return m_xform;

  };

  inline bool is_still(void) {
    return m_still;

  };

  inline Gaol::Bound& bound(void) {
    return m_bound;

  };

  // setters
  inline void set_linvel(glm::vec3& vel) {
    m_linvel = vel;
    m_still  = false;

  };

  inline void set_angvel(glm::quat& vel) {
    m_angvel = vel;
    m_still  = false;

  };

  inline void move(glm::vec3 vel) {
    m_xform.move(vel);
    this->calc_bounds();

  };

  inline void rot(glm::quat delta) {
    m_xform.rotate(delta);
    this->calc_bounds();

  };

  // ^moves accto set velocities
  inline void fmotion(void) {

    if(m_still) {return;};

    m_xform.move(m_linvel);
    m_xform.rotate(m_angvel);

    this->calc_bounds();

  };

  inline void halt(void) {

    m_still  = true;
    m_linvel = {0,0,0};
    m_angvel = {1,0,0,0};

  };

  // regenerate physbody
  void calc_bounds(void);

  // ^test against other
  bool boundschk(Node& other);

  // toggle visibility
  inline void set_visible(bool x) {
    m_visible=x;

  };

  // ^get
  inline bool is_visible(void) {
    return m_visible;

  };

};

// ---   *   ---   *   ---

#endif // __5E_NODE_H__
