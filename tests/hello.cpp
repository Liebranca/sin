// ---   *   ---   *   ---
// deps

  #include <glm/mat4x4.hpp>


  #include "chasm/window.h"

  #include "shader/Texture.hpp"
  #include "shader/Frame.hpp"

  #include "mesh/Camera.hpp"

  #include "mesh/Frame.hpp"
  #include "mesh/Solid.hpp"

// ---   *   ---   *   ---

static void* BUCK;

// ---   *   ---   *   ---

void draw(void) {
  ((Meshes*) BUCK)->draw(0);
//  ((Meshes*) BUCK)->draw(1);

};

// ---   *   ---   *   ---

int main(void) {

  winnt("CHASM",640,480,0);

  Programs p_frame;
  Program* p=p_frame.nit(&shader::mesh::Solid);

  p_frame.use(p);

  Texture tex("../bitter/out");
  tex.use();

  Meshes m_frame;
  BUCK=&m_frame;

  m_frame.use();
  m_frame.make_sprite(tex,0);

// ---   *   ---   *   ---

  T3D transform;

  Camera::Lens lens={
    .width  = 640,
    .height = 480,

    .scale  = 0.0040f,
    .near   = 0.001f,
    .far    = 100.0f

  };

  Camera cam({0,0,8},lens);
  cam.use_ortho();

  glm::vec3 vel({0.1,0.0,0.0});

// ---   *   ---   *   ---

  int panic=20;
  while(panic--) {

    glUniformMatrix4fv(
      p->get_uniform(0),

      1,GL_FALSE,

      &transform.get_model()[0][0]

    );

    glUniformMatrix4fv(
      p->get_uniform(1),

      1,GL_FALSE,

      &cam.get_view()[0][0]

    );

    winrend(0,&draw);

  };

// ---   *   ---   *   ---
// cleanup

  windl();
  return 0;

};

// ---   *   ---   *   ---
