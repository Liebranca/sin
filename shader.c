// ---   *   ---   *   ---
// SHADER
// Shader program interface
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,
// ---   *   ---   *   ---

// deps
  #include "shader.h"

  #include "stack.h"
  #include "shdp.h"

  #include "ce/ardef.h"
  #include "ce/arstd.h"

  #include <string.h>
  #include <stdio.h>

  #include <GL/glew.h>

// ---   *   ---   *   ---
// program descriptor

typedef struct {

  // index into mempool
  int idex;

  // program bind location
  int loc;

  // shader data bind locations
  int shaders[NUM_SHADERS];
  int attrs[NUM_ATTRS];
  int uniforms[NUM_UNIFORMS];
  int ubos[NUM_UBOS];
  int ssbos[NUM_SSBOS];
  int samplers[NUM_SAMPLERS];

  // placeholder!
  long flags;

} PRGM;

// ---   *   ---   *   ---
// instance container

typedef struct {

  // instance counter
  int instances;

  // memory pool
  PRGM slots[NUM_PROGRAMS];

  // index stack
  stk* stack;

  // current program
  PRGM* program;
  int program_idex;
  int passed_idex;

} SHMNG;static SHMNG mang={0};

// ---   *   ---   *   ---
// fwdecls

  void shddl(int idex);

// ---   *   ---   *   ---
// nit and del

void shdmnt(void) {

  mang.stack=stknt(NUM_PROGRAMS);
  for(int x=NUM_PROGRAMS-1;x>-1;x--) {
    stkpush(mang.stack,x);

  };

};void shdmdl(void) {

  // free programs
  for(int i=0;i<NUM_PROGRAMS;i++) {
    if(mang.slots[i].loc) {shddl(i);};

  // free mang mem
  };sfree((void**) &(mang.stack));

};

// ---   *   ---   *   ---
// errchk boiler

int shderr(
  int loc,
  int flags,

  int isProgram,
  const char* errme

) {

  char error[0x100]={0};
  int success;

  // honest to god dont know what this is
  if(isProgram) {
    glGetProgramiv(loc,flags,&success);

  } else {
    glGetShaderiv(loc,flags,&success);

// ---   *   ---   *   ---

  // ... but when it fails, theres a problem
  };if(!success) {
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

// ---   *   ---   *   ---

    // so fail if we have one
    };fprintf(stderr,"%s:%s\n",errme,error);
      return -1;

  };return 0;
};

// ---   *   ---   *   ---
// shader compile

int shdcmp(
  int* shader,
  int shader_type,

  const char** sources,
  int num_sources

) {

  // duh errchk
  if(!(*shader=glCreateShader(shader_type))) {
    fprintf(stderr,
      "Shader couldn't be created\n"

    );return -1;
  };

// ---   *   ---   *   ---
// compile (100% boiler, dont think about it)

  glShaderSource(
    *shader,
    num_sources,

    // dumbest cast ever conceived
    // that also means dont touch it
    (const char* const*) sources,

    NULL

  );glCompileShader(*shader);

  // more errchk
  if(shderr(
    *shader,
    GL_COMPILE_STATUS,
    0,"Shader couldn't compile"

  // on fail
  )) {return -1;};

  // success
  return 0;

};

// ---   *   ---   *   ---
// program constructor

int shdnt(const SHDP* params) {

  // get next free slot
  int idex=stkpop(mang.stack);
  PRGM program=mang.slots[idex];

  program.loc=glCreateProgram();

// ---   *   ---   *   ---
// compile boiler

  { int types[]={
      GL_VERTEX_SHADER,
      GL_FRAGMENT_SHADER

    };

    const char** sources[]={
      params->source_v,
      params->source_f

    };

    size_t sizes[]={
      params->source_v_sz,
      params->source_f_sz

    };

// ---   *   ---   *   ---
// iter and fail

    for(int i=0;i<2;i++) {
      if(shdcmp(

        program.shaders+i,types[i],
        sources[i],sizes[i]

      )<0) {return -1;};

    };

  };

// ---   *   ---   *   ---
// attach (also boiler)

  for(int i=0;i<2;i++) {
    glAttachShader(
      program.loc,
      program.shaders[i]

    );

  };

// ---   *   ---   *   ---
// fix attribute locations

  for(
    int attr_loc=0;
    attr_loc<params->num_attrs;
    attr_loc++

  ) {
    glBindAttribLocation(
      program.loc,
      attr_loc,
      params->attrs[attr_loc]

    );
  };

// ---   *   ---   *   ---
// link

  glLinkProgram(program.loc);

  // errchk
  if(shderr(
    program.loc,
    GL_LINK_STATUS,
    1,"Shader linking failed"

  )<0) {return -1;};

// ---   *   ---   *   ---

  glUseProgram(program.loc);

  // fix uniform locations
  for(
    int uniform_loc=0;
    uniform_loc<params->num_uniforms;
    uniform_loc++

  ) {

    program.uniforms[uniform_loc]=(
      glGetUniformLocation(
        program.loc,
        params->uniforms[uniform_loc]

      )
    );
  };

// ---   *   ---   *   ---

  // fix ubo locations
  for(
    int ubo_loc=0;
    ubo_loc<params->num_ubos;
    ubo_loc++

  ) {

    program.ubos[ubo_loc]=(
      glGetUniformBlockIndex(
        program.loc,
        params->ubos[ubo_loc]

      )

    );glUniformBlockBinding(
      program.loc,
      program.ubos[ubo_loc],
      ubo_loc

    );
  };

// ---   *   ---   *   ---

  // fix samplers
  for(
    int texslot=0;
    texslot<params->num_samplers;
    texslot++

  ) {

    program.samplers[texslot]=(
      glGetUniformLocation(
        program.loc,
        params->samplers[texslot]

      )

    );glUniform1i(
      program.samplers[texslot],
      texslot

    );
  };

// ---   *   ---   *   ---
// validate

  glValidateProgram(program.loc);

  // errchk
  if(shderr(
    program.loc,
    GL_VALIDATE_STATUS,
    1,"Shader validation failed"

  )<0) {return -1;};

// ---   *   ---   *   ---
// success

  mang.instances++;
  return idex;

};

// ---   *   ---   *   ---
// shader destructor

void shddl(int idex) {

  PRGM program=mang.slots[idex];

  // free program
  for(int i=0;i<2;i++) {

    glDetachShader(
      program.loc,
      program.shaders[i]

    );glDeleteShader(
      program.shaders[i]

    );

  };glDeleteProgram(program.loc);

  // free slot
  stkpush(mang.stack,idex);
  memset(&program,0,sizeof(PRGM));

  mang.instances--;

};

// ---   *   ---   *   ---
// binding

void useProgram(void) {
  mang.program=mang.slots+mang.passed_idex;
  mang.program_idex=mang.passed_idex;
  mang.passed_idex^=mang.passed_idex;

  glUseProgram(mang.program->loc);

};

static const nihil bind_tab[]={
  nope,
  useProgram

};

// ---   *   ---   *   ---
// bind check

void chkProgram(size_t idex) {
  mang.passed_idex=idex;
  bind_tab[ idex != mang.program_idex ]();

};

// ---   *   ---   *   ---
