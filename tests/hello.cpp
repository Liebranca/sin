// ---   *   ---   *   ---
// deps

  #include "chasm/window.h"

  #include "shader/src/Program.cpp"
  #include "shader/src/Frame.cpp"

  #include "font/lycon_sh.hpp"

// ---   *   ---   *   ---

void draw(void) {
  ;

};

int main(void) {

  winnt("CHASM",640,480,0);

  Programs p_frame;
  Program* p=p_frame.nit(&shader::font::lycon_sh);

//
//  int panic=20;
//  while(panic--) {
//    winrend(0,&draw);
//
//  };
//
//  windl();

  p_frame.del(p);

  return 0;

};

// ---   *   ---   *   ---
