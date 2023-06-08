#ifndef __SIN_H__
#define __SIN_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/GLM.hpp"

  #include "font/Text.hpp"
  #include "shader/Frame.hpp"
  #include "mesh/Frame.hpp"

// ---   *   ---   *   ---
// info

class SIN {

public:

  VERSION   "v0.01.0b";
  AUTHOR    "IBN-3DILA";

  // indices into shader array
  enum {

    LINE,TEXT,IMAGE,MESH,
    NUM_SHMODES

  };

  // indices into color array
  enum {

    BLACK,RED,GREEN,YELLOW,
    BLUE,PURPLE,CYAN,WHITE,

    BRIGHT=8

  };

  cx32 LINE_VAO_SZ = 0x100 * 2;

// ---   *   ---   *   ---
// helpers

  typedef std::vector<Meshes> Batches;
  typedef std::vector<Sprite> Sprites;

// ---   *   ---   *   ---
// attrs

public:

  Batches  meshes;
  Programs programs;

  Program* program;
  Meshes*  batch;
  uint32_t batch_id=-1;

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

  GBuff    m_gbuff[NUM_BUFFS];

  VAO      m_line_vao;
  Text     m_text;

  uint16_t m_line_cnt=0;

  // ^makes
  void nit_buffs(void);

  // updates ssbo with matrix block
  void upload_mats(
    Meshes::Draw_Queue_Mats& mats

  );

  uint32_t m_shaders[NUM_SHMODES];

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

  // use/dont use
  void bind_batch(uint32_t idex);
  void unbind_batch(void);

  // instantiate an array of
  // shader params
  void nit_programs(
    shader::List& list

  );

  // put draw commands "on hold"
  void enqueue(

    uint32_t   batid,
    uint32_t   meshid,

    mat4& model,
    mat3& nmat

  );

  // ^exec
  void draw_enqueued(void);

  void draw_line(

    vec3    a,
    vec3    b,

    uint8_t color_idex=RED

  );

  void draw_text(

    std::string ct,

    vec2        pos   = {0,0},
    vec2        dim   = {9,16},

    uint16_t    color = 0x00F7

  );

};

// ---   *   ---   *   ---

#endif // __SIN_H__
