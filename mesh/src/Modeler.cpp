// ---   *   ---   *   ---
// MODELER
// Makes geometry
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include "kvrnel/Seph.hpp"
  #include "mesh/Modeler.hpp"

// ---   *   ---   *   ---
// generate N points around origin

void Modeler::Joint::set_profile(
  uint16_t prof

) {

  m_verts.resize(prof);

  // get surface point
  vec3 fwd = {0,0,-1};

  // axis angle -> quaternion
  quat rot=T3D::qang(
    -Seph::PI/prof,{0,1,0}

  );

  // ^rotate around self
  for(uint16_t i=0;i<prof;i++) {

    m_verts[i]=Vertex(

      fwd * m_radius,
      fwd,

      m_base+i

    );

    fwd=fwd*rot;

  };

  m_profile=prof;
  this->updated=true;

};

// ---   *   ---   *   ---
// modify base idex for verts of joint

void Modeler::Joint::set_base(uint16_t base) {

  uint16_t i=0;

  for(auto& vert : m_verts) {
    vert.idex=base+i;
    i++;

  };

  m_base=base;
  this->updated=true;

};

// ---   *   ---   *   ---
// makes tris between points of
// two joints

void Modeler::join(
  uint16_t idex_a,
  uint16_t idex_b

) {

  auto& a=m_joints[idex_a];
  auto& b=m_joints[idex_b];

  uint16_t a_sz=a.get_profile();
  uint16_t b_sz=b.get_profile();

  if(a_sz == b_sz) {
    this->wind_even(a,b);

  } else if(a_sz < b_sz) {
    this->wind_uneven(b,a);

  } else {
    this->wind_uneven(a,b);

  };

  m_cache.calc_indices=true;

};

// ---   *   ---   *   ---
// default strategy

void Modeler::push_quad(

  Modeler::Joint& a,
  Modeler::Joint& b,

  uint16_t ai,
  uint16_t bi

) {

  auto& face=this->cur_face();

  // tri 0 (0,1,2)
  face.push_back(a.vof(ai+0));
  face.push_back(a.vof(ai+1));
  face.push_back(b.vof(bi+1));

  // tri 1 (0,2,3)
  face.push_back(a.vof(ai+0));
  face.push_back(b.vof(bi+1));
  face.push_back(b.vof(bi+0));

};

// ---   *   ---   *   ---
// ^way more awkward

void Modeler::push_tri(

  Modeler::Joint& a,
  Modeler::Joint& b,

  uint16_t ai,
  uint16_t bi

) {

  auto& face=this->cur_face();

  face.push_back(a.vof(ai+0));
  face.push_back(a.vof(ai+1));
  face.push_back(b.vof(bi+0));

//  printf(
//
//    "%u -> %u : %u\n",
//
//    a.iof(ai+0),
//    a.iof(ai+1),
//    b.iof(bi+0)
//
//  );

};

// ---   *   ---   *   ---
// evenly distributed tris

void Modeler::wind_even(
  Modeler::Joint& a,
  Modeler::Joint& b

) {

  for(uint16_t i=0;i<a.get_profile();i++) {
    this->new_face();
    this->push_quad(a,b,i,i);

  };

};

// ---   *   ---   *   ---
// ^utter chaos

void Modeler::wind_uneven(
  Modeler::Joint& a,
  Modeler::Joint& b

) {

  uint16_t bi = 1;
  uint16_t ai = 0;

  // get vertex distribution from
  // pre-calculated table
  auto d=this->get_distribution(a,b);

  // vertex count of A
  // for each vertex of B
  for(auto cnt : d) {

    this->new_face();
    this->push_tri(b,a,bi-1,ai);

    for(uint16_t i=0;i<cnt-1;i++) {
      this->push_tri(a,b,ai,bi);
      ai++;

    };

    bi++;

  };

  this->new_face();
  this->push_tri(b,a,bi-1,0);

};

// ---   *   ---   *   ---
// make tris within an elements
// own verts

void Modeler::cap(
  uint16_t idex,
  bool     up

) {

  auto& joint=m_joints[idex];

  // limits
  uint16_t prof = joint.get_profile();
  uint16_t cnt  = prof/2;
  uint16_t left = cnt+(prof & 1);

  // offsets
  uint16_t i    = 0;
  uint16_t j    = prof-1;

  // ^iter
  this->new_face();
  for(;i<cnt;i++,j--) {

    this->push_tri(joint,joint,i,j);
    left--;

    if(left) {
      this->push_tri(joint,joint,j-1,i+1);
      left--;

    };

  };

  // ^calc vertex normal inclination
  float sign=(up) ? -1 : 1;
  for(auto& vert : joint.get_verts()) {

    T3D::Facing dirn=vert.n;

    quat rot=T3D::qang(
      sign*(Seph::PI/8),dirn.hax

    );

    vert.n=vert.n*rot;

  };

  // mark for update
  m_cache.calc_indices=true;
  m_cache.calc_deforms=true;

};

// ---   *   ---   *   ---
// sum verts of all elements
//
// TODO: account for 'flat' duplicates

uint16_t Modeler::get_icount(void) {

  m_icount=(m_cache.calc_indices)
    ? 0
    : m_icount
    ;

  if(m_icount==0) {

    for(auto& face : m_faces) {
      m_icount+=face.size();

    };

  };

  return m_icount;

};

// ---   *   ---   *   ---
// get vertex indices for drawing
//
// TODO: duplicate 'flat' vertices

void Modeler::calc_indices(void) {

  uint16_t i=0;

  for(auto& face : m_faces) {
    for(auto& vert : face) {
      m_mesh.indices[i++]=vert.get().idex;

    };

  };

};

// ---   *   ---   *   ---
// sum verts of all elements

uint16_t Modeler::get_vcount(void) {

  m_vcount=(this->get_updated())
    ? 0
    : m_vcount
    ;

  if(m_vcount==0) {

    for(auto& joint : m_joints) {
      m_vcount+=joint.get_verts().size();

    };

  };

  return m_vcount;

};

// ---   *   ---   *   ---
// cache a copy of each elements
// verts with transforms applied

void Modeler::calc_deforms(void) {

  uint16_t i=0;

  for(auto& joint : m_joints) {

    auto& model = joint.get_xform().get_model();
    auto& nmat  = joint.get_xform().get_nmat();

    for(auto& vert : joint.get_verts()) {

      m_deformed[i]=Vertex(

        vec3(model * vec4(

          vert.co.x,
          vert.co.y,
          vert.co.z,

          1

        )),

        nmat * vert.n,
        i

      );

      i++;

    };

  };

};

// ---   *   ---   *   ---
// ^get mesh requires recalc

bool Modeler::get_updated(void) {

  // double check each element
  for(auto& joint : m_joints) {

    // feed element update back onto frame
    m_cache.calc_indices |= joint.updated;
    m_cache.calc_deforms |=
      joint.get_xform().get_updated();

  };

  return
     m_cache.calc_indices
  || m_cache.calc_deforms
  ;

};

// ---   *   ---   *   ---
// ^wraps over update routines

void Modeler::calc_mesh(void) {

  uint16_t old=m_vcount;

  // check array sizes
  if(old != this->get_vcount()) {
    m_deformed.resize(m_vcount);
    m_mesh.verts.resize(m_vcount);

  };

  old=m_icount;
  if(old != this->get_icount()) {
    m_mesh.indices.resize(m_icount);

  };

  // run necessary updates
  this->get_updated();
  if(m_cache.calc_indices) {

    this->calc_indices();

    m_cache.calc_indices = false;
    m_cache.repack       = true;

  };

  if(m_cache.calc_deforms) {

    this->calc_deforms();

    m_cache.calc_deforms = false;
    m_cache.repack       = true;

  };

};

// ---   *   ---   *   ---
// ^takes deformed mesh data and
// packs it to crk format

void Modeler::pack(void) {

  uint16_t i=0;

  for(auto& vert : m_deformed) {

    auto& dst=m_mesh.verts[i++];

    dst.set_xyz(vert.co);
    dst.set_n(vert.n);

  };

};

// ---   *   ---   *   ---
// get packed version of mesh for drawing

CRK::Prim& Modeler::get_packed(void) {

  this->calc_mesh();

  if(m_cache.repack) {
    this->pack();
    m_cache.repack=false;

  };

  return m_mesh;

};

// ---   *   ---   *   ---
