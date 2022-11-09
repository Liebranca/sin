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

Sprite::Sprite(uint16_t cnt) {

  Mem<uint32_t> frames(cnt);
  m_frames.copy(frames);

};

// ---   *   ---   *   ---
