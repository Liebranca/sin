#ifndef __5E_SPRITE_H__
#define __5E_SPRITE_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/Style.hpp"

  #include "shader/Texture.hpp"
  #include "mesh/Mesh.hpp"

// ---   *   ---   *   ---
// info

class Sprite {

public:

  VERSION     "v0.00.1b";
  AUTHOR      "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  Mem<uint32_t> m_frames;

// ---   *   ---   *   ---
// iface

public:

  Sprite(uint16_t cnt);
  inline uint32_t& operator[](long i) {
    return m_frames[i];

  };

};

// ---   *   ---   *   ---

#endif // __5E_SPRITE_H__
