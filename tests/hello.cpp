// ---   *   ---   *   ---
// deps

  #include <glm/mat4x4.hpp>

  #include "chasm/Chasm.hpp"
  #include "Sin.hpp"

  #include "mesh/Solid.hpp_sg"

// ---   *   ---   *   ---
// helpers

  typedef std::vector<uint32_t> Nodes;

// ---   *   ---   *   ---
// lame fwd decl

  int draw(void* data);

// ---   *   ---   *   ---
// chasm stuff

void spawn_window(void) {

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

};

// ---   *   ---   *   ---
// selfex

void camera_settings(bool ortho=true) {

  auto& Sin=SIN::ice();

  Camera::Lens lens={
    .width  = 640,
    .height = 480,

    .scale  = 0.01f,
    .fov    = 45.0f,

    .near   = 0.1f,
    .far    = 100.0f

  };

  Sin.nit_camera({0,0,8},lens,ortho);

};

// ---   *   ---   *   ---
// ^idem

void load_shaders(void) {

  auto& Sin=SIN::ice();

  shader::List programs {
    &shader::mesh::Solid

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

};

// ---   *   ---   *   ---
// chasm signal for draw step

int draw(void* data) {

  auto& Sin=SIN::ice();
  auto  bat=(Nodes*) data;

  for(auto& idex : *bat) {
    Sin.nodes[idex].draw();

  };

  return 1;

};

// ---   *   ---   *   ---

int main(void) {

  spawn_window();

  load_shaders();
  load_resources();

  camera_settings();

  T3D xform({1,0,0,1});

  auto& Sin=SIN::ice();

  Sin.new_node(0,Node::SPRITE);
  Sin.new_node(0,Node::SPRITE,xform);

  Nodes draw_buff {0,1};

  CHASM_RUN((void*) &draw_buff,NULL);

  return 0;

};

// ---   *   ---   *   ---
