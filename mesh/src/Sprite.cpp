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

  #include "bitter/kvrnel/Dice.hpp"
  #include "mesh/Sprite.hpp"

// ---   *   ---   *   ---
// run current animation

uint32_t Sprite::play(void) {

  // advance pose
  uint32_t out=m_cpose;
  this->calc_time();

  // get ctx
  auto& meta  = fetch_meta(m_src);
  auto& anim  = meta.get(m_canim);

  // cap current frame
  if(m_cpose == anim.end) {
    m_cpose=anim.beg;

  };

  // ret mesh idex offset by pose
  return out+m_src;

};

// ---   *   ---   *   ---
// advances current pose each
// N ticks of animation time

void Sprite::calc_time(void) {

  m_ctime  += FTIME;
  bool adv  = m_ctime >= 1.0f;

  m_ctime  *= ! adv;
  m_cpose  += adv;

};

// ---   *   ---   *   ---

void Sprite::set_anim(uint16_t idex) {

  auto& meta = fetch_meta(m_src);
  auto& anim = meta.get(idex);

  m_canim    = idex;
  m_cpose    = anim.beg;

  auto r     = Dice::roll(1,8);
  m_ctime    = float(r) * FTIME;

};

void Sprite::set_anim(std::string tag) {

  auto& meta = fetch_meta(m_src);
  auto  idex = uint32_t(meta.iof(tag));
  auto& anim = meta.get(idex);

  m_canim    = idex;
  m_cpose    = anim.beg;

  auto r     = Dice::roll(1,8);
  m_ctime    = float(r) * FTIME;

};

// ---   *   ---   *   ---
