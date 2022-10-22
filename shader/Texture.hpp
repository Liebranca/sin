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

  VERSION   "v0.00.1b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  uint32_t m_loc  = 0;
  uint32_t m_slot = 0;

  uint64_t m_img_sz;

// ---   *   ---   *   ---
// iface

public:

  // compiler trash
  Texture(void) {};

  // make/destroy
  Texture(std::string fpath);
  ~Texture(void);

  // bind
  void use(void);

};

// ---   *   ---   *   ---

#endif // __5E_TEXTURE_H__