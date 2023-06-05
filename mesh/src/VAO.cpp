// ---   *   ---   *   ---
// VAO
// More boilerwrap!
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
  #include "mesh/VAO.hpp"

// ---   *   ---   *   ---
// generate self

void VAO::gen(void) {
  glGenVertexArrays(1,&m_loc);
  m_nitted=true;

};

// ---   *   ---   *   ---
// nit descriptor for vattr Fs

void VAO::Attr_Desc::expand_mode(uint32_t mode) {

  // get data type
  uint32_t t = mode & (INT_ATTR | FLOAT_ATTR);
  uint32_t s = 0;
  bool     n = false;

  switch(t) {

    case NFLOAT:
      n=true;

    case FLOAT:
      t=GL_FLOAT;
      s=4;
      break;

    case U8:
      t=GL_UNSIGNED_BYTE;
      s=1;
      break;

    case U16:
      t=GL_UNSIGNED_SHORT;
      s=2;
      break;

    case U32:
    default:
      t=GL_UNSIGNED_INT;
      s=4;
      break;

  };

  // get attr size
  uint32_t c = mode & ATTR_SIZE;

  switch(c) {

    case SINGLE:
      c=1;
      break;

    case VEC2:
      c=2;
      break;

    case VEC3:
      c=3;
      break;

    case VEC4:
    default:
      c=4;
      break;

  };

  // ^asg
  this->elem_sz  = s;
  this->elem_cnt = c;
  this->gl_type  = t;
  this->norm     = n;

};

// ---   *   ---   *   ---
// int-type vertex attribute

void VAO::vattr_i(

  uint32_t mode,

  uint64_t stride,
  uint64_t offset

) {

  // get descriptor from flags
  Attr_Desc d;
  d.expand_mode(mode);

  // ^set values
  glVertexAttribIPointer(

    m_attr_ptr,
    d.elem_cnt,d.gl_type,

    stride,(void*) offset

  );

  // move to next attr
  this->adv_ptr(d);

};

// ---   *   ---   *   ---
// ^float

void VAO::vattr_f(

  uint32_t mode,

  uint64_t stride,
  uint64_t offset

) {

  // get descriptor from flags
  Attr_Desc d;
  d.expand_mode(mode);

  // ^set values
  glVertexAttribIPointer(

    m_attr_ptr,
    d.elem_cnt,d.gl_type,

    stride,(void*) offset

  );

  // move to next attr
  this->adv_ptr(d);

};

// ---   *   ---   *   ---
// increase idex accto attr size

void VAO::adv_ptr(
  VAO::Attr_Desc& d

) {

  uint32_t i=(
    d.elem_cnt
  * d.elem_sz

  ) / ALIGN;

  m_attr_ptr += i + (1 *! i);

};

// ---   *   ---   *   ---
// set vertex attr

void VAO::vattr(

  uint32_t mode,

  uint64_t stride,
  uint64_t offset

) {

  this->bind();
  m_gbuff[VBO].bind();

  glEnableVertexAttribArray(m_attr_ptr);

  if(type & INT_ATTR) {
    this->vattr_i(mode,stride,offset);

  } else {
    this->vattr_f(mode,stride,offset);

  };

  m_gbuff[VBO].unbind();
  this->unbind();

};

// ---   *   ---   *   ---
// use/dont use

void VAO::bind(void) {
  glBindVertexArray(m_loc);

};

void VAO::unbind(void) {
  glBindVertexArray(0);

};

// ---   *   ---   *   ---
// cstruc

VAO::VAO(

  uint32_t vbo_mode,
  uint32_t ibo_mode,

  uint64_t vbo_elem_sz,
  uint64_t vbo_elem_cnt,

  uint64_t ibo_elem_sz,
  uint64_t ibo_elem_cnt

) {

  this->nit(

    vbo_mode,
    ibo_mode,

    vbo_elem_sz,
    vbo_elem_cnt,

    ibo_elem_sz,
    ibo_elem_cnt

  );

};

void VAO::nit(

  uint32_t vbo_mode,
  uint32_t ibo_mode,

  uint64_t vbo_elem_sz,
  uint64_t vbo_elem_cnt,

  uint64_t ibo_elem_sz,
  uint64_t ibo_elem_cnt

) {

  this->gen();
  this->bind();

  m_gbuff[VBO].nit(
    vbo_mode,
    vbo_elem_sz,
    vbo_elem_cnt

  );

  m_gbuff[IBO].nit(
    ibo_mode,
    ibo_elem_sz,
    ibo_elem_cnt

  );

  this->unbind();

};

// ---   *   ---   *   ---
// ^dstruc

VAO::~VAO(void) {

  if(m_nitted) {
    glDeleteVertexArrays(1,&m_loc);

  };

};

// ---   *   ---   *   ---
