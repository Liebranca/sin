#ifndef __5E_NODE_H__
#define __5E_NODE_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/Style.hpp"
  #include "mesh/T3D.hpp"

// ---   *   ---   *   ---
// info

class Node {

public:

  VERSION   "v0.00.2b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// helpers

  enum {

    SPRITE=0x01,
    STATIC=0x02

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

  static void sprite_draw(Node* node);
  static void static_draw(Node* node);

  // ^ptr shorthand
  typedef void (*Draw_Fn) (Node* node);

// ---   *   ---   *   ---
// attrs

private:

  Draw_Data m_draw_data;
  Draw_Fn   m_draw_fn;

  T3D       m_xform;

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

  // convenience wrappers
  inline void draw(void) {m_draw_fn(this);};

  inline void move(glm::vec3 vel) {
    m_xform.move(vel);

  };

  inline void rot(glm::quat delta) {
    m_xform.rotate(delta);

  };

};

// ---   *   ---   *   ---

#endif // __5E_NODE_H__
