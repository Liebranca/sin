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

  m_draw_fn=(m_draw_data.type == SPRITE)
    ? &sprite_draw
    : &static_draw
    ;

};

// ---   *   ---   *   ---
