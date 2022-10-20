// ---   *   ---   *   ---
// MESH
// Points and lines come together
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include "mesh/Mesh.hpp"

//   ---     ---     ---     ---     ---

typedef struct {

  union {
    struct {

      char x;// plane coordinates
      char y;
      char z;
      char w;

      char c;// color
      char n;// normal
      char t;// tangent

      char e;

    };

    char arr[8];

  };

} Vertex;

// get your brain around this tri
static Vertex mesh[]={
  0x80,0x80,0x00,0xFF,0x0F,0x00,0x00,0x00,
  0x00,0x7F,0x00,0xFF,0x0C,0x00,0x00,0x00,
  0x7F,0x80,0x00,0xFF,0x0C,0x00,0x00,0x00

};

#define vertex_count \
  ((sizeof(mesh)/sizeof(vertex))-1)

//   ---     ---     ---     ---     ---

static int batch;
static int batch_vao;
static int batch_ubo;

static int batch_program;

enum shader_types {shd_vert,shd_frag,shd_num};
static int batch_shaders[shd_num]={0,0};

static Vertex camera={
  0x20,0x20,0x20,0x00,0x00,0x00,0x00,0x00

};

//   ---     ---     ---     ---     ---

static size_t sel_vertex=0;

void dr_meshbat(int keys) {

  Vertex* v=mesh+sel_vertex;
  //v->c=0xC;

  /*sel_vertex+=
   -(keys&0x10 && sel_vertex>0)
   +(keys&0x20 && sel_vertex<vertex_count);

  v=mesh+sel_vertex;
  v->c=0xF;

  v->x+=0x1*(
   -(keys&0x1 && v->x>-128)
   +(keys&0x2 && v->x<127)

  );v->y+=1*(
   -(keys&0x4 && v->y>-128)
   +(keys&0x8 && v->y<127)

  );*/

//   ---     ---     ---     ---     ---

  char cx=camera.x;
  char cy=camera.y;

  camera.x+=
   -((keys&0x10) && camera.x>-128)
   +((keys&0x20) && camera.x<127);

  camera.y+=
   -((keys&0x40) && camera.y>-128)
   +((keys&0x80) && camera.y<127);

  vec4 pos={32-camera.x,32-camera.y,48,ftoc(1)};
  vec4 at={0,0,0,ftoc(1)};
  vec4 up={0,32,0,ftoc(1)};

  mat4 view=lookat(pos,at,up);
  /*mat4 view={0};

  view.a=(vec4) {32,0,0,0};
  view.b=(vec4) {0,32,0,0};
  view.c=(vec4) {0,0,32,0};
  view.d=(vec4) {0,0,0,32};*/

  if(cx!=camera.x || cy!=camera.y) {
    prm(&view);

  };

};

//   ---     ---     ---     ---     ---
