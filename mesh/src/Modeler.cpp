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

  m_verts.resize(prof+1);

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

  // duplicate first vertex for uv
  m_verts.back()      = m_verts[0];
  m_verts.back().idex = m_base+prof;
  m_verts.back().n    = vec3({0,0,-1});

  // register changes
  m_profile = prof;
  m_capped  = false;

  this->updated=true;

};

// ---   *   ---   *   ---
// modify base idex for verts of ring

void Modeler::Ring::set_base(uint16_t base) {

  uint16_t i=0;
  for(auto& vert : m_verts) {
    vert.idex=base+i;
    i++;

  };

  m_base=base;
  this->updated=true;

};

// ---   *   ---   *   ---
// set rinf vertex distance from origin

void Modeler::Ring::set_radius(float r) {

  r=(r > 4.0f) ? 4.0f : r;
  r=(r < 0.01f) ? 0.01f : r;

  for(auto& vert : m_verts) {
    vec3 n=glm::normalize(vert.co);
    vert.co=n*r;

  };

  m_radius=r;
  this->updated=true;

};

// ---   *   ---   *   ---
// darkens or brightens verts
// affects AO calc in shader

void Modeler::Ring::occlude(float fac) {

  for(auto& vert : m_verts) {
    vert.ao=fac;

  };

};

// ---   *   ---   *   ---
// register vertex merge

void Modeler::Ring::glue(

  uint16_t id,

  uint16_t base,
  uint16_t cnt

) {

  m_glued.push_back(Glue());
  auto& dst=m_glued.back();

  dst.ring=id;
  dst.indices.resize(cnt);

  // mark all vertices as merged
  for(uint16_t i=base,j=0;i<base+cnt;i++) {
    dst.indices[j++]=i;

  };

};

// ---   *   ---   *   ---
// ^undo

void Modeler::Ring::unglue(uint16_t id) {

  svec<uint16_t> found;

  // get rings matching id
  uint16_t i=0;
  for(auto& glue : m_glued) {

    if(glue.ring != id) {
      found.push_back(i);

    };

    i++;

  };

  // ^pop from array
  for(auto j : found) {
    m_glued.erase(m_glued.begin()+j);

  };

};

// ---   *   ---   *   ---
// mark rings as merged

void Modeler::glue(
  uint16_t idex_a,
  uint16_t idex_b

) {

  auto& a=m_rings[idex_a];
  auto& b=m_rings[idex_b];

  a.glue(idex_b,0,b.get_verts().size());
  b.glue(idex_a,0,a.get_verts().size());

};

// ---   *   ---   *   ---
// ^propagate updates to rings
// merged with A

void Modeler::sync_glued(uint16_t id) {

  auto& a     = m_rings[id];

  auto& verts = a.get_verts();
  auto& glued = a.get_glued();

  for(auto& glue : glued) {

    auto& b=m_rings[glue.ring];
    b.set_radius(a.get_radius());

    for(auto i : glue.indices) {

      auto& dst=b.vof(i);
      auto& src=a.vof(i);

      dst.n  = src.n;
      dst.ao = src.ao;

    };

  };

};

// ---   *   ---   *   ---
// find if two faces share verts

void Modeler::get_face_nebor(
  uint16_t idex_a,
  uint16_t idex_b

) {

  svec<uint16_t> shared_a;
  svec<uint16_t> shared_b;

  auto& a   = m_faces[idex_a];
  auto& b   = m_faces[idex_b];

  // for each vert of A,
  // compare against each vert of B
  uint16_t i=0;
  for(auto& vert_a : a.verts) {

    uint16_t j=0;
    for(auto& vert_b : b.verts) {

      // ^same idex means same vertex
      // shared vertex means neighbor face
      if(

         vert_a.get().idex
      == vert_b.get().idex

      ) {

        shared_a.push_back(i);
        shared_b.push_back(j);

      };

      j++;

    };

    i++;

  };

  // ^if any shared verts found,
  // register A<->B as neighbors
  if(shared_a.size()) {

    a.nebors.push_back(
      Face_Nebor(idex_b,shared_a)

    );

    b.nebors.push_back(
      Face_Nebor(idex_a,shared_b)

    );

  };

};

// ---   *   ---   *   ---
// calc which faces share vertices

void Modeler::calc_nebor_faces(void) {

  // clear previous calc
  for(auto& face : m_faces) {
    face.nebors.clear();

  };

  // ^compare each face to each other
  for(uint16_t i=0;i < m_faces.size();i++) {
  for(uint16_t j=i+1;j < m_faces.size();j++) {
    this->get_face_nebor(i,j);

  }};

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

  uint16_t a_sz=a.get_profile();
  uint16_t b_sz=b.get_profile();

  uint16_t face_beg=m_faces.size();

  if(a_sz == b_sz) {
    this->wind_even(a,b);

  } else if(a_sz < b_sz) {
    this->wind_uneven(b,a);

  } else {
    this->wind_uneven(a,b);

  };

  for(

    uint16_t i=face_beg;

    i < m_faces.size();
    i++

  ) {

    this->set_face_rings(i,{idex_a,idex_b});

  };

  m_cache.calc_indices=true;

};

// ---   *   ---   *   ---
// default strategy

void Modeler::push_quad(

  Modeler::Ring& a,
  Modeler::Ring& b,

  uint16_t ai,
  uint16_t bi

) {

  auto& face=this->cur_face();

  // tri 0 (0,1,2)
  face.verts.push_back(a.vof(ai+0));
  face.verts.push_back(a.vof(ai+1));
  face.verts.push_back(b.vof(bi+1));

  // tri 1 (0,2,3)
  face.verts.push_back(a.vof(ai+0));
  face.verts.push_back(b.vof(bi+1));
  face.verts.push_back(b.vof(bi+0));

};

// ---   *   ---   *   ---
// ^way more awkward

void Modeler::push_tri(

  Modeler::Ring& a,
  Modeler::Ring& b,

  uint16_t ai,
  uint16_t bi

) {

  auto& face=this->cur_face();

  face.verts.push_back(a.vof(ai+0));
  face.verts.push_back(a.vof(ai+1));
  face.verts.push_back(b.vof(bi+0));

};

// ---   *   ---   *   ---
// add row to current uv island

void Modeler::push_uv_row(
  Ring& ring,
  float h

) {

  // get current island and add row
  auto& island=m_uv_map.back();
  island.rows.push_back(UV_Row());

  // fill out row with vertex indices
  auto& row=island.rows.back();

  for(auto& vert : ring.get_verts()) {
    row.indices.push_back(vert.idex);

  };

  row.height=h;

};

// ---   *   ---   *   ---
// evenly distributed tris

void Modeler::wind_even(
  Modeler::Ring& a,
  Modeler::Ring& b

) {

  for(uint16_t i=0;i<a.get_profile();i++) {
    this->new_face();
    this->push_quad(a,b,i,i);

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
// remove additional vertex
// cap uvs do not need it

void Modeler::Ring::cap_prologue(

  float dx,
  float dy,

  float scale

) {

  m_verts.erase(
    m_verts.begin()
  + m_verts.size()-1

  );

  for(auto& vert : m_verts) {

    vert.uv=
      vert.uv_for_cap()
    * scale
    + vec2({dx,dy})
    ;

  };

  m_capped=true;

};

// ---   *   ---   *   ---
// make face within ring

uint16_t Modeler::cap(

  uint16_t idex,

  bool     up,

  float    dx,
  float    dy,

  float    scale

) {

  auto  n_idex = this->new_ring(idex);
  auto& ring   = m_rings[n_idex];

  ring.cap_prologue();

  // limits
  uint16_t prof = ring.get_profile();
  uint16_t cnt  = prof/2;
  uint16_t left = cnt+(prof & 1);

  // offsets
  uint16_t i    = 0;
  uint16_t j    = prof-1;

  // ^iter
  auto& face=this->new_face();
  for(;i<cnt;i++,j--) {

    this->push_tri(ring,ring,i,j);
    left--;

    if(left) {
      this->push_tri(ring,ring,j-1,i+1);
      left--;

    };

  };

  face.rings={n_idex};

  // adjust visuals
  float sign=(up) ? 1 : -1;
  this->nrot(n_idex,8*sign);
  ring.occlude(0.5f);

  // ^merge to parent and sync
  this->glue(n_idex,idex);
  this->sync_glued(n_idex);

  // mark for update
  m_cache.calc_indices=true;
  m_cache.calc_deforms=true;

  return n_idex;

};

// ---   *   ---   *   ---
// rotate vertex normals of ring

void Modeler::nrot(
  uint16_t idex,
  float    f

) {

  auto& ring = m_rings[idex];

  float sign = (f<0) ? -1 : 1;
  float ang  = Seph::PI/fabs(f);

  // walk ring verts
  for(auto& vert : ring.get_verts()) {

    // rotate about horizontal axis
    T3D::Facing dirn=vert.n;
    quat rot=T3D::qang(ang,sign*dirn.hax);

    // ^apply rotation to normal
    vert.n=rot*vert.n;

  };

};

// ---   *   ---   *   ---
// ^flip em

void Modeler::nflip(
  uint16_t idex

) {

  auto& ring = m_rings[idex];
  for(auto& vert : ring.get_verts()) {
    vert.n=-vert.n;

  };

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
  auto& ring=m_rings[beg];

  // ^get settings
  uint16_t base   = this->get_top();
  uint16_t prof   = ring.get_profile();
  float    radius = ring.get_radius();

  T3D      origin = ring.get_xform();

  // get movement vector for extrusion
  float sign=(in) ? 1 : -1;
  float step=len/cuts;
  vec3  mvec={0,sign*step,0};

  // ^make successive extrusions
  for(uint16_t i=1;i<cuts+1;i++) {

    // create new ring
    uint16_t cut   = this->new_ring();
    auto&    cring = m_rings[cut];

    // push in/out
    cring.set_base(base);
    cring.set_radius(radius);
    cring.set_profile(prof);
    cring.set_xform(origin);
    cring.get_xform().move(mvec);
    this->push_uv_row(cring,step);

    // move to next
    base   += prof+1;
    mvec   += mvec;

    out[i]  = cut;

  };

  // ^join rings
  svec<uint16_t> order=(in)
    ? svec<uint16_t>({1,0})
    : svec<uint16_t>({0,1})
    ;

  for(uint16_t i=0;i<cuts;i++) {
    m_join_q.push_back(out[i+order[0]]);
    m_join_q.push_back(out[i+order[1]]);

  };

  // darken/lighten base
  ring.occlude(sign * 1.00f);

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

  uint16_t uv_row=m_uv_map.back().rows.size();

  svec<uint16_t> out=this->extrude(
    beg,cuts,0,false

  );

  m_rings[beg].occlude(1.0f);

  float step   = dist/cuts;
  float radius =
    m_rings[beg].get_radius()
  - step
  ;

  for(uint16_t i=0;i<cuts;i++) {

    m_uv_map.back().rows[uv_row++].height=step;

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
      m_icount+=face.verts.size();

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

  m_cache.calc_indices=true;

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
    for(auto& vert : face.verts) {
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
      m_vcount+=ring.get_verts().size();

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

    xform.full_update();

    for(auto& vert : ring.get_verts()) {

      m_deformed[vert.idex]=Vertex(

        xform.point_model(vert.co),
        xform.point_nmat(vert.n),

        vert.idex

      );

      m_deformed[vert.idex].uv=vert.uv;
      m_deformed[vert.idex].ao=vert.ao;

    };

  };

};

// ---   *   ---   *   ---
// generate texcords for shape

void Modeler::calc_uvs(void) {

  for(auto& island : m_uv_map) {

    uint16_t num_rows = island.rows.size();
    uint16_t row_i    = 0;

    for(auto& row : island.rows) {

      float y=(
        float(row_i)
      * row.height

      )/num_rows;

      uint16_t num_cols = row.indices.size();
      uint16_t col_i    = 0;

      for(auto idex : row.indices) {

        float x=float(col_i)/num_cols;

        m_deformed[idex].uv=

          vec2({x,y})

        * island.scale
        + island.displace
        ;

        col_i++;

      };

      row_i++;

    };

  };

};

// ---   *   ---   *   ---
// living nightmare

void Modeler::calc_tangents(void) {

  // walk each tri
  for(auto& face : m_faces) {

    auto& verts=face.verts;

    for(uint16_t i=0;i<verts.size();i+=3) {

      auto& a=m_deformed[verts[i+0].get().idex];
      auto& b=m_deformed[verts[i+1].get().idex];
      auto& c=m_deformed[verts[i+2].get().idex];

      // get delta
      vec3 e0=b.co-a.co;
      vec3 e1=c.co-a.co;
      vec2 d0=b.uv-a.uv;
      vec2 d1=c.uv-a.uv;

      // no idea
      float f=1.0f / (
        d0.x * d1.y
      - d1.x * d0.y

      );

      // probably cross
      a.t=glm::normalize(f*vec3({
        d1.y * e0.x - d0.y * e1.x,
        d1.y * e0.y - d0.y * e1.y,
        d1.y * e0.z - d0.y * e1.z

      }));

      // ^same for all verts of tri
      b.t=c.t=a.t;

    };

  };

};

// ---   *   ---   *   ---
// ^get mesh requires recalc

bool Modeler::get_updated(void) {

  // double check each element
  for(auto& ring : m_rings) {

    // feed element update back onto frame
    m_cache.calc_indices |= ring.updated;
    m_cache.calc_deforms |=
      ring.get_xform().get_updated()
    | ring.updated
    ;

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

    this->calc_nebor_faces();
    this->calc_indices();

    m_cache.calc_indices = false;
    m_cache.repack       = true;

  };

  if(m_cache.calc_deforms) {

    this->calc_deforms();
    this->calc_uvs();
    this->calc_tangents();

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

    dst.set_nt(vert.n,vert.t);
    dst.data[7]=vert.clamp_ao();

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
