#ifndef __5E_TEXTURE_H__
#define __5E_TEXTURE_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/Style.hpp"
  #include "bitter/ff/JOJ.hpp"

// ---   *   ---   *   ---
// info

class Texture {

public:

  VERSION   "v0.00.4b";
  AUTHOR    "IBN-3DILA";

  cx8 MAX_MIP         = 1;
  cx8 MAX_DEPTH       = 64;

  cx8 MATERIAL_DEPTH  = 3;
  cx8 AUTO_DEPTH      = 64;
  cx8 OVERWRITE       = 65;

  cx8 ATLAS           = 0x00;
  cx8 LAYERS          = 0x01;

// ---   *   ---   *   ---
// helpers

private:

  struct Range {
    uint32_t beg;
    uint32_t end;

  };

  typedef std::vector<Range> Ranges;

// ---   *   ---   *   ---
// attrs

  uint32_t m_loc  = 0;
  uint32_t m_slot = 0;

  uint64_t m_img_sz;
  uint32_t m_top  = 0;

  Ranges   m_rmark;
  Ranges   m_umark;
  Ranges   m_materials;

// ---   *   ---   *   ---
// iface

public:

  // ctrash
  Texture(void) {};

  // cstruc
  void nit(
    uint64_t sz,
    uint64_t slot = 0

  );

  // dstruc
  ~Texture(void);

  // bind
  void use(void);

//  // transfer ownership
//  inline void from(Texture& other) {
//
//    m_loc       = other.m_loc;
//    m_slot      = other.m_slot;
//    m_img_sz    = other.m_img_sz;
//
//    other.m_loc = 0;
//
//  };

  // puts data into buff
  void upload(

    JOJ&     joj,

    uint32_t start,
    uint8_t  mode=LAYERS

  );

  // marks section for reuse
  inline void remove(uint32_t idex) {

    auto& r=m_umark[idex];

    m_rmark.push_back(r);
    m_umark.erase(m_umark.begin()+idex);

  };

  // get N layers will fit
  // without overwrite
  uint32_t can_push(uint32_t n);

  // absolute idex of material
  // for a given entry
  uint32_t matid(uint32_t idex) {
    return m_materials[idex].beg;

  };

  // ^force push an entry
  void new_matid(uint32_t idex) {

    Range r={
      .beg=idex,
      .end=idex+3,

    };

    m_materials.push_back(r);

  };

};

// ---   *   ---   *   ---

#endif // __5E_TEXTURE_H__
