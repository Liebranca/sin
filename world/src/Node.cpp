// ---   *   ---   *   ---
// NODE
// An abstract generality
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include "world/Node.hpp"
  #include "mesh/Sprite.hpp"

// ---   *   ---   *   ---
// cstruc

void Node::nit(Bld& bld) {

  m_xform     = bld.xform;
  m_draw_data = bld.draw;

  this->calc_bounds();

};

// ---   *   ---   *   ---
// regenerate physbody

void Node::calc_bounds(void) {

  glm::vec3 dim(0.5f,1,0.5f);

  m_bound.set(
    m_xform.get_model(),
    m_xform.position(),
    dim

  );

  m_updated.bounds=false;

};

// ---   *   ---   *   ---
// ^test against other

bool Node::boundschk(Node& other) {

  auto& s0=m_bound.sphere();
  auto& s1=other.m_bound.sphere();

  // get coldata
  auto col=s0.isect_sphere(s1);

  if(col.hit()) {
    glm::vec3 n=col.normal()*0.05f;
    this->set_linvel(n);

  };

  return col.hit();

};

// ---   *   ---   *   ---
