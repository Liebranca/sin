// ---   *   ---   *   ---
// deps

  #include <glm/mat4x4.hpp>


  #include "chasm/window.h"

  #include "shader/src/Program.cpp"
  #include "shader/src/Texture.cpp"
  #include "shader/src/Frame.cpp"

  #include "mesh/src/T3D.cpp"
  #include "mesh/src/Camera.cpp"

  #include "mesh/src/Mesh.cpp"
  #include "mesh/src/Frame.cpp"

  #include "mesh/Solid.hpp"

// ---   *   ---   *   ---

static void* BUCK;

// ---   *   ---   *   ---

void draw(void) {
  ((Meshes*) BUCK)->draw(0);

};

// ---   *   ---   *   ---

int main(void) {

  winnt("CHASM",640,480,0);

  Programs p_frame;
  Program* p=p_frame.nit(&shader::mesh::Solid);

  p_frame.use(p);

  Texture tex("../bitter/out");
  tex.use();

  Meshes   m_frame;
  BUCK=&m_frame;

// ---   *   ---   *   ---
// wrap your head round this tri ;>

  Mesh::Vertex verts[4]={

    {.XYZ={0xFF,0x00,0x80},.TEX={0xFF,0x00}},
    {.XYZ={0x00,0x00,0x80},.TEX={0x00,0x00}},
    {.XYZ={0x00,0xFF,0x80},.TEX={0x00,0xFF}},

    {.XYZ={0xFF,0xFF,0x80},.TEX={0xFF,0xFF}},

  };

  uint16_t indices[6]={
    0,1,2,
    0,2,3

  };

  m_frame.nit(verts,indices,4,6);

// ---   *   ---   *   ---

  T3D transform;

  Camera::Lens lens={
    .width  = 640,
    .height = 480,

    .scale  = 0.0045f,
    .near   = 0.01f,
    .far    = 100.0f

  };

  Camera cam({0,0,0},lens);
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

    transform.move(vel);
    winrend(0,&draw);

  };

// ---   *   ---   *   ---
// cleanup

  windl();
  return 0;

};

// ---   *   ---   *   ---
