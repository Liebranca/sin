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

  const char* attrs[SIN.shdp.attr_cnt];
  const size_t num_attrs;

  const char* uniforms[SIN.shdp.uniform_cnt];
  const size_t num_uniforms;

  const char* ubos[SIN.shdp.ubo_cnt];
  const size_t num_ubos;

  const char* ssbos[SIN.shdp.ssbo_cnt];
  const size_t num_ssbos;

  const char* samplers[SIN.shdp.sampler_cnt];
  const size_t num_samplers;

  const uint64_t flags;

} SHDP;

// ---   *   ---   *   ---

#ifdef __cplusplus
};
#endif

#endif // __SHADER_PARAMS_H__
