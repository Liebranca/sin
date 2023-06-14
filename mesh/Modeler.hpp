#ifndef __5E_MODELER_H__
#define __5E_MODELER_H__

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/GLM.hpp"
  #include "bitter/kvrnel/T3D.hpp"

// ---   *   ---   *   ---
// info

class Modeler {

public:

  VERSION   "v0.00.2b";
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

// ---   *   ---   *   ---

  struct Joint {

  public:
    T3D xform;

  private:

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

  };

  typedef svec<Joint> Joints;

// ---   *   ---   *   ---
// attrs

private:

  Joints    m_joints;
  Verts_Ref m_vrefs;

// ---   *   ---   *   ---
// guts

  void join(Joint& a,Joint& b);

  void join_quad(

    Joint&   a,
    Joint&   b,

    uint16_t ai,
    uint16_t bi

  );

  void join_tri(

    Joint&   a,
    Joint&   b,

    uint16_t ai,
    uint16_t bi

  );

  void wind_even(Joint& a,Joint& b);
  void wind_uneven(Joint& a,Joint& b);

  #include "mesh/Modeler/Aux.hpp"

// ---   *   ---   *   ---
// iface

public:

};

// ---   *   ---   *   ---

#endif // __5E_MODELER_H__
