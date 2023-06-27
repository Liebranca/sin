#ifndef __5E_MESH_H__
#define __5E_MESH_H__

// ---   *   ---   *   ---
// deps

  #include <iostream>

  #include <GL/glew.h>
  #include <glm/vec4.hpp>

  #include "bitter/kvrnel/Style.hpp"

// ---   *   ---   *   ---
// info

class Mesh {

public:

  VERSION   "v0.00.7b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  uint16_t m_vcount;
  uint16_t m_icount;
  uint16_t m_mode;

  uint16_t m_voffset;
  uint16_t m_ioffset;

  uint16_t m_icap;
  uint16_t m_vcap;

  uint64_t m_byte_offset;

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  Mesh(

    uint16_t vcount,
    uint16_t icount,

    uint16_t mode,

    uint16_t voffset,
    uint16_t ioffset

  ) {

    this->nit(vcount,icount,mode,voffset,ioffset);

  };

  void nit(

    uint16_t vcount,
    uint16_t icount,

    uint16_t mode,

    uint16_t voffset,
    uint16_t ioffset

  ) {

    m_vcount  = vcount;
    m_icount  = icount;

    m_mode    = mode;

    m_voffset = voffset;
    m_ioffset = ioffset;

    m_byte_offset = m_ioffset * sizeof(uint16_t);

  };

  // ctrash
  Mesh(void) {};

  inline void draw(void) {
    glDrawElementsBaseVertex(

      m_mode,
      m_icount,

      GL_UNSIGNED_SHORT,
      (void*) m_byte_offset,

      m_voffset

    );

  };

// ---   *   ---   *   ---
// mesh slot is big enough for data

  inline bool room_for(
    uint16_t vcount,
    uint16_t icount

  ) {

  return
     vcount <= m_vcap
  && icount <= m_icap
  ;

  };

// ---   *   ---   *   ---
// setters

  inline void set_caps(
    uint16_t vcap,
    uint16_t icap

  ) {

    m_vcap=vcap;
    m_icap=icap;

  };

// ---   *   ---   *   ---
// getters

  inline uint16_t get_vcount(void) {
    return m_vcount;

  };

  inline uint16_t get_icount(void) {
    return m_icount;

  };

  inline uint16_t get_voffset(void) {
    return m_voffset;

  };

  inline uint16_t get_ioffset(void) {
    return m_ioffset;

  };

};

// ---   *   ---   *   ---

#endif // __5E_MESH_H__
