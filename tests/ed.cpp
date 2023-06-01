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

//// ---   *   ---   *   ---
//// lame fwd decls
//
//  void step_left(void);
//  void step_right(void);
//
//// ---   *   ---   *   ---
//// ROM
//
//BEG_KEYSET(ED)
//
//  {escape,{
//
//    NULL,
//    NULL,
//
//    &chasm_exit
//
//  }},
//
//  {left,{
//
//    &step_left,
//    &step_left,
//
//    NULL
//
//  }},
//
//  {right,{
//
//    &step_right,
//    &step_right,
//
//    NULL
//
//  }}
//
//END_KEYSET;

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

inline Clock& get_clock(void) {
  auto& Chasm=CHASM::ice();
  return Chasm.win.clock();

};

void spawn_window(void) {

  auto& Chasm=CHASM::ice();

  Win::Desc win_desc={

    .title      = "HELLO.CPP",

    .width      = SCR_X,
    .height     = SCR_Y,

    .fullscreen = false,
    .fps        = 60,

    .flags      = Win::DO_MOUSE_TRAP

  };

  Chasm.nit(win_desc);
  Chasm.win.set_ambient_color(6);

  Chasm.draw  = &draw;
  Chasm.logic = &logic;

  // get handle to program clock
  auto& Sin=SIN::ice();
  Sin.get_clock=&get_clock;

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

  Sin.new_batch(SIN::PROGRAM1);
  Sin.batch->new_edit();

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

  auto& Sin   = SIN::ice();
  auto& Chasm = CHASM::ice();

  auto& rat   = Chasm.ev.get_rat();
  auto  mo    = rat.get_motion(0.01f);

  auto& cam   = Sin.cam;

  bool dx = fabs(mo.y) > 0.0001f;
  bool dy = fabs(mo.x) > 0.0001f;

  if(dx || dy) {

    auto  hax = cam.get_hax() * mo.y;

    glm::quat y {1,-hax.x,hax.y,-hax.z};
    glm::quat x {1,0,-mo.x,0};

    glm::quat r=x*y;
    glm::vec3 v={r.x,r.y,r.z};

    v*=4;

    cam.set_angvel(v);
    cam.ang_fmotion();

  };

  auto lclick = rat.clicks(Rat::LEFT);
  auto rclick = rat.clicks(Rat::RIGHT);

  if(lclick || rclick) {

    float sign = (lclick) ? -1 : 1;

    cam.set_lindirn_ax(2,sign * 4);
    cam.lin_fmotion();

  };

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
