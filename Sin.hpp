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

  VERSION   "v0.00.5b";
  AUTHOR    "IBN-3DILA";

  cx8 PROGRAM0 = 0x00;
  cx8 PROGRAM1 = 0x01;

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
  uint32_t batch_id=-1;

  Nodes    nodes;
  Sprites  sprites;

// ---   *   ---   *   ---
// guts

private:

  typedef std::vector <
    Meshes::Draw_Queue

  > Draw_Queues;

  struct Queue {
    Draw_Queues draw_data;

    uint32_t    top = 0;
    uint32_t    i   = 0;

  };

  typedef std::vector<Queue> Queues;

  Queues   m_queues;

  // for gl buffers
  enum {
    MATRIX_SSBO,
    NUM_BUFFS

  };

  uint32_t m_buff[NUM_BUFFS];

  // ^makes and undoes
  void nit_buffs(void);
  void del_buffs(void);

// ---   *   ---   *   ---
// iface

public:

  // ctrash
  SIN(void);
  ~SIN(void);

  SINGLETON(SIN);

  // create new mesh batch
  uint32_t new_batch(
    uint32_t texsz=256,
    uint32_t pidex=0

  );

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

  // put draw commands "on hold"
  void enqueue(

    uint32_t   batid,
    uint32_t   meshid,

    glm::mat4& model,
    glm::mat3& nmat

  );

  // ^exec
  void draw_enqueued(void);

  // updates ssbo with matrix block
  void upload_mats(
    Meshes::Draw_Queue_Mats& mats

  );

};

// ---   *   ---   *   ---

#endif // __SIN_H__
