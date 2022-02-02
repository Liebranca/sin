#ifndef __SINGBL_H__
#define __SINGBL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

// ---   *   ---   *   ---
// global constants

// assorted shader-param bounds
typedef struct {

  const size_t attr_cnt;
  const size_t uniform_cnt;
  const size_t ubo_cnt;
  const size_t ssbo_cnt;
  const size_t sampler_cnt;

  const size_t shader_cnt;
  const size_t program_cnt;

} SINGBL_SHADER;

// ---   *   ---   *   ---

// effectively, a namespace/container for constants
typedef struct {

  const SINGBL_SHADER shdp;

} SINGBL;extern const SINGBL SIN;

// ---   *   ---   *   ---

#ifdef __cplusplus
};
#endif

#endif // __SINGBL_H__
