#ifndef __SIN_H__
#define __SIN_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/GLM.hpp"

  #include "ui/Frame.hpp"
  #include "ui/Debug_Draw.hpp"

  #include "shader/Frame.hpp"
  #include "mesh/Frame.hpp"

// ---   *   ---   *   ---
// info

class SIN {

public:

  VERSION   "v0.01.2b";
  AUTHOR    "IBN-3DILA";

  // indices into shader array
  enum {

    DEBUG,TEXT,IMAGE,MESH,
    NUM_SHMODES

  };

  // indices into color array
  enum {

    BLACK,RED,GREEN,YELLOW,
    BLUE,PURPLE,CYAN,WHITE,

    BRIGHT  = 0x08,
    OPAQUE  = 0xF0,
    XPARENT = 0x80

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

  typedef Debug_Draw DD;

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

  GBuff m_gbuff[NUM_BUFFS];

  DD    m_dd_lines;
  DD    m_dd_points;

  UI    m_ui;

  // ^makes
  void nit_buffs(void);

  // updates ssbo with matrix block
  void upload_mats(
    Meshes::Draw_Queue_Mats& mats

  );

  uint32_t m_shaders[NUM_SHMODES];

// ---   *   ---   *   ---
// sub-steps of draw_enqueued

  void draw_nodes(void);

  void draw_dd(void);
  void draw_ui(void);

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

  // world space coords
  void draw_line(

    vec3    a,
    vec3    b,

    uint8_t color=OPAQUE|RED,
    float   width=1.0f

  );

  // ^screen space
  void draw_line(

    vec2    a,
    vec2    b,

    uint8_t color=OPAQUE|RED,
    float   width=1.0f

  );

  // world space coords
  void draw_point(

    vec3    a,

    uint8_t color=OPAQUE|YELLOW,
    float   width=1.0f

  );

  // ^screen space
  void draw_point(

    vec2    a,

    uint8_t color=OPAQUE|YELLOW,
    float   width=1.0f

  );

  // makes ui element
  // gives idex to it
  uint32_t draw_ui_text(

    std::string ct,

    vec2        pos      = {0,0},
    vec3        dim      = {9,16,8},

    uint16_t    color    = OPAQUE|WHITE,
    bool        show_ctl = false

  );

  // ^same system, plain quad
  uint32_t draw_ui_rect(

    vec2     pos,
    vec2     dim,

    uint16_t color=(OPAQUE|BRIGHT|BLACK) << 8

  );

  // ^retrieve
  inline UI::Elem& ui_elem(uint32_t idex) {
    return m_ui.get_elem(idex);

  };

  // ui draw cursor ctl
  inline vec2& get_ui_cursor(void) {
    return m_ui.get_cursor();

  };

  inline void reset_ui_cursor(void) {
    m_ui.reset_cursor();

  };

  inline float get_line_layer(void) {
    return m_dd_lines.get_layer();

  };

  inline void set_line_layer(float z) {
    m_dd_lines.set_layer(z);

  };

  inline float get_point_layer(void) {
    return m_dd_points.get_layer();

  };

  inline void set_point_layer(float z) {
    m_dd_points.set_layer(z);

  };

};

// ---   *   ---   *   ---

#endif // __SIN_H__
