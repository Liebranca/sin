// ---   *   ---   *   ---
// deps

  #include <pthread.h>
  #include <glm/mat4x4.hpp>

  #include "chasm/Chasm.hpp"
  #include "Sin.hpp"

  #include "mesh/Solid.hpp_sg"

// ---   *   ---   *   ---
// helpers

  typedef std::vector<uint32_t> Nodes;

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

    .width      = 640,
    .height     = 480,

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
    .width  = 640,
    .height = 480,

    .scale  = 0.0055f,
    .fov    = 45.0f,

    .near   = 0.1f,
    .far    = 100.0f

  };

  Sin.nit_camera({0,0,20},lens,bind_idex,ortho);

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

  auto& Sin = SIN::ice();
  auto  bat = (Nodes*) data;

  for(auto& idex : *bat) {
    Sin.nodes[idex].draw();

  };

  return 0;

};

// ---   *   ---   *   ---
// ^selfex

int logic(void* data) {

//  static int i=0;
//  cxr32 vel=0.05f;
//
//  auto& Sin = SIN::ice();
//  auto& nd  = Sin.nodes[0];
//
//  float xdir=(i<128) ? -(vel*2.1) : (vel*2.1);
//  float ydir=vel*1.125;
//
//  nd.move({xdir,ydir,0});
//
//  i++;
//
//  if(i==0x7F) {
//    nd.rot({1,0,180,0});
//    nd.move({0,-(ydir*0x80),0});
//
//  } else if(i==0xFF) {
//    nd.rot({1,0,-180,0});
//    nd.move({0,-(ydir*0x80),0});
//
//  };
//
//  i&=0xFF;

  return 1;

};

// ---   *   ---   *   ---

int main(void) {

  spawn_window();

  load_shaders();
  load_resources();

  auto& Chasm = CHASM::ice();
  auto& Sin   = SIN::ice();

  camera_settings(Sin.program->get_ubo(0));

  Nodes draw_buff;
//  T3D xform({6,-3.5f,0,1});
  Sin.new_node(0,Node::SPRITE);

  draw_buff.push_back(0);

  CHASM_RUN((void*) &draw_buff,NULL);

  return 0;

};

// ---   *   ---   *   ---

//  uint32_t limit = 1;
//  uint32_t i     = 0;
//
//  float co[3]={
//
//    -6.5f,
//    -3.5f,
//
//    0.0f,
//
//  };
//
//  for(int y=0;y<limit;y++) {
//
//    for(int x=0;x<limit;x++) {
//
//      T3D xform({co[0],co[1],co[2],1});
//      Sin.new_node(0,Node::SPRITE,xform);
//
//      draw_buff.push_back(i++);
//
//      co[0] += 0.25f;
//
//    };
//
//    co[0]  = -6.5f;
//
//    co[1] += 0.25f;
//    co[2] += 0.1f;
//
//  };

// ---   *   ---   *   ---
