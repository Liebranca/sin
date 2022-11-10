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

  VERSION     "v0.00.2b";
  AUTHOR      "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  uint64_t m_cur=0;

  std::vector<uint32_t> m_frames;

// ---   *   ---   *   ---
// iface

public:

  Sprite(std::vector<uint32_t>& frames) {
    m_frames=frames;

  };

  Sprite(std::vector<uint32_t> frames) {
    m_frames=frames;

  };

  inline uint32_t operator[](uint64_t i) {
    i-=i*(i>=m_frames.size());
    return m_frames[i];

  };

  inline uint32_t play(void) {

    uint32_t out=m_cur++;
    m_cur*=1-(m_cur>=m_frames.size());

    return out;

  };

};

// ---   *   ---   *   ---

#endif // __5E_SPRITE_H__
