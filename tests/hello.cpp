// ---   *   ---   *   ---
// deps

  #include <glm/mat4x4.hpp>

  #include "chasm/Chasm.hpp"
  #include "shader/Frame.hpp"

  #include "mesh/Camera.hpp"

  #include "mesh/Frame.hpp"
  #include "mesh/Solid.hpp_sg"

// ---   *   ---   *   ---
// GBL

  Camera* EYE=NULL;

// ---   *   ---   *   ---
// helpers

  struct Draw_Cmd {

    Meshes*  frame;

    Program* program;
    Sprite*  sprite;

    T3D*     xform;

  };

  typedef std::vector<Draw_Cmd> Draw_Batch;

// ---   *   ---   *   ---
// chasm signal for draw step

int draw(void* data) {

  static uint32_t i=0;
  Draw_Batch* bat=(Draw_Batch*) data;

  for(auto& o : *bat) {

    auto frame   = o.frame;

    auto program = o.program;
    auto sprite  = o.sprite;
    auto xform   = o.xform;

    auto mat0    = xform->get_model();
    auto mat1    = EYE->get_view();

    program->set_uniform(0,mat0);
    program->set_uniform(1,mat1);
    program->set_uniform(2,EYE->get_proj());

    frame->draw(sprite->play());

  };

  if(i==6) {
    if(EYE->is_ortho()) {
      EYE->use_persp();

    } else {
      EYE->use_ortho();

    };

    i=0;

  } else {
    i++;

  };

  return 1;

};

// ---   *   ---   *   ---

int main(void) {

  Win::Desc win_desc={
    .title="HELLO.CPP",
    .width=640,
    .height=480,

    .fullscreen=false,
    .fps=8

  };

  Chasm.nit(win_desc);

  Chasm.draw=&draw;
  Chasm.win.set_ambient_color(6);

  Programs p_frame;
  Program* p=p_frame.nit(&shader::mesh::Solid);

  p_frame.use(p);

  Meshes m_frame;
  m_frame.use();

  std::string path=
    "/home/lyeb/Desktop/spritebake/sheet.daf";

  Sprite dummy=m_frame.make_sprite(path);

// ---   *   ---   *   ---

  T3D xform;

  Camera::Lens lens={
    .width  = 640,
    .height = 480,

    .scale  = 0.01f,
    .fov    = 45.0f,

    .near   = 0.1f,
    .far    = 100.0f

  };

  Camera cam({0,0,8},lens);
  cam.use_persp();

  EYE=&cam;

  glm::vec3 vel({0.1,0.0,0.0});

  Draw_Cmd ffs={

    .frame   = &m_frame,
    .program = p,

    .sprite  = &dummy,
    .xform   = &xform

  };

  Draw_Batch draw_buff {ffs};

  CHASM_RUN((void*) &draw_buff,NULL);

  return 0;

};

// ---   *   ---   *   ---
