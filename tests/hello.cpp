// ---   *   ---   *   ---
// deps

  #include <pthread.h>
  #include <glm/mat4x4.hpp>

  #include "bitter/kvrnel/Dice.hpp"

  #include "gaoler/Box.hpp"
  #include "chasm/Chasm.hpp"
  #include "Sin.hpp"

  #include "mesh/JOJ_Sprite.hpp_sg"
  #include "mesh/M3D.hpp_sg"

// ---   *   ---   *   ---
// helpers

  typedef std::vector<uint32_t> Nodes;

// ---   *   ---   *   ---
// GBL

  static int SCR_X=640;
  static int SCR_Y=480;

// ---   *   ---   *   ---
// lame fwd decls

  int draw(void* data);
  int logic(void* data);

// ---   *   ---   *   ---
// chasm stuff

void spawn_window(void) {

  auto& Chasm=CHASM::ice();

  Win::Desc win_desc={

    .title      = "HELLO.CPP",

    .width      = SCR_X,
    .height     = SCR_Y,

    .fullscreen = false,
    .fps        = 30

  };

  Chasm.nit(win_desc);
  Chasm.win.set_ambient_color(6);

  Chasm.draw  = &draw;
  Chasm.logic = &logic;

};

// ---   *   ---   *   ---
// selfex

void camera_settings(
  uint32_t bind_idex,
  bool     ortho=true

) {

  auto& Sin=SIN::ice();

  Camera::Lens lens={
    .width  = float(SCR_X),
    .height = float(SCR_Y),

    .scale  = 0.0055f,
    .fov    = 45.0f,

    .near   = 0.1f,
    .far    = 100.0f

  };

  Sin.nit_camera({0,0,10},lens,bind_idex,ortho);

};

// ---   *   ---   *   ---
// ^idem

void load_shaders(void) {

  auto& Sin=SIN::ice();

  shader::List programs {
    &shader::mesh::JOJ_Sprite,
    &shader::mesh::M3D,

  };

  Sin.nit_programs(programs);

};

// ---   *   ---   *   ---
// ^...

void load_resources(void) {

  auto& Sin=SIN::ice();

  std::string path=
    "/home/lyeb/Desktop/spritebake/sheet.daf";

  Sin.new_batch(SIN::PROGRAM0);
  Sin.batch->new_sprite(path);

  Sin.new_node(0,Node::SPRITE);

  Sin.new_batch(SIN::PROGRAM1);

  path="/home/lyeb/Desktop/crkbake/Cube.crk";
  Sin.batch->load_statics(path);

};

// ---   *   ---   *   ---
// chasm signal for draw step

int draw(void* data) {

  auto& Sin = SIN::ice();
  auto  bat = (Nodes*) data;

  // trigger camera update
  Sin.cam.get_view();

  // walk objects
  for(auto& idex : *bat) {
    Sin.nodes[idex].draw();

  };

  Sin.draw_enqueued();

  return 0;

};

// ---   *   ---   *   ---
// ^selfex

int logic(void* data) {

  static vec3 linvel(0.2f,0,0);
  static quat angvel(1,
    glm::radians(1.25f),
    glm::radians(0.75f),

    0

  );

  auto& Sin = SIN::ice();
  auto& nd1 = Sin.nodes[1];

  auto& pos = nd1.xform().position();

  if(nd1.is_still()) {
//    nd1.set_linvel(linvel);
    nd1.set_angvel(angvel);

  } else if(pos.x > 10.0f) {
    pos.x=-10.0f;

  };

  nd1.set_visible(
    Sin.cam.bound_in_frustum(nd1.bound())

  );

  nd1.fmotion();

  return 1;

};

// ---   *   ---   *   ---

int main(void) {

  spawn_window();

  load_shaders();
  load_resources();

  auto& Chasm = CHASM::ice();
  auto& Sin   = SIN::ice();

  camera_settings(Sin.program->get_ubo(0),false);

  Nodes draw_buff;

  draw_buff.push_back(
    Sin.new_node(0,Node::STATIC)

  );

  CHASM_RUN((void*) &draw_buff,NULL);

  return 0;

};

// ---   *   ---   *   ---
