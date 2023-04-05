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

  VERSION     "v0.00.7b";
  AUTHOR      "IBN-3DILA";

  typedef std::vector<uint32_t> Poses;
  typedef std::vector<Poses>    Anims;

// ---   *   ---   *   ---
// defd by SIN

  static ANS&     fetch_meta(uint32_t idex);
  static Texture& fetch_sheet(uint32_t idex);
  static Poses&   fetch_poses(uint32_t idex);

// ---   *   ---   *   ---
// attrs

private:

  uint32_t m_cpose = 0;
  uint32_t m_canim = 0;

  uint32_t m_src   = 0;

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

  inline void set_anim(uint16_t idex) {

    auto& meta = fetch_meta(m_src);
    auto& anim = meta.get(idex);

    m_canim    = idex;
    m_cpose    = anim.beg;

  };

  inline void set_anim(std::string tag) {

    auto& meta = fetch_meta(m_src);
    auto  idex = uint32_t(meta.iof(tag));
    auto& anim = meta.get(idex);

    m_canim    = idex;
    m_cpose    = anim.beg;

  };

  // offset for fetch methods
  inline void set_src(uint32_t idex) {
    m_src=idex;

  };

// ---   *   ---   *   ---
// run anims

  uint32_t play(void);

};

// ---   *   ---   *   ---

#endif // __5E_SPRITE_H__
