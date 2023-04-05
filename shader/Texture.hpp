#ifndef __5E_TEXTURE_H__
#define __5E_TEXTURE_H__

// ---   *   ---   *   ---
// deps

  #include <string>
  #include "bitter/kvrnel/Style.hpp"

// ---   *   ---   *   ---
// info

class Texture {

public:

  VERSION   "v0.00.3b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  uint32_t m_loc  = 0;
  uint32_t m_slot = 0;

  uint64_t m_img_sz;
  std::vector<uint64_t> m_tab;

// ---   *   ---   *   ---
// iface

public:

  // ctrash
  Texture(void) {};

  // cstruc
  void nit(std::string fpath);

  // dstruc
  ~Texture(void);

  // bind
  void use(void);

  // transfer ownership
  inline void from(Texture& other) {

    m_loc       = other.m_loc;
    m_slot      = other.m_slot;
    m_img_sz    = other.m_img_sz;
    m_tab       = other.m_tab;

    other.m_loc = 0;

  };

};

// ---   *   ---   *   ---

#endif // __5E_TEXTURE_H__
