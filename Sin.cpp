// ---   *   ---   *   ---
// SIN
// Byte-sized singleton
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/Bytes.hpp"

  #include "glsl/ui/Debug_Draw.hpp_sg"
  #include "glsl/ui/Raster.hpp_sg"
  #include "glsl/mesh/JOJ_Sprite.hpp_sg"
  #include "glsl/mesh/M3D.hpp_sg"

  #include "Sin.hpp"

// ---   *   ---   *   ---
// decld at mesh/Sprite

Sprite::Poses& Sprite::fetch_poses(
  uint32_t idex

) {

  auto& Sin=SIN::ice();
  return Sin.batch->get_anims()[idex];

};

ANS& Sprite::fetch_meta(
  uint32_t idex

) {

  auto& Sin=SIN::ice();
  return Sin.batch->get_anim_meta()[idex];

};

// ---   *   ---   *   ---
// create new mesh batch

uint32_t SIN::new_batch(
  uint32_t texsz,
  uint32_t pidex

) {

  uint32_t out=meshes.size();

  meshes.push_back(Meshes());
  meshes.back().nit(m_shaders[pidex],texsz);

  this->bind_batch(out);
  m_queues.push_back(Queue());

  return out;

};

// ---   *   ---   *   ---
// ^make current

void SIN::bind_batch(uint32_t idex) {

  // already set, skip
  if(idex == batch_id) {
    return;

  };

  // ^else set state
  batch_id = idex;

  batch    = &meshes[idex];
  program  = programs.get(batch->get_pidex());

  programs.bind(program);
  batch->bind();

};

// ---   *   ---   *   ---
// ^iv

void SIN::unbind_batch(void) {

  if(batch == NULL || batch_id== -1) {
    return;

  };

  batch->unbind();

  batch    = NULL;
  batch_id = -1;

};

// ---   *   ---   *   ---
// instantiate an array of
// shader params

void SIN::nit_programs(
  shader::List& list

) {

  for(auto params : list) {
    programs.nit(params);

  };

};

// ---   *   ---   *   ---
// put draw commands "on hold"

void SIN::enqueue(

  uint32_t   batid,
  uint32_t   meshid,

  mat4& model,
  mat3& nmat

) {

  // get queue for this batch
  auto& q=m_queues[batid];
  while(q.draw_data.size() < q.top+1) {
    q.draw_data.push_back(Meshes::Draw_Queue());

  };

  // ^get draw data assoc with batch
  auto& dq   = q.draw_data[q.top];
  auto& mats = dq.mats;

  // set matrices
  mats.model[q.i] = model;
  mats.nmat[q.i]  = nmat;

  // set mesh ptr
  dq.meshid[q.i]  = meshid;

  dq.cnt++;
  q.i++;

  if(q.i == Meshes::QUEUE_SZ) {

    q.top++;
    q.i=0;

  };

};

// ---   *   ---   *   ---
// ^exec

void SIN::draw_enqueued(void) {

  this->draw_nodes();

  this->draw_dd();
  this->draw_ui();

};

// ---   *   ---   *   ---
// draw enqueued nodes

void SIN::draw_nodes(void) {

  uint32_t i=0;

  // one queue for each batch
  for(auto& q : m_queues) {

    if(! q.draw_data.size()) {
      i++;continue;

    };

    this->bind_batch(i);

    // ^multiple matrix blocks
    // per batch
    for(uint32_t j=0;j<q.top+1;j++) {

      auto& dq=q.draw_data[j];
      this->upload_mats(dq.mats);

      // ^multiple draws per
      // matrix block
      for(

        uint32_t drawid=0;

        drawid < dq.cnt;
        drawid++

      ) {

        // get material index for mesh
        uint32_t meshid = dq.meshid[drawid];
        uint32_t matid  = batch->matof(meshid);

        // ^single uniform per draw ;>
        uvec4 obdata={
          drawid,
          matid,
          0,
          0

        };

        // upload
        program->set_uniform(0,obdata);
        batch->draw(meshid);

      };

      dq.cnt=0;

    };

    // reset counters
    q.top = 0;
    q.i   = 0;

    // advance batid
    i++;

    this->unbind_batch();

  };

};

// ---   *   ---   *   ---
// draw enqueued debug points/lines

void SIN::draw_dd(void) {

  bool lines  = m_dd_lines.ready();
  bool points = m_dd_points.ready();

  if(lines || points) {

    program=programs.get(m_shaders[DEBUG]);
    programs.bind(program);

    if(lines) {
      m_dd_lines.upload();
      m_dd_lines.draw();
      m_dd_lines.clear();

    };

    if(points) {
      m_dd_points.upload();
      m_dd_points.draw();
      m_dd_points.clear();

    };

  };

};

// ---   *   ---   *   ---
// draw enqueued ui elements

void SIN::draw_ui(void) {

  if(m_ui.ready()) {

    program=programs.get(m_shaders[TEXT]);
    programs.bind(program);

    m_ui.upload();
    m_ui.draw();

  };

};

// ---   *   ---   *   ---
// world space coords

void SIN::draw_line(

  vec3    a,
  vec3    b,

  uint8_t color,
  float   width

) {

  m_dd_lines.push_line(
    a,b,width,color,
    DD::DRAW_3D

  );

};

// ---   *   ---   *   ---
// ^screen space

void SIN::draw_line(

  vec2    a,
  vec2    b,

  uint8_t color,
  float   width

) {

  vec3 a3(a.x,a.y,0);
  vec3 b3(b.x,b.y,0);

  m_dd_lines.push_line(
    a3,b3,width,color,
    DD::DRAW_2D

  );

};

// ---   *   ---   *   ---
// world space coords

void SIN::draw_point(

  vec3    a,

  uint8_t color,
  float   width

) {

  m_dd_points.push_point(
    a,width,color,
    DD::DRAW_3D

  );

};

// ---   *   ---   *   ---
// ^screen space

void SIN::draw_point(

  vec2    a,

  uint8_t color,
  float   width

) {

  vec3 a3(a.x,a.y,0);

  m_dd_points.push_point(
    a3,width,color,
    DD::DRAW_2D

  );

};

// ---   *   ---   *   ---
// makes ui element
// gives idex to it

uint32_t SIN::draw_ui_text(

  std::string ct,

  vec2        pos,
  vec3        dim,

  uint16_t    color,
  bool        show_ctl

) {

  return m_ui.push_text(
    ct,pos,dim,color,show_ctl

  );

};

// ---   *   ---   *   ---
// ^same system, plain quad

uint32_t SIN::draw_ui_rect(

  vec2     pos,
  vec2     dim,

  uint16_t color

) {

  return m_ui.push_quad(
    pos,dim,color

  );

};

// ---   *   ---   *   ---
// updates ssbo with matrix block

void SIN::upload_mats(
  Meshes::Draw_Queue_Mats& mats

) {

  m_gbuff[MATRIX_SSBO].sub_data(
    (void*) &mats,0,1

  );

};

// ---   *   ---   *   ---
// gl boiler

void SIN::nit_buffs(void) {

  m_gbuff[MATRIX_SSBO].nit(

    GBuff::D_STORAGE,

    sizeof(Meshes::Draw_Queue_Mats),
    1

  );

  m_gbuff[MATRIX_SSBO].bind_base(0);

};

// ---   *   ---   *   ---
// ctrash

SIN::SIN(void) {

  glEnable(GL_PROGRAM_POINT_SIZE);

  this->nit_buffs();

  m_shaders[DEBUG]=programs.nit(
    &shader::ui::Debug_Draw

  );

  m_shaders[TEXT]=programs.nit(
    &shader::ui::Raster

  );

  m_shaders[IMAGE]=programs.nit(
    &shader::mesh::JOJ_Sprite

  );

  m_shaders[MESH]=programs.nit(
    &shader::mesh::M3D

  );

  m_dd_lines.nit(0x400,GL_LINES);
  m_dd_points.nit(0x400,GL_POINTS);

  m_ui.nit_vao(0x1000);

};

SIN::~SIN(void) {};

// ---   *   ---   *   ---
