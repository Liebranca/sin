#ifndef __SHADER_PARAMS_H__
#define __SHADER_PARAMS_H__

// ---   *   ---   *   ---
// deps

  #include "shader/Constants.h"
  #include <stdint.h>

// ---   *   ---   *   ---
// shader creation helper

namespace shader {

  typedef struct {

    const char**    source_v;
    const uint64_t  source_v_sz;

    const char**    source_f;
    const uint64_t  source_f_sz;

    const char*     attrs         [NUM_ATTRS];
    const uint64_t  num_attrs;

    const char*     uniforms      [NUM_UNIFORMS];
    const uint64_t  num_uniforms;

    const char*     ubos          [NUM_UBOS];
    const uint64_t  num_ubos;

    const char*     ssbos         [NUM_SSBOS];
    const uint64_t  num_ssbos;

    const char*     samplers      [NUM_SAMPLERS];
    const uint64_t  num_samplers;

    const uint64_t  flags;

  } Params;

};

// ---   *   ---   *   ---

#endif // __SHADER_PARAMS_H__
