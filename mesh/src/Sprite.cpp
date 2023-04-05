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

  uint32_t out=m_cpose++;

  // get ctx
  auto& meta  = fetch_meta(m_src);
  auto& sheet = fetch_sheet(m_src);
  auto& anim  = meta.get(m_canim);

  // cap current frame
  if(m_cpose == anim.end) {
    m_cpose=anim.beg;

  };

  sheet.use();

  // ret mesh idex
  return out;

};

// ---   *   ---   *   ---
