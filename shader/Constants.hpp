#ifndef __SHADER_CONSTANTS_H__
#define __SHADER_CONSTANTS_H__

// ---   *   ---   *   ---
// deps

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

#endif // __SHADER_CONSTANTS_H__
