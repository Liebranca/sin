#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

// ---   *   ---   *   ---
// deps

  #include <vector>
  #include <glm/mat4x4.hpp>

  #include "bitter/kvrnel/Style.hpp"
  #include "shader/Constants.hpp"
  #include "shader/Params.hpp"

// ---   *   ---   *   ---
// info

class Program {

public:

  VERSION   "v0.00.6b";
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
  inline void fix_ssbos(void);
  inline void fix_samplers(void);

// ---   *   ---   *   ---
// interface

public:

  // cstruc
  int nit(
    uint32_t idex,
    const shader::Params* params

  );

  // ^dstruc
  void del(void);

  // ctrash
  Program(void) {;};
  ~Program(void) {this->del();};

// ---   *   ---   *   ---
// variations on uploading data

  void set_uniform(
    uint32_t   idex,
    glm::mat4& data

  );

  void set_uniform(
    uint32_t   idex,
    glm::mat3& data

  );

  void set_uniform(
    uint32_t idex,
    uint32_t data

  );

// ---   *   ---   *   ---
// getters

  inline uint32_t get_idex(void) {
    return m_idex;

  };

  inline uint32_t get_loc(void) {
    return m_loc;

  };

  inline uint32_t get_uniform(uint32_t idex) {
    return m_uniforms[idex];

  };

  inline uint32_t get_ubo(uint32_t idex) {
    return m_ubos[idex];

  };

};

// glUseProgram wrapper
void chkProgram(uint64_t idex);

// ---   *   ---   *   ---

#endif // __SHADER_PROGRAM_H__
