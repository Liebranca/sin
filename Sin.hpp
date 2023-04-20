#ifndef __SIN_H__
#define __SIN_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/Style.hpp"

  #include "shader/Frame.hpp"
  #include "mesh/Frame.hpp"
  #include "mesh/Camera.hpp"
  #include "world/Node.hpp"

// ---   *   ---   *   ---
// info

class SIN {

public:

  VERSION   "v0.00.4b";
  AUTHOR    "IBN-3DILA";

  cx8 PROGRAM0 = 0x00;
  cx8 PROGRAM1 = 0x01;

// ---   *   ---   *   ---
// helpers

  typedef std::vector<Meshes> Batches;
  typedef std::vector<Sprite> Sprites;
  typedef std::vector<Node>   Nodes;
  typedef std::vector<uint32_t> Statics;

// ---   *   ---   *   ---
// attrs

  Batches  meshes;
  Programs programs;
  Camera   cam;

  Program* program;
  Meshes*  batch;
  uint32_t batch_id=-1;

  Nodes    nodes;
  Sprites  sprites;
  Statics  statics;

// ---   *   ---   *   ---
// iface

  // ctrash
  SIN(void) {};
  ~SIN(void) {};

  SINGLETON(SIN);

  // create new mesh batch
  uint32_t new_batch(uint8_t pidex);

  // ^make current
  void use_batch(uint32_t idex);

  // instantiate an array of
  // shader params
  void nit_programs(
    shader::List& list

  );

  // initialize camera
  void nit_camera(

    const glm::vec3& pos,
    Camera::Lens&    lens,

    uint32_t         bind_idex,

    bool             ortho=true

  );

  uint32_t new_node(
    uint32_t meshid,
    uint8_t  type,

    T3D      xform=T3D()

  );

};

// ---   *   ---   *   ---

#endif // __SIN_H__
