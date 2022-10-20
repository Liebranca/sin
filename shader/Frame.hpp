#ifndef __SHADER_FRAME_H__
#define __SHADER_FRAME_H__

// ---   *   ---   *   ---
// deps

  #include <vector>

  #include "bitter/kvrnel/Style.hpp"
  #include "shader/Constants.hpp"

  #include "shader/Program.hpp"

// ---   *   ---   *   ---
// info

class Programs {

public:

  VERSION   "v0.00.2b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---

private:

  // instance counter
  uint32_t  m_instances;

  // memory pool
  Program   m_slots[NUM_PROGRAMS];

  // index stack
  std::vector<uint32_t> m_stack;

  // current program
  Program*  m_current;

// ---   *   ---   *   ---

public:

  // construc/destroy
  Programs(void);
  ~Programs(void);

  // get new
  Program* nit(const shader::Params* params);

  // ^get existing
  inline Program* get(uint32_t idex) {
    return &m_slots[idex];

  };

  // ^let go
  void del(Program* p);

  // set current
  void use(Program* p);
  inline void use(uint32_t p) {
    this->use(this->get(p));

  };

};

// ---   *   ---   *   ---

#endif // __SHADER_FRAME_H__
