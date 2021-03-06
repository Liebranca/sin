#ifndef __SHADER_PARAMS_H__
#define __SHADER_PARAMS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "singbl.h"
#include <stdint.h>

// ---   *   ---   *   ---
// shader creation helper

typedef struct SHADER_PARAMS {

  const char** source_v;
  const size_t source_v_sz;

  const char** source_f;
  const size_t source_f_sz;

  const char* attrs[NUM_ATTRS];
  const size_t num_attrs;

  const char* uniforms[NUM_UNIFORMS];
  const size_t num_uniforms;

  const char* ubos[NUM_UBOS];
  const size_t num_ubos;

  const char* ssbos[NUM_SSBOS];
  const size_t num_ssbos;

  const char* samplers[NUM_SAMPLERS];
  const size_t num_samplers;

  const uint64_t flags;

} SHDP;

// ---   *   ---   *   ---

#ifdef __cplusplus
};
#endif

#endif // __SHADER_PARAMS_H__
