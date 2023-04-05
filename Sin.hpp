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

  VERSION   "v0.00.2b";
  AUTHOR    "IBN-3DILA";

  cx8 PROGRAM0 = 0x00;

// ---   *   ---   *   ---
// helpers

  typedef std::vector<Meshes> Batches;
  typedef std::vector<Sprite> Sprites;
  typedef std::vector<Node>   Nodes;

// ---   *   ---   *   ---
// attrs

  Batches  meshes;
  Programs programs;
  Camera   cam;

  Program* program;
  Meshes*  batch;
  uint32_t batch_id;

  Nodes    nodes;
  Sprites  sprites;

// ---   *   ---   *   ---
// iface

  static inline SIN& ice(void) {
    static SIN ice;
    return ice;

  };

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

    bool ortho=true

  );

  uint32_t new_node(
    uint32_t meshid,
    uint8_t  type,

    T3D      xform=T3D()

  );

  // ctrash
  SIN(void) {};
  ~SIN(void) {};

  // this is what idiocy looks like
  SIN(SIN const&)             = delete;
  void operator=(SIN const&)  = delete;

};

// ---   *   ---   *   ---

#endif // __SIN_H__
