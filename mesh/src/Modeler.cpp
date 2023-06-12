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

  // get surface point
  vec3 fwd = xform.fwd();
  quat rot = {1,0,Seph::PI2/prof,0};

  // ^rotate around self
  for(uint16_t i=0;i<prof;i++) {

    m_verts.push_back(Vertex(
      fwd * m_radius,fwd,m_base+i

    ));

    fwd=fwd*rot;

  };

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
  Modeler::Joint& a,
  Modeler::Joint& b

) {

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

void Modeler::join_quad(

  Modeler::Joint& a,
  Modeler::Joint& b,

  uint16_t ai,
  uint16_t bi

) {

  // tri 0 (0,1,2)
  m_vrefs.push_back(a.vof(ai+0));
  m_vrefs.push_back(a.vof(ai+1));
  m_vrefs.push_back(b.vof(bi+1));

  // tri 1 (0,2,3)
  m_vrefs.push_back(a.vof(ai+0));
  m_vrefs.push_back(b.vof(bi+1));
  m_vrefs.push_back(b.vof(bi+0));

};

// ---   *   ---   *   ---
// ^way more awkward

void Modeler::join_tri(

  Modeler::Joint& a,
  Modeler::Joint& b,

  uint16_t ai,
  uint16_t bi

) {

  m_vrefs.push_back(a.vof(ai+0));
  m_vrefs.push_back(a.vof(ai+1));
  m_vrefs.push_back(b.vof(bi+0));

};

// ---   *   ---   *   ---
// evenly distributed tris

void Modeler::wind_even(
  Modeler::Joint& a,
  Modeler::Joint& b

) {

  for(uint16_t i=0;i<a.get_profile();i++) {
    this->join_quad(a,b,i,i);

  };

};

// ---   *   ---   *   ---
// ^utter chaos

void Modeler::wind_uneven(
  Modeler::Joint& a,
  Modeler::Joint& b

) {

  uint16_t ai    = 0;
  uint16_t bi    = 1;

  uint16_t align = 0;
  uint16_t tresh = 

  svec<uint16_t> anchors;

  for(uint16_t i=0;i<a.get_profile();i++) {

    this->join_tri(a,b,ai,bi);

    align++;
    ai++;

    if(align==tresh) {

      anchors.push_back(ai);

      align=0;
      bi++;

    };

  };

};

// ---   *   ---   *   ---
