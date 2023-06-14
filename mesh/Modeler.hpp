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

  VERSION   "v0.00.3b";
  AUTHOR    "IBN-3DILA";

  enum {

    TRIANGULAR = 3,
    CUBICAL    = 4,

    PENTAGONAL = 5,
    HEXAGONAL  = 6,
    HEPTAGONAL = 7,
    OCTAGONAL  = 8

  };

// ---   *   ---   *   ---
// helpers

  struct Vertex {

    vec3     co;
    vec3     n;

    uint16_t idex;

    Vertex(

      vec3     _co,
      vec3     _n,

      uint16_t _idex

    ):

      co   (_co),
      n    (_n),

      idex (_idex)

    {};

  };

  typedef svec<Vertex>       Verts;
  typedef svec<sref<Vertex>> Verts_Ref;

  typedef svec<Verts_Ref>    Faces;

// ---   *   ---   *   ---

  struct Joint {

  private:

    T3D      m_xform;

    Verts    m_verts;

    float    m_radius      = 1.0f;

    uint16_t m_profile     = CUBICAL;
    uint16_t m_base        = 0;

    uint16_t m_inset_mask  = 0x00;
    uint16_t m_inset_depth = 0x00;

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

  };

  typedef svec<Joint> Joints;

// ---   *   ---   *   ---

  struct Cache {
    bool calc_mesh=true;

  };

// ---   *   ---   *   ---
// attrs

private:

  CRK::Prim m_mesh;

  Joints m_joints;
  Faces  m_faces;

  Cache  m_cache;

// ---   *   ---   *   ---
// guts

  void wind_even(Joint& a,Joint& b);
  void wind_uneven(Joint& a,Joint& b);

  Verts_Ref& new_face(void) {
    m_faces.push_back(Verts_Ref());
    m_cache.calc_mesh=true;

    return m_faces.back();

  };

  inline Verts_Ref& cur_face(void) {
    return m_faces.back();

  };

  void push_quad(

    Joint&   a,
    Joint&   b,

    uint16_t ai,
    uint16_t bi

  );

  void push_tri(

    Joint&   a,
    Joint&   b,

    uint16_t ai,
    uint16_t bi

  );

  #include "mesh/Modeler/Aux.hpp"

  void calc_indices(void);
  void calc_verts(void);

// ---   *   ---   *   ---
// iface

public:

  Modeler(void) {};

  inline uint64_t new_joint(void) {

    uint64_t out=m_joints.size();
    m_joints.push_back(Joint());

    return out;

  };

  void join(
    uint64_t idex_a,
    uint64_t idex_b

  );

  inline Joint& joint(uint64_t idex) {
    return m_joints[idex];

  };

  CRK::Prim& get_mesh(void);

};

// ---   *   ---   *   ---

#endif // __5E_MODELER_H__
