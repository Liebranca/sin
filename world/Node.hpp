#ifndef __5E_NODE_H__
#define __5E_NODE_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/GLM.hpp"
  #include "gaoler/Bound.hpp"

  #include "world/T3D.hpp"

// ---   *   ---   *   ---
// info

class Node {

public:

  VERSION   "v0.00.8b";
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

  // for keeping track of
  // state changes
  struct States {

    bool bounds;
    bool movement;

    bool visibility;

  };

// ---   *   ---   *   ---
// defd by SIN

  void draw(void);

  void lin_fmotion(void);
  void ang_fmotion(void);

// ---   *   ---   *   ---
// attrs

private:

  Draw_Data   m_draw_data;

  Gaol::Bound m_bound;

  vec3   m_lindirn = {0,0,0};
  vec3   m_linvel  = {0,0,0};

  vec3   m_angdirn = {0,0,0};
  vec3   m_angvel  = {0,0,0};

  bool        m_still   = true;
  bool        m_visible = false;

// ---   *   ---   *   ---
// ctrash

protected:

  T3D         m_xform;
  States      m_updated = {0};

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

  // local vectors
  inline vec3& get_hax(void) {
    return m_xform.hax();

  };

  inline vec3& get_up(void) {
    return m_xform.up();

  };

  inline vec3& get_fwd(void) {
    return m_xform.fwd();

  };

  inline vec3& get_pos(void) {
    return m_xform.position();

  };

  inline quat& get_rot(void) {
    return m_xform.orientation();

  };

  // calc point in front of self
  inline vec3 get_fwd_cast(
    float dist=3.5f

  ) {

    return

      (this->get_pos())
    + (this->get_fwd() * dist)

    ;

  };

  // setters
  inline void set_linvel(vec3& vel) {
    m_linvel = vel;
    m_still  = false;

  };

  inline void set_angvel(vec3& vel) {
    m_angvel = vel;
    m_still  = false;

  };

  inline void set_lindirn(vec3& dirn) {

    m_lindirn = dirn;
    m_still   = false;

    m_updated.movement = true;

  };

  inline void set_lindirn_ax(
    uint8_t ax,
    float   value

  ) {

    m_lindirn[ax] = value;
    m_still       = false;

    m_updated.movement = true;

  };

  inline void set_angdirn(vec3& dirn) {

    m_angdirn = dirn;
    m_still   = false;

    m_updated.movement = true;

  };

  inline void set_angdirn_ax(
    uint8_t ax,
    float   value

  ) {

    m_angdirn[ax] = value;
    m_still       = false;

    m_updated.movement = true;

  };

  inline void move(vec3& vel) {

    m_xform.move(vel);

    m_updated.bounds   = true;
    m_updated.movement = true;

  };

  inline void rotate(quat& delta) {

    m_xform.rotate(delta);
    m_xform.calc_facing();

    m_updated.bounds   = true;
    m_updated.movement = true;

  };

  inline void teleport(vec3& to) {

    m_xform.teleport(to);

    m_updated.bounds   = true;
    m_updated.movement = true;

  };

  inline void halt(void) {

    m_still   = true;

    m_lindirn = {0,0,0};
    m_linvel  = {0,0,0};
    m_angdirn = {0,0,0};
    m_angvel  = {0,0,0};

  };

  // regenerate physbody
  void calc_bounds(void);

  // ^test against other
  bool boundschk(Node& other);

  // toggle visibility
  inline void set_visible(bool x) {
    m_visible=x;
    m_updated.visibility=true;

  };

  // ^get
  inline bool is_visible(void) {
    return m_visible;

  };

};

// ---   *   ---   *   ---

#endif // __5E_NODE_H__
