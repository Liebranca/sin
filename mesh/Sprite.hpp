#ifndef __5E_SPRITE_H__
#define __5E_SPRITE_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/Style.hpp"
  #include "sin/shader/Texture.hpp"

// ---   *   ---   *   ---
// info

class Sprite {

public:

  VERSION     "v0.00.3b";
  AUTHOR      "IBN-3DILA";

// ---   *   ---   *   ---
// helpers

  struct Bld {
    std::string crk_path;
    std::string ans_path;

  };

  struct Anim {

    uint16_t beg;
    uint16_t end;

    uint32_t flags;

//    nihil    chk;

  };

  typedef std::vector<Anim> Anims;
  typedef std::vector<uint32_t> Frames;

// ---   *   ---   *   ---
// attrs

private:

  uint32_t m_cframe = 0;
  Anim*    m_canim  = NULL;

  Frames   m_frames;
  Anims    m_anims;
  Texture  m_sheet;

// ---   *   ---   *   ---
// guts

// ---   *   ---   *   ---
// iface

public:

  // ctrash
  Sprite(void) {};
  ~Sprite(void) {};

  // actual
  Sprite(Frames& frames,Anims& anims) {
    m_frames = frames;
    m_anims  = anims;

  };

  inline void set_anim(uint16_t idex) {
    m_canim  = &m_anims[idex];
    m_cframe = m_canim->beg;

  };

  inline uint32_t operator[](uint64_t i) {
    i-=i*(i>=m_frames.size());
    return m_frames[i];

  };

// ---   *   ---   *   ---
// used by frame to construct

  inline void add_frame(uint32_t idex) {
    m_frames.push_back(idex);

  };

  inline void add_anim(Anim& anim) {
    m_anims.push_back(anim);

  };

  inline void set_sheet(Texture& tex) {
    m_sheet.from(tex);

  };

// ---   *   ---   *   ---

  // run current animation
  inline uint32_t play(void) {

    uint32_t out=m_cframe++;

    // cap current frame
    if(m_cframe >= m_canim->end) {
      m_cframe=m_canim->beg;

    };

    // run assoc fn
//    m_canim->chk();
    m_sheet.use();

    // ret mesh idex
    return out;

  };

};

// ---   *   ---   *   ---

#endif // __5E_SPRITE_H__
