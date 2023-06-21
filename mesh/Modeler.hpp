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

  VERSION   "v0.00.8b";
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

    vec2     uv;

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

    // calc uv coords for a cap
    // set of triangles
    inline vec2 uv_for_cap(void) {
      return {this->co.x,this->co.z};

    };

  };

  typedef svec<Vertex>       Verts;
  typedef svec<sref<Vertex>> Verts_Ref;

  typedef svec<Verts_Ref>    Faces;

// ---   *   ---   *   ---
// ^vertex ring

  struct Ring {

  private:

    T3D      m_xform;

    Verts    m_cap_verts;
    Verts    m_hax_verts;

    float    m_radius      = 0.5f;

    uint16_t m_profile     = QUAD;
    uint16_t m_base        = 0;

    uint16_t m_inset_mask  = 0x00;
    uint16_t m_inset_depth = 0x00;

    bool     m_capped      = false;

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

    inline uint16_t get_top(void) {
      return (m_capped)
        ? m_base+m_profile+1+m_profile
        : m_base+m_profile+1
        ;

    };

    void set_radius(float r);
    float get_radius(void) {
      return m_radius;

    };

    inline uint16_t hiof(uint16_t i) {
      return i % m_hax_verts.size();

    };

    inline uint16_t ciof(uint16_t i) {
      return i % m_cap_verts.size();

    };

    inline Vertex& hvof(uint16_t i) {
      return m_hax_verts[this->hiof(i)];

    };

    inline Vertex& cvof(uint16_t i) {
      return m_cap_verts[this->ciof(i)];

    };

    inline Verts& get_hax_verts(void) {
      return m_hax_verts;

    };

    inline Verts& get_cap_verts(void) {
      return m_cap_verts;

    };

    inline T3D& get_xform(void) {
      return m_xform;

    };

    // resize and populate container
    // for split top/bottom vertices
    void cap_prologue(void);

    // ^undo
    bool uncap(void);

    bool updated=false;

  };

  typedef svec<Ring> Rings;

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

  Rings       m_rings;
  Faces       m_faces;

  Verts       m_deformed;

  Cache       m_cache;

  uint16_t    m_vcount=0;
  uint16_t    m_icount=0;

  svec<uint16_t> m_join_q;

// ---   *   ---   *   ---
// guts

  // forks from join; these are the Fs
  // that det tri indices
  void wind_even(Ring& a,Ring& b);
  void wind_uneven(Ring& a,Ring& b);

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

    Ring&   a,
    Ring&   b,

    uint16_t ai,
    uint16_t bi,

    bool     cap=false

  );

  // ^single
  void push_tri(

    Ring&   a,
    Ring&   b,

    uint16_t ai,
    uint16_t bi,

    bool     cap=false

  );

  // paste-in generated code
  #include "mesh/Modeler/Aux.hpp"

  // run join/cap ops in Q
  void calc_faces(void);

  // fetch tri indices for drawing
  void calc_indices(void);

  // cache a copy of each elements
  // verts with transforms applied
  void calc_deforms(void);

  // generate texcords for shape
  void calc_uvs(void);

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
  inline uint16_t new_ring(void) {
    uint16_t out=m_rings.size();
    m_rings.push_back(Ring());

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

  // ^undo
  inline void uncap(uint16_t idex) {
    m_rings[idex].uncap();

  };

  // extend mesh from ring
  svec<uint16_t> extrude(

    uint16_t beg,
    uint16_t cuts,

    float    len,
    bool     in=false

  );

  // add inwards ring from other
  svec<uint16_t> inset(

    uint16_t beg,
    uint16_t cuts,

    float    dist

  );

  // get handle to element
  inline Ring& ring(uint16_t idex) {
    return m_rings[idex];

  };

  // get packed version of mesh for drawing
  CRK::Prim& get_packed(void);

  // get ref to array of vertex references
  inline Faces& get_faces(void) {
    return m_faces;

  };

  // get ref to array of deformed verts
  inline Verts& get_deformed(void) {
    this->calc_mesh();
    return m_deformed;

  };

  // ^get range of array corresponding to
  // deformed copies of an elements verts
  inline svec<uint16_t> get_ring_dverts(
    uint16_t id

  ) {

    auto& ring=m_rings[id];

    uint16_t beg=ring.get_base();
    uint16_t end=ring.get_hax_verts().size()-1;

    return {beg,uint16_t(beg+end)};

  };

  // get recalcs needed
  bool get_updated(void);

};

// ---   *   ---   *   ---

#endif // __5E_MODELER_H__
