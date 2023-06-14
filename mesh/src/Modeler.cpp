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

  m_verts.clear();

  T3D   point;

  vec3 origin = {0,0,0};
  quat rot    = {1,0,Seph::PI/prof,0};

  // ^rotate around self
  for(uint16_t i=0;i<prof;i++) {

    point.teleport(origin);
    point.rotate(rot);

    point.teleport(point.fwd());

    auto& model = point.get_model();
    auto& nmat  = point.get_nmat();

    vec3  fwd   = glm::normalize(
      nmat * point.fwd()

    );

    vec3 pos    = vec3(model * vec4(
      point.position(),1

    ));

    printf(
      "%u: %f,%f,%f\n",
      i,fwd.x,fwd.y,fwd.z

    );

    m_verts.push_back(Vertex(
      fwd,fwd,m_base+i

    ));

  };

  printf("\n");

  m_profile=prof;

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

};

// ---   *   ---   *   ---
// makes tris between points of
// two joints

void Modeler::join(
  uint64_t idex_a,
  uint64_t idex_b

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
    this->push_tri(a,b,bi-1,ai);

    for(uint16_t i=0;i<cnt-1;i++) {
      this->push_tri(a,b,ai+i,bi);
      ai++;

    };

    bi++;

  };

  this->push_tri(a,b,bi-1,0);

};

// ---   *   ---   *   ---
// get vertex indices for drawing

void Modeler::calc_indices(void) {

  // TODO: duplicate 'flat' vertices
  for(auto& face : m_faces) {
    for(auto& vert : face) {
      m_mesh.indices.push_back(vert.get().idex);
//      printf("%u,",vert.get().idex);

    };

//    printf("\n");

  };

};

// ---   *   ---   *   ---
// get packed vertices for drawing

void Modeler::calc_verts(void) {

  for(auto& joint : m_joints) {

    auto& model = joint.get_xform().get_model();
    auto& nmat  = joint.get_xform().get_nmat();

    for(auto& vert : joint.get_verts()) {

      m_mesh.verts.push_back(CRK::Vertex());
      auto& dst=m_mesh.verts.back();

      vec3 co=vec3(model * vec4(

        vert.co.x,
        vert.co.y,
        vert.co.z,

        1

      ));

      vec3 n=nmat * vert.n;

      dst.set_xyz(co);
      dst.set_n(n);

    };

    printf("\n");

  };

};

// ---   *   ---   *   ---
// ^fetch data for draw buffer

CRK::Prim& Modeler::get_mesh(void) {

  if(m_cache.calc_mesh) {

    m_mesh.clear();

    this->calc_verts();
    this->calc_indices();

    m_cache.calc_mesh=false;

  };

  return m_mesh;

};

// ---   *   ---   *   ---
