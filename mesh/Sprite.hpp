#ifndef __5E_SPRITE_H__
#define __5E_SPRITE_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/Style.hpp"
  #include "bitter/ff/ANS.hpp"

  #include "sin/shader/Texture.hpp"

// ---   *   ---   *   ---
// info

class Sprite {

public:

  VERSION     "v0.00.8b";
  AUTHOR      "IBN-3DILA";

  typedef std::vector<uint32_t> Poses;
  typedef std::vector<Poses>    Anims;

// ---   *   ---   *   ---
// defd by SIN

  static ANS&     fetch_meta(uint32_t idex);
  static Poses&   fetch_poses(uint32_t idex);

// ---   *   ---   *   ---
// attrs

private:

  float    m_ctime   = 0.0f;

  uint32_t m_cpose   = 0;
  uint32_t m_canim   = 0;
  uint32_t m_src     = 0;

// ---   *   ---   *   ---
// guts

  cxr32 FTIME=1.0f/4;

  // advances current pose each
  // N ticks of animation time
  void calc_time(void);

// ---   *   ---   *   ---
// iface

public:

  // ctrash
  Sprite(void) {};
  ~Sprite(void) {};

  // nit from existing
  inline void copy(Sprite& other) {
    m_cpose = other.m_cpose;
    m_canim = other.m_canim;
    m_src   = other.m_src;

  };

  // swap out current animation
  void set_anim(uint16_t idex);
  void set_anim(std::string tag);

  // offset for fetch methods
  inline void set_src(uint32_t idex) {
    m_src=idex;

  };

  // run anims
  uint32_t play(void);

  // get current pose
  inline uint32_t get_cpose(void) {
    return m_cpose;

  };

};

// ---   *   ---   *   ---

#endif // __5E_SPRITE_H__
