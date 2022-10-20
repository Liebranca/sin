#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

// ---   *   ---   *   ---
// deps

  #include <vector>

  #include "bitter/kvrnel/Style.hpp"
  #include "shader/Constants.hpp"
  #include "shader/Params.hpp"

// ---   *   ---   *   ---
// info

class Program {

public:

  VERSION   "v0.00.2b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// attrs

private:

  // index into mempool
  uint32_t m_idex;

  // program bind location
  uint32_t m_loc;

  const shader::Params* m_params;

  // shader data bind locations
  uint32_t m_shaders  [NUM_SHADERS];
  uint32_t m_attrs    [NUM_ATTRS];
  uint32_t m_uniforms [NUM_UNIFORMS];
  uint32_t m_ubos     [NUM_UBOS];
  uint32_t m_ssbos    [NUM_SSBOS];
  uint32_t m_samplers [NUM_SAMPLERS];

  // placeholder
  uint64_t m_flags;

// ---   *   ---   *   ---

private:

  int get_err(

    int         loc,
    int         flags,

    int         isProgram,

    const char* errme

  );

  int compile(

    uint32_t     idex,
    uint32_t     type,

    const char** sources,
    uint32_t     num_sources

  );

  int create(void);
  int link(void);
  int validate(void);

  inline void attach(void);

  inline void fix_attrs(void);
  inline void fix_uniforms(void);
  inline void fix_ubos(void);
  inline void fix_samplers(void);

// ---   *   ---   *   ---
// interface

public:

  // initialize
  int nit(
    uint32_t idex,
    const shader::Params* params

  );

  // ^destroy
  void del(void);

  // compiler trash
  Program(void) {;};
  ~Program(void) {this->del();};

// ---   *   ---   *   ---
// getters

  inline uint32_t get_idex(void) {
    return m_idex;

  };

  inline uint32_t get_loc(void) {
    return m_loc;

  };

};

// glUseProgram wrapper
void chkProgram(uint64_t idex);

// ---   *   ---   *   ---

#endif // __SHADER_PROGRAM_H__
