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

  VERSION   "v0.00.3b";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---

private:

  // memory pool
  std::vector<Program> m_slots;

  // current program
  Program*  m_current;

// ---   *   ---   *   ---

public:

  // construc/destroy
  Programs(void);
  ~Programs(void);

  // get new
  uint32_t nit(const shader::Params* params);

  // ^get existing
  inline Program* get(uint32_t idex) {
    return &m_slots[idex];

  };

  // ^let go
  void del(Program* p);
  inline void del(uint32_t p) {
    this->del(this->get(p));

  };

  // set current
  void bind(Program* p);
  inline void bind(uint32_t p) {
    this->bind(this->get(p));

  };

};

// ---   *   ---   *   ---

#endif // __SHADER_FRAME_H__
