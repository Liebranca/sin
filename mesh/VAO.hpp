#ifndef __5E_VAO_H__
#define __5E_VAO_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/Style.hpp"
  #include "mesh/GBuff.hpp"

// ---   *   ---   *   ---
// info

class VAO {

public:

  VERSION   'v0.00.1b';
  AUTHOR    'IBN-3DILA';

  enum {

    VBO,
    IBO,

    NUM_GBUFF

  };

  cx32 ALIGN = 0x10;

  // vertex attr types
  cx32 INT_ATTR  = 0x000000FF;
  cx32 U8        = 0x00000001;
  cx32 U16       = 0x00000002;
  cx32 U32       = 0x00000004;

  // ^floating variation
  cx32 FLT_ATTR  = 0x0000FF00;
  cx32 FLOAT     = 0x00000100;
  cx32 NFLOAT    = 0x00000200;

  // ^flags for size of attr
  cx32 ATTR_SIZE = 0x00FF0000;
  cx32 SINGLE    = 0x0001;
  cx32 VEC2      = 0x0002;
  cx32 VEC3      = 0x0004;
  cx32 VEC4      = 0x0008;

  // ^shorthands
  cx32 U32_4     = U32    | VEC4;
  cx32 R32_4     = FLOAT  | VEC4;
  cx32 N32_4     = NFLOAT | VEC4;

// ---   *   ---   *   ---
// helpers

private:

  struct Attr_Desc {

    uint32_t elem_sz;
    uint32_t elem_cnt;

    uint32_t gl_type;

    bool     norm;

    void expand_mode(uint32_t mode);

  };

// ---   *   ---   *   ---
// attrs

  GBuff    m_gbuff[NUM_GBUFF];

  uint32_t m_loc      = 0;
  uint32_t m_attr_ptr = 0;

  bool     m_nitted   = false;

// ---   *   ---   *   ---
// guts

  void gen(void);

  void vattr_i(

    uint32_t mode,

    uint64_t stride,
    uint64_t offset

  );

  void vattr_f(

    uint32_t mode,

    uint64_t stride,
    uint64_t offset

  );

  void adv_ptr(Attr_Desc& d);

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  void nit(

    uint32_t vbo_mode,
    uint32_t ibo_mode,

    uint64_t vbo_elem_sz,
    uint64_t vbo_elem_cnt,

    uint64_t ibo_elem_sz,
    uint64_t ibo_elem_cnt

  );

  VAO(

    uint32_t vbo_mode,
    uint32_t ibo_mode,

    uint64_t vbo_elem_sz,
    uint64_t vbo_elem_cnt,

    uint64_t ibo_elem_sz,
    uint64_t ibo_elem_cnt

  );

  // ctrash
  VAO(void) {};
  ~VAO(void);

  // use/dont use
  void bind(void);
  void unbind(void);

  // set vertex attrs
  void vattr(

    uint32_t mode,

    uint64_t stride,
    uint64_t offset

  );

  // get handle to buffer
  inline GBuff& gbuff(uint32_t idex) {
    return m_gbuff[idex];

  };

};

// ---   *   ---   *   ---

#endif // __5E_VAO_H__
