#ifndef __5E_FBO_H__
#define __5E_FBO_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/GLM.hpp"
  #include "shader/Texture.hpp"

// ---   *   ---   *   ---
// info

class FBO {

public:

  VERSION   "v0.00.1b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// helpers

// ---   *   ---   *   ---
// attrs

private:

  Texture  m_texture;

  uint32_t m_loc;
  uint64_t m_size;

// ---   *   ---   *   ---
// guts

  // check ice is OK
  bool validate(void);

  // nasty bit of texture setup boiler
  void nit_texture(void);

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  void nit(uint64_t size);

  // ^dstruc
  ~FBO(void);

  // use/dont use
  void bind(void);
  void unbind(void);

};

// ---   *   ---   *   ---

#endif // __5E_FBO_H__
