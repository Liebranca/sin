// ---   *   ---   *   ---
// deps

  #include <glm/mat4x4.hpp>


  #include "chasm/Window.hpp"

  #include "shader/Texture.hpp"
  #include "shader/Frame.hpp"

  #include "mesh/Camera.hpp"

  #include "mesh/Frame.hpp"
  #include "mesh/Solid.hpp"

// ---   *   ---   *   ---

int main(void) {

  Win::Desc win_desc={
    .title="HELLO.CPP",
    .width=640,
    .height=480,

    .fullscreen=false,
    .fps=4

  };

  Win win(win_desc);

  Programs p_frame;
  Program* p=p_frame.nit(&shader::mesh::Solid);

  p_frame.use(p);

  Texture tex("../bitter/out");
  tex.use();

  Meshes m_frame;

  m_frame.use();
  Sprite dummy(m_frame.make_sprite(tex,0));

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

    win.refresh(0);
    m_frame.draw(dummy.play());

  };

  return 0;

};

// ---   *   ---   *   ---
