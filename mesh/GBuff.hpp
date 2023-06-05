#ifndef __5E_GBUFF_H__
#define __5E_GBUFF_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/Style.hpp"

// ---   *   ---   *   ---
// info

class GBuff {

public:

  VERSION   "v0.00.1b";
  AUTHOR    "IBN-3DILA";

  // usage hints
  cx32 BUFF_USAGE = 0x00FF;
  cx32 DYNAMIC    = 0x0001;
  cx32 STATIC     = 0x0002;

  // buffer types
  cx32 BUFF_TYPE  = 0xFF00;
  cx32 ARRAY      = 0x0100;
  cx32 ELEMENT    = 0x0200;
  cx32 STORAGE    = 0x0400;
  cx32 UNIFORM    = 0x0800;

  // ^shorthands
  cx32 D_ARRAY    = DYNAMIC | ARRAY;
  cx32 S_ARRAY    = STATIC  | ARRAY;
  cx32 D_ELEMENT  = DYNAMIC | ELEMENT;
  cx32 S_ELEMENT  = STATIC  | ELEMENT;
  cx32 D_STORAGE  = DYNAMIC | STORAGE;
  cx32 S_STORAGE  = STATIC  | STORAGE;
  cx32 D_UNIFORM  = DYNAMIC | UNIFORM;
  cx32 S_UNIFORM  = STATIC  | UNIFORM;

// ---   *   ---   *   ---
// attrs

private:

  uint32_t m_loc=0;
  uint32_t m_type;

  uint64_t m_elem_cnt;
  uint64_t m_elem_sz;
  uint64_t m_size;

  uint32_t m_usage;
  bool     m_nitted=false;

// ---   *   ---   *   ---
// guts

  void gen(void);
  void expand_mode(uint32_t mode);

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  GBuff(

    uint32_t  mode,

    uint64_t  elem_sz,
    uint64_t  elem_cnt

  );

  void nit(

    uint32_t  mode,

    uint64_t  elem_sz,
    uint64_t  elem_cnt

  );

  // ^ctrash
  GBuff(void) {};
  ~GBuff(void);

  // use/dont use
  void bind(void);
  void unbind(void);

  // (re) alloc and write
  void set_data(void* data);

  // ^partial write, no alloc
  void sub_data(

    void*    data,

    uint64_t offset,
    uint64_t cnt

  );

};

// ---   *   ---   *   ---

#endif // __5E_GBUFF_H__
