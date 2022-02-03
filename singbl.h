#ifndef __SINGBL_H__
#define __SINGBL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

// ---   *   ---   *   ---
// shader settings defaults

#ifndef NUM_ATTRS
#define NUM_ATTRS 8
#endif

#ifndef NUM_UNIFORMS
#define NUM_UNIFORMS 16
#endif

#ifndef NUM_UBOS
#define NUM_UBOS 4
#endif

#ifndef NUM_SSBOS
#define NUM_SSBOS 4
#endif

#ifndef NUM_SAMPLERS
#define NUM_SAMPLERS 4
#endif

#ifndef NUM_SHADERS
#define NUM_SHADERS 2
#endif

#ifndef NUM_PROGRAMS
#define NUM_PROGRAMS 16
#endif

// ---   *   ---   *   ---

#ifdef __cplusplus
};
#endif

#endif // __SINGBL_H__
