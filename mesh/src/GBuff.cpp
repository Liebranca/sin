// ---   *   ---   *   ---
// GBUFF
// Wrap around the boiler
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include <GL/glew.h>
  #include "mesh/GBuff.hpp"

// ---   *   ---   *   ---
// generate self

void GBuff::gen(void) {
  glGenBuffers(1,&m_loc);
  m_nitted=true;

};

// ---   *   ---   *   ---
// determine type and usage
// from single value

void GBuff::expand_mode(uint32_t mode) {

  // usage
  uint32_t u = mode & BUFF_USAGE;

  switch(u) {

  case STATIC:
    u=GL_STATIC_DRAW;
    break;

  case DYNAMIC:
  default:
    u=GL_DYNAMIC_DRAW;
    break;

  };

  // ^type
  uint32_t t = mode & BUFF_TYPE;

  switch(t) {

  case UNIFORM:
    t=GL_UNIFORM_BUFFER;
    break;

  case STORAGE:
    t=GL_SHADER_STORAGE_BUFFER;
    break;

  case ELEMENT:
    t=GL_ELEMENT_ARRAY_BUFFER;
    break;

  case ARRAY:
  default:
    t=GL_ARRAY_BUFFER;
    break;

  };

  // ^asg
  m_usage = u;
  m_type  = t;

};

// ---   *   ---   *   ---
// use/dont use

void GBuff::bind(void) {
  glBindBuffer(m_type,m_loc);

};

void GBuff::unbind(void) {
  glBindBuffer(m_type,0);

};

// ---   *   ---   *   ---
// (re) alloc

void GBuff::set_data(void* data) {

  this->bind();
  glBufferData(m_type,m_size,data,m_usage);

};

// ---   *   ---   *   ---
// ^partial write, no alloc

void GBuff::sub_data(

  void*    data,

  uint64_t offset,
  uint64_t cnt

) {

  offset *= m_elem_sz;
  cnt    *= m_elem_sz;

  this->bind();
  glBufferSubData(m_type,offset,cnt,data);

};

// ---   *   ---   *   ---
// cstruc

GBuff::GBuff(

  uint32_t  mode,

  uint64_t  elem_sz,
  uint64_t  elem_cnt

) {

  this->nit(mode,elem_sz,elem_cnt);

};

void GBuff::nit(

  uint32_t  mode,

  uint64_t  elem_sz,
  uint64_t  elem_cnt

) {

  this->gen();

  m_elem_sz  = elem_sz;
  m_elem_cnt = elem_cnt;
  m_size     = m_elem_cnt * m_elem_sz;

  this->expand_mode(mode);
  this->set_data(NULL);

};

// ---   *   ---   *   ---
// ^dstruc

GBuff::~GBuff(void) {

  if(m_nitted) {
    glDeleteBuffers(1,&m_loc);

  };

};

// ---   *   ---   *   ---
