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
  meshes.back().nit(pidex,texsz);

  this->use_batch(out);
  m_queues.push_back(Queue());

  return out;

};

// ---   *   ---   *   ---
// ^make current

void SIN::use_batch(uint32_t idex) {

  // already set, skip
  if(idex == batch_id) {
    return;

  };

  // ^else set state
  batch_id = idex;

  batch    = &meshes[idex];
  program  = programs.get(batch->get_pidex());

  programs.use(program);
  batch->use();

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
// initialize camera

void SIN::nit_camera(

  const glm::vec3& pos,
  Camera::Lens&    lens,

  uint32_t         bind_idex,
  bool             ortho

) {

  cam.nit(pos,lens,bind_idex);

  if(ortho) {
    cam.use_ortho();

  } else {
    cam.use_persp();

  };

  cam.get_view();

};

// ---   *   ---   *   ---

uint32_t SIN::new_node(
  uint32_t meshid,
  uint8_t  type,

  T3D      xform

) {

  uint32_t out  = nodes.size();
  uint32_t mesh = 0;

  mesh=sprites.size();
  sprites.push_back(
    batch->ice_asset(meshid)

  );

  Node::Bld bld={

    .draw={
      .batch = batch_id,
      .mesh  = mesh,

      .type  = type

    },

    .xform=xform

  };

  nodes.push_back(Node());
  nodes.back().nit(bld);

  return out;

};

// ---   *   ---   *   ---
// decld at world/Node

void Node::draw(void) {

  auto& Sin    = SIN::ice();
  auto& data   = this->draw_data();

  auto  meshid = (data.type==Node::ANIMATED)
    ? Sin.sprites[data.mesh].play()
    : Sin.sprites[data.mesh].get_cpose()
    ;

  Sin.enqueue(

    data.batch,
    meshid,

    this->xform().get_model(),
    this->xform().get_nmat()

  );

};

// ---   *   ---   *   ---
// put draw commands "on hold"

void SIN::enqueue(

  uint32_t   batid,
  uint32_t   meshid,

  glm::mat4& model,
  glm::mat3& nmat

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

  uint32_t i=0;

  // one queue for each batch
  for(auto& q : m_queues) {

    if(! q.draw_data.size()) {
      i++;continue;

    };

    this->use_batch(i);

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
        glm::uvec4 obdata={
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

  };

};

// ---   *   ---   *   ---
// updates ssbo with matrix block

void SIN::upload_mats(
  Meshes::Draw_Queue_Mats& mats

) {

  glBindBuffer(
    GL_SHADER_STORAGE_BUFFER,
    m_buff[MATRIX_SSBO]

  );

  glBufferSubData(
    GL_SHADER_STORAGE_BUFFER,0,

    sizeof(mats),
    (void*) &mats

  );

};

// ---   *   ---   *   ---
// gl boiler

void SIN::nit_buffs(void) {

  glGenBuffers(NUM_BUFFS,&m_buff[0]);

  glBindBuffer(
    GL_SHADER_STORAGE_BUFFER,
    m_buff[MATRIX_SSBO]

  );

  glBufferData(
    GL_SHADER_STORAGE_BUFFER,

    sizeof(Meshes::Draw_Queue_Mats),

    NULL,
    GL_DYNAMIC_DRAW

  );

  glBindBufferBase(
    GL_SHADER_STORAGE_BUFFER,
    0,

    m_buff[MATRIX_SSBO]

  );

};

// ---   *   ---   *   ---
// ^cleanup

void SIN::del_buffs(void) {
  glDeleteBuffers(NUM_BUFFS,&m_buff[0]);

};

// ---   *   ---   *   ---
// ctrash

SIN::SIN(void) {
  this->nit_buffs();

};

SIN::~SIN(void) {
  this->del_buffs();

};

// ---   *   ---   *   ---
