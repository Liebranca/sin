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

  VERSION     "v0.00.5b";
  AUTHOR      "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  typedef std::vector<uint32_t> Frames;

  uint32_t   m_cframe = 0;
  ANS::Anim* m_canim  = NULL;

  Frames     m_frames;
  Texture    m_sheet;

  ANS        m_data;

// ---   *   ---   *   ---
// guts

// ---   *   ---   *   ---
// iface

public:

  // ctrash
  Sprite(void) {};
  ~Sprite(void) {};

  inline void set_anim(uint16_t idex) {
    m_canim  = m_data.get(idex);
    m_cframe = m_canim->beg;

  };

  inline void set_anim(std::string tag) {
    m_canim  = m_data.get(tag);
    m_cframe = m_canim->beg;

  };

  inline uint32_t operator[](uint64_t i) {
    i-=i*(i>=m_frames.size());
    return m_frames[i];

  };

// ---   *   ---   *   ---
// used by frame to cstruc

  inline void add_frame(uint32_t idex) {
    m_frames.push_back(idex);

  };

  inline void set_sheet(Texture& tex) {
    m_sheet.from(tex);

  };

  inline void set_meta(std::string path) {
    m_data=ANS(path);

  };

// ---   *   ---   *   ---
// run anims

  uint32_t play(void);

};

// ---   *   ---   *   ---

#endif // __5E_SPRITE_H__
