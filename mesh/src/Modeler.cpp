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

void Modeler::Ring::set_profile(
  uint16_t prof

) {

  m_hax_verts.resize(prof+1);

  // get surface point
  vec3 fwd = {0,0,-1};

  // axis angle -> quaternion
  quat rot=T3D::qang(
    -Seph::PI/prof,{0,1,0}

  );

  // ^rotate around self
  for(uint16_t i=0;i<prof;i++) {

    m_hax_verts[i]=Vertex(

      fwd * m_radius,
      fwd,

      m_base+i

    );

    fwd=fwd*rot;

  };

  // duplicate first horizontal vertex for uv
  m_hax_verts.back()=m_hax_verts[0];
  m_hax_verts.back().idex=m_base+prof;

  // register changes
  m_profile = prof;
  m_capped  = false;

  this->updated=true;

};

// ---   *   ---   *   ---
// modify base idex for verts of ring

void Modeler::Ring::set_base(uint16_t base) {

  uint16_t i=0;
  for(auto& vert : m_hax_verts) {
    vert.idex=base+i;
    i++;

  };

  m_base=base;
  this->updated=true;

};

// ---   *   ---   *   ---
// set rinf vertex distance from origin

void Modeler::Ring::set_radius(float r) {

  for(auto& vert : m_hax_verts) {
    vert.co=vert.n*r;

  };

  for(auto& vert : m_cap_verts) {
    vert.co=vert.n*r;

  };

  m_radius=r;

};

// ---   *   ---   *   ---
// makes tris between points of
// two rings

void Modeler::join(
  uint16_t idex_a,
  uint16_t idex_b

) {

  auto& a=m_rings[idex_a];
  auto& b=m_rings[idex_b];

printf("%u -> %u\n",idex_a,idex_b);

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

  Modeler::Ring& a,
  Modeler::Ring& b,

  uint16_t ai,
  uint16_t bi,

  bool     cap

) {

  auto& face=this->cur_face();

  if(cap) {

    // tri 0 (0,1,2)
    face.push_back(a.cvof(ai+0));
    face.push_back(a.cvof(ai+1));
    face.push_back(b.cvof(bi+1));

    // tri 1 (0,2,3)
    face.push_back(a.cvof(ai+0));
    face.push_back(b.cvof(bi+1));
    face.push_back(b.cvof(bi+0));

  } else {

    // tri 0 (0,1,2)
    face.push_back(a.hvof(ai+0));
    face.push_back(a.hvof(ai+1));
    face.push_back(b.hvof(bi+1));

    // tri 1 (0,2,3)
    face.push_back(a.hvof(ai+0));
    face.push_back(b.hvof(bi+1));
    face.push_back(b.hvof(bi+0));

printf(

  "%u: %u,%u,%u,%u,%u,%u\n",

  m_faces.size(),

  a.hvof(ai+0).idex,
  a.hvof(ai+1).idex,
  b.hvof(bi+1).idex,

  a.hvof(ai+0).idex,
  b.hvof(bi+1).idex,
  b.hvof(bi+0).idex

);

  };

};

// ---   *   ---   *   ---
// ^way more awkward

void Modeler::push_tri(

  Modeler::Ring& a,
  Modeler::Ring& b,

  uint16_t ai,
  uint16_t bi,

  bool     cap

) {

  auto& face=this->cur_face();

  if(cap) {
    face.push_back(a.cvof(ai+0));
    face.push_back(a.cvof(ai+1));
    face.push_back(b.cvof(bi+0));

  } else {
    face.push_back(a.hvof(ai+0));
    face.push_back(a.hvof(ai+1));
    face.push_back(b.hvof(bi+0));

  };

};

// ---   *   ---   *   ---
// evenly distributed tris

void Modeler::wind_even(
  Modeler::Ring& a,
  Modeler::Ring& b

) {

  for(uint16_t i=0;i<a.get_profile();i++) {
    this->new_face();
    this->push_quad(a,b,i,i,false);

  };

};

// ---   *   ---   *   ---
// ^utter chaos

void Modeler::wind_uneven(
  Modeler::Ring& a,
  Modeler::Ring& b

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
    this->push_tri(b,a,bi-1,ai,false);

    for(uint16_t i=0;i<cnt-1;i++) {
      this->push_tri(a,b,ai,bi,false);
      ai++;

    };

    bi++;

  };

  this->new_face();
  this->push_tri(b,a,bi-1,0,false);

};

// ---   *   ---   *   ---
// resize and populate container
// for split top/bottom vertices

void Modeler::Ring::cap_prologue(void) {

  m_cap_verts.resize(m_profile);

  for(uint16_t i=0;i<m_profile;i++) {
    m_cap_verts[i]=m_hax_verts[i];
    m_cap_verts[i].idex+=m_profile+1;

  };

  m_capped=true;

};

// ---   *   ---   *   ---
// ^undo

bool Modeler::Ring::uncap(void) {

  bool out=m_capped;
  m_cap_verts.clear();

  this->updated = true;
  m_capped      = false;

  return out;

};

// ---   *   ---   *   ---
// make tris within an elements
// own verts

void Modeler::cap(
  uint16_t idex,
  bool     up

) {

  auto& ring=m_rings[idex];
  ring.cap_prologue();

  // limits
  uint16_t prof = ring.get_profile();
  uint16_t cnt  = prof/2;
  uint16_t left = cnt+(prof & 1);

  // offsets
  uint16_t i    = 0;
  uint16_t j    = prof-1;

  // ^iter
  this->new_face();
  for(;i<cnt;i++,j--) {

    this->push_tri(ring,ring,i,j,true);
    left--;

    if(left) {
      this->push_tri(ring,ring,j-1,i+1,true);
      left--;

    };

  };

  // ^calc vertex normal inclination
  //
  // TODO: copy normals to hax verts
  //       if smooth shading

  float sign = (up) ? -1 : 1;
  float ang  = sign*(Seph::PI/4);

  for(auto& vert : ring.get_cap_verts()) {

    T3D::Facing dirn=vert.n;
    quat rot=T3D::qang(ang,dirn.hax);

    // ^apply rotation to normal
    vert.n=vert.n*rot;

  };

  // mark for update
  m_cache.calc_indices=true;
  m_cache.calc_deforms=true;

};

// ---   *   ---   *   ---
// extend mesh from ring

svec<uint16_t> Modeler::extrude(

  uint16_t beg,
  uint16_t cuts,

  float    len,
  bool     in

) {

  svec<uint16_t> out;
  out.resize(cuts+1);

  out[0]=beg;

  // get guide ring
  // clear face if present
  auto& ring  = m_rings[beg];
  bool  recap = ring.uncap();

  // ^get settings
  uint16_t base   = ring.get_top();
  uint16_t prof   = ring.get_profile();
  float    radius = ring.get_radius();

  // get movement vector for extrusion
  float sign=(in) ? 1 : -1;
  vec3  mvec={0,sign*(len/cuts),0};

  // ^make successive extrusions
  for(uint16_t i=1;i<cuts+1;i++) {

    // create new ring and push in/out
    uint16_t cut=this->new_ring();
    m_rings[cut].set_base(base);
    m_rings[cut].set_radius(radius);
    m_rings[cut].set_profile(prof);
    m_rings[cut].get_xform().move(mvec);

    // move to next
    base   += prof+1;
    mvec   += mvec;

    out[i]  = cut;

  };

  // ^join rings
  for(uint16_t i=0;i<cuts;i++) {
    m_join_q.push_back(out[i+0]);
    m_join_q.push_back(out[i+1]);

  };

  // add cap to end if beg was capped
  if(recap) {
    this->cap(out.back());

  };

  out.erase(out.begin()+0);
  return out;

};

// ---   *   ---   *   ---
// add inwards ring from other

svec<uint16_t> Modeler::inset(

  uint16_t beg,
  uint16_t cuts,

  float    dist

) {

  svec<uint16_t> out=this->extrude(
    beg,cuts,0,true

  );

  float step   = dist/cuts;
  float radius =
    m_rings[beg].get_radius()
  - step
  ;

  for(uint16_t i=0;i<cuts;i++) {
    m_rings[out[i]].set_radius(radius);
    radius-=step;

  };

  return out;

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
// run join/cap ops in Q

void Modeler::calc_faces(void) {

  for(uint16_t i=0;i<m_join_q.size();i+=2) {
    this->join(m_join_q[i+0],m_join_q[i+1]);

  };

  m_join_q.clear();
  m_mesh.indices.resize(this->get_icount());

};

// ---   *   ---   *   ---
// get vertex indices for drawing
//
// TODO: duplicate 'flat' vertices

void Modeler::calc_indices(void) {

  uint16_t i=0;
  uint16_t j=1;

  if(m_join_q.size()) {
    this->calc_faces();

  };

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

    for(auto& ring : m_rings) {
      m_vcount+=ring.get_hax_verts().size();
      m_vcount+=ring.get_cap_verts().size();

    };

  };

  return m_vcount;

};

// ---   *   ---   *   ---
// cache a copy of each elements
// verts with transforms applied

void Modeler::calc_deforms(void) {

  uint16_t row      = 0;
  uint16_t num_rows = m_rings.size();

  for(auto& ring : m_rings) {

    T3D&     xform = ring.get_xform();
    uint16_t prof  = ring.get_profile();

    uint16_t i     = 0;

    for(auto& vert : ring.get_hax_verts()) {

      m_deformed[vert.idex]=Vertex(

        xform.point_model(vert.co),
        xform.point_nmat(vert.n),

        vert.idex

      );

      m_deformed[vert.idex].uv=vec2(

        float(i)/prof,

        xform.position().y
      + (float(row)/num_rows)

      );

      i++;

    };

    row++;

    for(auto& vert : ring.get_cap_verts()) {

      m_deformed[vert.idex]=Vertex(

        xform.point_model(vert.co),
        xform.point_nmat(vert.n),

        vert.idex

      );

      m_deformed[vert.idex].uv=
        vert.uv_for_cap();

    };

  };

};

// ---   *   ---   *   ---
// generate texcords for shape
//
// NOTE: placeholder

void Modeler::calc_uvs(void) {

};

// ---   *   ---   *   ---
// ^get mesh requires recalc

bool Modeler::get_updated(void) {

  // double check each element
  for(auto& ring : m_rings) {

    // feed element update back onto frame
    m_cache.calc_indices |= ring.updated;
    m_cache.calc_deforms |=
      ring.get_xform().get_updated();

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

    auto& dst=m_mesh.verts[vert.idex];

    dst.set_xyz(vert.co);
    dst.set_uv(vert.uv);

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
