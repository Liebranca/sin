// ---   *   ---   *   ---
// SINGBL
// Global constants for sin
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,
// ---   *   ---   *   ---

// deps
  #include "singbl.h"

// ---   *   ---   *   ---
// shader settings defaults

#ifndef SIN_SHDP_ATTR_CNT
#define SIN_SHDP_ATTR_CNT 8
#endif

#ifndef SIN_SHDP_UNIFORM_CNT
#define SIN_SHDP_UNIFORM_CNT 16
#endif

#ifndef SIN_SHDP_UBO_CNT
#define SIN_SHDP_UBO_CNT 4
#endif

#ifndef SIN_SHDP_SSBO_CNT
#define SIN_SHDP_SSBO_CNT 4
#endif

#ifndef SIN_SHDP_SAMPLER_CNT
#define SIN_SHDP_SAMPLER_CNT 4
#endif

#ifndef SIN_SHDP_SHADER_CNT
#define SIN_SHDP_SHADER_CNT 2
#endif

#ifndef SIN_SHDP_PROGRAM_CNT
#define SIN_SHDP_PROGRAM_CNT 16
#endif

// ---   *   ---   *   ---
// define container

const SINGBL SIN={

  SIN_SHDP_ATTR_CNT,
  SIN_SHDP_UNIFORM_CNT,
  SIN_SHDP_UBO_CNT,
  SIN_SHDP_SSBO_CNT,
  SIN_SHDP_SAMPLER_CNT,

  SIN_SHDP_SHADER_CNT,
  SIN_SHDP_PROGRAM_CNT

};

// ---   *   ---   *   ---
