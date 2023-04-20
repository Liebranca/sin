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
// lame fwd

  void prich_fmodel(glm::mat4& model);
  void prich_imodel(glm::mat4& model);

// ---   *   ---   *   ---
// decld at mesh/Sprite

Sprite::Poses& Sprite::fetch_poses(
  uint32_t idex

) {

  auto& Sin=SIN::ice();
  return Sin.batch->get_anims()[idex];

};

Texture& Sprite::fetch_sheet(
  uint32_t idex

) {

  auto& Sin=SIN::ice();
  return Sin.batch->get_textures()[idex];

};

ANS& Sprite::fetch_meta(
  uint32_t idex

) {

  auto& Sin=SIN::ice();
  return Sin.batch->get_anim_meta()[idex];

};

// ---   *   ---   *   ---
// create new mesh batch

uint32_t SIN::new_batch(uint8_t pidex) {

  uint32_t out=meshes.size();

  meshes.push_back(Meshes());
  meshes.back().nit(pidex);

  this->use_batch(out);

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

  if(type == Node::SPRITE) {

    mesh=sprites.size();

    sprites.push_back(
      batch->ice_sprite(meshid)

    );

  } else {

    mesh=statics.size();

    statics.push_back(
      batch->ice_static(meshid)

    );

  };

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

void Node::sprite_draw(Node* node) {

  auto& Sin    = SIN::ice();

  // TODO: sort all of this out of here ;>
  auto& data   = node->draw_data();
  auto& model  = node->xform().get_model();
  auto  meshid = data.mesh;

  Sin.use_batch(data.batch);

  Sin.program->set_uniform(0,model);
  Sin.batch->draw(Sin.sprites[meshid].play());

};

// ---   *   ---   *   ---
// ^idem

void Node::static_draw(Node* node) {

  auto& Sin    = SIN::ice();

  // TODO: sort all of this out of here ;>
  auto& data   = node->draw_data();
  auto& model  = node->xform().get_model();
  auto  meshid = data.mesh;

  Sin.use_batch(data.batch);

  Sin.program->set_uniform(0,model);
  Sin.batch->draw(Sin.statics[meshid]);

};

// ---   *   ---   *   ---
// debug stuff

void prich_fmodel(glm::mat4& model) {

  printf(

    "[%.3f %.3f %.3f %.3f]\n"
    "[%.3f %.3f %.3f %.3f]\n"
    "[%.3f %.3f %.3f %.3f]\n"
    "[%.3f %.3f %.3f %.3f]\n"

    "\n\n",

    model[0][0],model[0][1],model[0][2],model[0][3],
    model[1][0],model[1][1],model[1][2],model[1][3],
    model[2][0],model[2][1],model[2][2],model[2][3],
    model[3][0],model[3][1],model[3][2],model[3][3]

  );

};

// ---   *   ---   *   ---
// ^frac'd

void prich_imodel(glm::mat4& model) {

  int16_t um[4][4];
  float  fum[4][4];

  Frac::Rounding_Mode=Frac::CORD;

  for(int y=0;y<4;y++) {
  for(int x=0;x<4;x++) {

    um[y][x]=frac<int16_t>(

      model[y][x],

      Frac::STEP[Frac::STEP_5BIT],
      Frac::BITS[Frac::SIZE_8BIT],

      Frac::SIGNED

    );

    model[y][x]=unfrac<int16_t>(

      um[y][x],

      Frac::STEP[Frac::STEP_5BIT],
      Frac::BITS[Frac::SIZE_8BIT],

      Frac::SIGNED

    );

  }};

//  prich_fmodel(model);

};

// ---   *   ---   *   ---
