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

  #include "Sin.hpp"

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

  bool ortho

) {

  cam=Camera(pos,lens);

  if(ortho) {
    cam.use_ortho();

  } else {
    cam.use_persp();

  };

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

    fprintf(
      stderr,
      "STATIC MESHES NOT IMPLEMENTED\n"

    );

    exit(1);

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

  auto& data   = node->draw_data();
  auto& model  = node->xform().get_model();

  auto& view   = Sin.cam.get_view();
  auto& proj   = Sin.cam.get_proj();

  auto  meshid = data.mesh;

  Sin.use_batch(data.batch);

  // TODO: camera should be UBO
  Sin.program->set_uniform(0,model);
  Sin.program->set_uniform(1,view);
  Sin.program->set_uniform(2,proj);

  Sin.batch->draw(Sin.sprites[meshid].play());

};

// ---   *   ---   *   ---
// ^idem

void Node::static_draw(Node* node) {

  fprintf(
    stderr,
    "DRAWING STATICS NOT IMPLEMENTED\n"

  );

  exit(1);

};

// ---   *   ---   *   ---
