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
  #include "mesh/Debug_Line.hpp_sg"

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
// initialize camera

void SIN::nit_camera(

  const vec3& pos,
  const quat& rot,

  Camera::Lens&    lens,

  uint32_t         bind_idex,
  bool             ortho

) {

  cam.nit(pos,rot,lens,bind_idex);

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
  uint32_t mesh = sprites.size();

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
    : Sin.sprites[data.mesh].get_cpose_abs()
    ;

  Sin.enqueue(

    data.batch,
    meshid,

    this->xform().get_model(),
    this->xform().get_nmat()

  );

};

// ---   *   ---   *   ---
// moves accto set direction
// and linear velocity

void Node::lin_fmotion(void) {

  auto& Sin=SIN::ice();

  vec3 mvec=((
    (this->get_hax() * m_lindirn.x)
  + (this->get_up()  * m_lindirn.y)
  + (this->get_fwd() * m_lindirn.z)

  ) + m_linvel) * Sin.fBy();

  this->move(mvec);

};

// ---   *   ---   *   ---
// ^rotates

void Node::ang_fmotion(void) {

  auto& Sin=SIN::ice();

  vec3 dirn=((
    (this->get_hax() * m_angdirn.x)
  + (this->get_up()  * m_angdirn.y)
  + (this->get_fwd() * m_angdirn.z)

  ) + m_angvel) * Sin.fBy();;

  quat rvec={1,dirn.x,dirn.y,dirn.z};

  this->rotate(rvec);

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

  if(m_line_cnt) {

    m_line_vao.bind();

    program=programs.get(PROGRAM0);
    programs.bind(program);

    glDrawElements(

      GL_LINES,
      m_line_cnt*2,

      GL_UNSIGNED_SHORT,
      (void*) 0

    );

    m_line_cnt=0;
    m_line_vao.unbind();

  };

};

// ---   *   ---   *   ---
// ^selfex

void SIN::draw_line(

  vec3    a,
  vec3    b,

  uint8_t color_idex

) {

  uint16_t offset=m_line_cnt*2;

  vec4 verts[2]={
    {a.x,a.y,a.z,float(color_idex)},
    {b.x,b.y,b.z,float(color_idex)}

  };

  uint16_t indices[2]={
    uint16_t(offset+0),
    uint16_t(offset+1)

  };

  auto& vbo=m_line_vao.gbuff(VAO::VBO);
  auto& ibo=m_line_vao.gbuff(VAO::IBO);

  vbo.sub_data((void*) verts,offset,2);
  ibo.sub_data((void*) indices,offset,2);

  m_line_cnt++;

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

  m_line_vao.nit(

    GBuff::D_ARRAY,
    GBuff::D_ELEMENT,

    sizeof(vec4),
    LINE_VAO_SZ,

    sizeof(uint16_t),
    LINE_VAO_SZ

  );

  m_line_vao.vattr(

    VAO::R32_4,

    sizeof(vec4),
    0

  );

  m_line_vao.unbind();

};

// ---   *   ---   *   ---
// ctrash

SIN::SIN(void) {
  this->nit_buffs();
  programs.nit(&shader::mesh::Debug_Line);

};

SIN::~SIN(void) {};

// ---   *   ---   *   ---
