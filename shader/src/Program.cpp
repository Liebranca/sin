// ---   *   ---   *   ---
// PROGRAM
// Gives you shaders
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include <cstring>
  #include <cstdio>

  #include <GL/glew.h>

  #include "ce/ardef.h"
  #include "ce/arstd.h"

  #include "shader/Params.hpp"
  #include "shader/Program.hpp"

// ---   *   ---   *   ---
// variations on uploading data

void Program::set_uniform(
  uint32_t   idex,
  glm::mat4& data

) {

  glUniformMatrix4fv(
    this->get_uniform(idex),
    1,GL_FALSE,&data[0][0]

  );

};

void Program::set_uniform(
  uint32_t   idex,
  glm::mat3& data

) {

  glUniformMatrix3fv(
    this->get_uniform(idex),
    1,GL_FALSE,&data[0][0]

  );

};

void Program::set_uniform(
  uint32_t    idex,
  glm::uvec4& data

) {

  glUniform4uiv(
    this->get_uniform(idex),
    1,&data[0]

  );

};

void Program::set_uniform(
  uint32_t idex,
  uint32_t data

) {

  glUniform1uiv(
    this->get_uniform(idex),
    1,&data

  );

};

// ---   *   ---   *   ---
// errchk boiler

int Program::get_err(

  int         loc,
  int         flags,

  int         isProgram,

  const char* errme

) {

  int  out=AR_DONE;

  char error[0x100]={0};
  int  success;

  // honest to God dont know what this is
  if(isProgram) {
    glGetProgramiv(loc,flags,&success);

  } else {
    glGetShaderiv(loc,flags,&success);

  };

  // ^but when it fails, theres a problem
  if(!success) {

    if(isProgram) {
      glGetProgramInfoLog(
        loc,sizeof(error),
        NULL,error

      );

    } else {
      glGetShaderInfoLog(
        loc,sizeof(error),
        NULL,error

      );

    };

    // ^so fail if we have one
    fprintf(stderr,"%s:%s\n",errme,error);
    out=AR_FATAL;

  };

  return out;

};

// ---   *   ---   *   ---
// shader compile

int Program::compile(

  uint32_t     idex,
  uint32_t     type,

  const char** sources,
  uint32_t     num_sources

) {

  int       out    = AR_DONE;
  uint32_t* shader = &m_shaders[idex];

// ---   *   ---   *   ---
// make and errchk

  if(!(*shader=glCreateShader(type))) {

    fprintf(stderr,
      "Shader couldn't be created\n"

    );

    out=AR_FATAL;
    goto TAIL;

  };

// ---   *   ---   *   ---
// compile and errchk

  glShaderSource(
    *shader,
    num_sources,

    // dumbest cast ever conceived
    // that also means dont touch it
    (const char* const*) sources,

    NULL

  );

  glCompileShader(*shader);

  // get compilation error
  out=this->get_err(*shader,

    GL_COMPILE_STATUS,0,
    "Shader couldn't compile"

  );

// ---   *   ---   *   ---
// hope out is not FATAL

TAIL:
  return out;

};

// ---   *   ---   *   ---
// make shader from params

int Program::create(void) {

  int out = AR_DONE;
  m_loc   = glCreateProgram();

  int types[]={
    GL_VERTEX_SHADER,
    GL_FRAGMENT_SHADER

  };

  const char** sources[]={
    m_params->source_v,
    m_params->source_f

  };

  uint64_t sizes[]={
    m_params->source_v_sz,
    m_params->source_f_sz

  };

// ---   *   ---   *   ---
// iter and fail

  for(uint32_t i=0;i<2;i++) {

    out=this->compile(
      i,types[i],sources[i],sizes[i]

    );

    if(out==AR_FATAL) {break;};

  };

  return out;

};

// ---   *   ---   *   ---
// boiler methods

inline void Program::attach(void) {

  for(uint32_t i=0;i<2;i++) {
    glAttachShader(
      m_loc,
      m_shaders[i]

    );

  };

};

// ---   *   ---   *   ---
// ^boiler

inline void Program::fix_attrs(void) {

  for(

    uint32_t attr_loc=0;

    attr_loc < m_params->num_attrs;
    attr_loc++

  ) {

    glBindAttribLocation(
      m_loc,

      attr_loc,
      m_params->attrs[attr_loc]

    );

  };

};

// ---   *   ---   *   ---
// ^errchk'd boiler

int Program::link(void) {

  int out=AR_DONE;
  glLinkProgram(m_loc);

  // catch linking fail
  out=this->get_err(
    m_loc,

    GL_LINK_STATUS,
    1,"Shader linking failed"

  );

  return out;

};

// ---   *   ---   *   ---
// ^just boiler

inline void Program::fix_uniforms(void) {

  for(

    uint32_t uniform_loc=0;

    uniform_loc < m_params->num_uniforms;
    uniform_loc++

  ) {

    m_uniforms[uniform_loc]=glGetUniformLocation(
      m_loc,
      m_params->uniforms[uniform_loc]

    );

  };

};

// ---   *   ---   *   ---
// ^idem

inline void Program::fix_ubos(void) {

  for(

    uint32_t ubo_loc=0;

    ubo_loc < m_params->num_ubos;
    ubo_loc++

  ) {

    m_ubos[ubo_loc]=glGetUniformBlockIndex(
      m_loc,
      m_params->ubos[ubo_loc]

    );

    glUniformBlockBinding(
      m_loc,
      m_ubos[ubo_loc],
      ubo_loc

    );

  };

};

// ---   *   ---   *   ---
// ^shader storage buffers

inline void Program::fix_ssbos(void) {

  for(

    uint32_t ssbo_loc=0;

    ssbo_loc < m_params->num_ssbos;
    ssbo_loc++

  ) {

    m_ssbos[ssbo_loc]=glGetProgramResourceIndex(
      m_loc,

      GL_SHADER_STORAGE_BLOCK,
      m_params->ssbos[ssbo_loc]

    );

    glShaderStorageBlockBinding(
      m_loc,m_ssbos[ssbo_loc],ssbo_loc

    );

  };

};

// ---   *   ---   *   ---
// ^and so on...

inline void Program::fix_samplers(void) {

  for(

    uint32_t texslot=0;

    texslot<m_params->num_samplers;
    texslot++

  ) {

    m_samplers[texslot]=glGetUniformLocation(
      m_loc,
      m_params->samplers[texslot]

    );

    glUniform1i(
      m_samplers[texslot],
      texslot

    );

  };

};

// ---   *   ---   *   ---
// errchk'd boiler

int Program::validate(void) {

  int out=AR_DONE;
  glValidateProgram(m_loc);

  // catch fail
  out=this->get_err(
    m_loc,

    GL_VALIDATE_STATUS,
    1,"Shader validation failed"

  );

  return out;

};

// ---   *   ---   *   ---
// anti-cpp constructor

int Program::nit(
  uint32_t idex,
  const shader::Params* params

) {

  int out  = AR_DONE;

  m_idex   = idex;
  m_params = params;

  if(this->create()==AR_FATAL) {
    out=AR_FATAL;
    goto TAIL;

  };

  this->attach();
  this->fix_attrs();

// ---   *   ---   *   ---

  if(this->link()==AR_FATAL) {
    out=AR_FATAL;
    goto TAIL;

  };

  glUseProgram(m_loc);

  this->fix_uniforms();
  this->fix_ubos();
  this->fix_ssbos();
  this->fix_samplers();

  out=this->validate();

// ---   *   ---   *   ---
// hope out is not an error

TAIL:
  return out;

};

// ---   *   ---   *   ---
// destroy

void Program::del(void) {

  if(!m_loc) {return;};

  for(uint32_t i=0;i<2;i++) {

    glDetachShader(
      m_loc,
      m_shaders[i]

    );

    glDeleteShader(
      m_shaders[i]

    );

  };

  glDeleteProgram(m_loc);
  m_loc^=m_loc;

};

// ---   *   ---   *   ---
