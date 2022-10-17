// ---   *   ---   *   ---
// SHADER FRAME
// Program container
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include "shader/Frame.hpp"

// ---   *   ---   *   ---
// construc

Programs::Programs(void) {
  m_stack.reserve(NUM_PROGRAMS);

  for(uint32_t i=0;i<NUM_PROGRAMS;i++) {
    m_stack.push_back(i);

  };

};

// ---   *   ---   *   ---
// ^destroy

Programs::~Programs(void) {

  for(uint32_t i=0;i<NUM_PROGRAMS;i++) {
    m_slots[idex].del();

  };

};

// ---   *   ---   *   ---
// initialize new shader

Program* Programs::nit(
  const shader::Params* params

) {

  uint32_t idex=m_stack.pop_back();
  int state=m_slots[idex].nit(idex,params);

  if(state!=AR_DONE) {
    fprintf(stderr,"Terminated\n");
    exit(1);

  };

  return &m_slots[idex];

};

// ---   *   ---   *   ---
// ^free

void Programs::del(Program* p) {

  uint32_t idex=p->get_idex();
  m_slots[idex].del();

  m_stack.push_back(idex);

};

// ---   *   ---   *   ---
// set current program

void use(Program* p) {

  if(p!=m_current) {

    m_current=p;
    glUseProgram(p->get_loc());

  };

};

// ---   *   ---   *   ---
