// ---   *   ---   *   ---
// SPRITE
// Little devils
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include "mesh/Sprite.hpp"

// ---   *   ---   *   ---
// run current animation

uint32_t Sprite::play(void) {

  uint32_t out=m_cframe++;

  // cap current frame
  if(m_cframe == m_canim->end) {
    m_cframe=m_canim->beg;

  };

  m_sheet.use();

  // ret mesh idex
  return out;

};

// ---   *   ---   *   ---
