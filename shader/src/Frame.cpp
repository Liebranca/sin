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

  #include <GL/glew.h>
  #include "shader/Frame.hpp"

// ---   *   ---   *   ---
// cstruc

Programs::Programs(void) {
  m_slots.reserve(NUM_PROGRAMS);

};

// ---   *   ---   *   ---
// ^dstruc

Programs::~Programs(void) {

  for(uint32_t i=0;i<NUM_PROGRAMS;i++) {
    m_slots[i].del();

  };

};

// ---   *   ---   *   ---
// initialize new shader

uint32_t Programs::nit(
  const shader::Params* params

) {

  uint32_t idex=m_slots.size();
  m_slots.push_back(Program());

  int state=m_slots[idex].nit(idex,params);

  if(state != AR_DONE) {
    fprintf(stderr,"Terminated\n");
    exit(1);

  };

  return idex;

};

// ---   *   ---   *   ---
// ^free

void Programs::del(Program* p) {

  uint32_t idex=p->get_idex();
  m_slots[idex].del();

  m_slots.erase(m_slots.begin()+idex);

};

// ---   *   ---   *   ---
// set current program

void Programs::bind(Program* p) {

  if(p != m_current) {
    m_current=p;
    glUseProgram(p->get_loc());

  };

};

// ---   *   ---   *   ---
