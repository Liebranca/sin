#ifndef __5E_MODELER_H__
#define __5E_MODELER_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/GLM.hpp"
  #include "bitter/kvrnel/T3D.hpp"
  #include "bitter/ff/CRK.hpp"

// ---   *   ---   *   ---
// info

class Modeler {

public:

  VERSION   "v0.00.6b";
  AUTHOR    "IBN-3DILA";

  enum {

    TRIANGLE   = 3,
    QUAD       = 4,

    PENTAGON   = 5,
    HEXAGON    = 6,
    HEPTAGON   = 7,

    OCTAGON    = 8,
    NONAGON    = 9,
    DECAGON    = 10,

    HENDECAGON = 11

  };

// ---   *   ---   *   ---
// helpers

  struct Vertex {

    vec3     co;
    vec3     n;

    uint16_t idex;

    // cstruc
    Vertex(

      vec3     _co,
      vec3     _n,

      uint16_t _idex

    ):

      co   (_co),
      n    (_n),

      idex (_idex)

    {};

    // ^ctrash
    Vertex(void) {};

  };

  typedef svec<Vertex>       Verts;
  typedef svec<sref<Vertex>> Verts_Ref;

  typedef svec<Verts_Ref>    Faces;

// ---   *   ---   *   ---
// ^vertex ring

  struct Joint {

  private:

    T3D      m_xform;

    Verts    m_verts;

    float    m_radius      = 0.5f;

    uint16_t m_profile     = QUAD;
    uint16_t m_base        = 0;

    uint16_t m_inset_mask  = 0x00;
    uint16_t m_inset_depth = 0x00;

// ---   *   ---   *   ---

  public:

    void set_profile(uint16_t prof);
    inline uint16_t get_profile(void) {
      return m_profile;

    };

    void set_base(uint16_t base);
    inline uint16_t get_base(void) {
      return m_base;

    };

    inline uint16_t iof(uint16_t i) {
      return i % m_verts.size();

    };

    inline Vertex& vof(uint16_t i) {
      return m_verts[this->iof(i)];

    };

    inline Verts& get_verts(void) {
      return m_verts;

    };

    inline T3D& get_xform(void) {
      return m_xform;

    };

    bool updated=false;

  };

  typedef svec<Joint> Joints;

// ---   *   ---   *   ---
// keeps track of internal state

  struct Cache {

    bool calc_indices = true;
    bool calc_deforms = true;

    bool repack       = true;

  };

// ---   *   ---   *   ---
// attrs

private:

  CRK::Prim   m_mesh;

  Joints      m_joints;
  Faces       m_faces;

  Verts       m_deformed;

  Cache       m_cache;

  uint16_t    m_vcount=0;
  uint16_t    m_icount=0;

// ---   *   ---   *   ---
// guts

  // forks from join; these are the Fs
  // that det tri indices
  void wind_even(Joint& a,Joint& b);
  void wind_uneven(Joint& a,Joint& b);

  // make new face for pushing tris
  Verts_Ref& new_face(void) {
    m_faces.push_back(Verts_Ref());
    return m_faces.back();

  };

  // ^get last created
  inline Verts_Ref& cur_face(void) {
    return m_faces.back();

  };

  // ^add two tris
  void push_quad(

    Joint&   a,
    Joint&   b,

    uint16_t ai,
    uint16_t bi

  );

  // ^single
  void push_tri(

    Joint&   a,
    Joint&   b,

    uint16_t ai,
    uint16_t bi

  );

  // paste-in generated code
  #include "mesh/Modeler/Aux.hpp"

  // fetch tri indices for drawing
  void calc_indices(void);

  // cache a copy of each elements
  // verts with transforms applied
  void calc_deforms(void);

  // ^wraps over update routines
  void calc_mesh(void);

  // ^takes deformed mesh data and
  // packs it to crk format
  void pack(void);

  // get size of internal arrays
  // has been updated
  uint16_t get_vcount(void);
  uint16_t get_icount(void);

// ---   *   ---   *   ---
// iface

public:

  // ctrash
  Modeler(void) {};

  // create new element
  inline uint16_t new_joint(void) {

    uint16_t out=m_joints.size();
    m_joints.push_back(Joint());

    return out;

  };

  // make tris between two elements
  void join(
    uint16_t idex_a,
    uint16_t idex_b

  );

  // make tris within an elements
  // own verts
  void cap(uint16_t idex,bool up=true);

  // get handle to element
  inline Joint& joint(uint16_t idex) {
    return m_joints[idex];

  };

  // get packed version of mesh for drawing
  CRK::Prim& get_packed(void);

  // get ref to array of vertex references
  inline Faces& get_faces(void) {
    return m_faces;

  };

  // get ref to array of deformed verts
  inline Verts& get_deformed(void) {
    return m_deformed;

  };

  // ^get range of array corresponding to
  // deformed copies of an elements verts
  inline svec<uint16_t> get_joint_dverts(
    uint16_t id

  ) {

    auto& joint=m_joints[id];

    uint16_t beg=joint.get_base();
    uint16_t end=joint.get_verts().size();

    return {beg,uint16_t(beg+end)};

  };

  // get recalcs needed
  bool get_updated(void);

};

// ---   *   ---   *   ---

#endif // __5E_MODELER_H__
