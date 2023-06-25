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

  VERSION   "v0.01.4b";
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
    vec3     t;

    vec2     uv;

    float    ao=0.5f;
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
      return glm::normalize(vec2(
        {this->co.x,this->co.z}

      ));

    };

    // clamp occlusion value and
    // map it to 0..255 range
    inline uint8_t clamp_ao(void) {

      ao=(ao > 1.0f)
        ? 1.0f
        : ao * (ao > 0.0f)
        ;

      return uint8_t(ao*255);

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
    Verts    m_verts;

    float    m_radius      = 0.5f;

    uint16_t m_profile     = QUAD;
    uint16_t m_base        = 0;

    bool     m_capped      = false;

// ---   *   ---   *   ---

  public:

    inline Ring& operator=(Ring& other) {

      m_xform   = other.m_xform;

      m_verts   = other.m_verts;

      m_radius  = other.m_radius;

      m_profile = other.m_profile;
      m_base    = other.m_base;

      m_capped  = other.m_capped;

      return *this;

    };

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
        ? m_base+m_profile
        : m_base+m_profile+1
        ;

    };

    void set_radius(float r);
    float get_radius(void) {
      return m_radius;

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

    inline void set_xform(T3D xform) {
      m_xform=xform;

    };

    // remove additional vertex
    // cap uvs do not need it
    void cap_prologue(

      float dx    = 0.0f,
      float dy    = 0.0f,

      float scale = 1.0f

    );

    // ^undo
    bool uncap(void);

    bool updated=false;

    // darkens or brightens verts
    // affects AO calc in shader
    void occlude(float fac);

  };

  typedef svec<Ring> Rings;

// ---   *   ---   *   ---
// texcord unwrap helpers

  struct UV_Row {

    svec<uint16_t> indices;

    float height=1.0f;

  };

  typedef svec<UV_Row> UV_Rows;

  struct UV_Island {

    UV_Rows rows;

    vec2    displace;
    float   scale;

  };

  typedef svec<UV_Island> UV_Map;

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
  UV_Map      m_uv_map;

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
    uint16_t bi

  );

  // ^single
  void push_tri(

    Ring&   a,
    Ring&   b,

    uint16_t ai,
    uint16_t bi

  );

  // add row to current uv island
  void push_uv_row(
    Ring& ring,
    float h=1.0f

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

  // annoying math for normal maps
  void calc_tangents(void);

  // ^wraps over update routines
  void calc_mesh(void);

  // ^takes deformed mesh data and
  // packs it to crk format
  void pack(void);

  // get size of internal arrays
  // has been updated
  uint16_t get_vcount(void);
  uint16_t get_icount(void);

  // get base value of last rings
  inline uint16_t get_top(void) {
    return m_rings.back().get_top();

  };

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

  // ^new from existing
  inline uint16_t new_ring(uint16_t src) {

    uint16_t base = this->get_top();
    uint16_t out  = this->new_ring();

    m_rings[out]=m_rings[src];
    m_rings[out].set_base(base);

    return out;

  };

  // make tris between two elements
  void join(
    uint16_t idex_a,
    uint16_t idex_b

  );

  // ^create new ring that forms
  // a face within itself
  uint16_t cap(

    uint16_t idex,

    bool     up    = true,

    float    dx    = 0.0f,
    float    dy    = 0.0f,

    float    scale = 1.0f

  );

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

  // rotate vertex normals of ring
  void nrot(
    uint16_t idex,
    float    f

  );

  // get handle to element
  inline Ring& ring(uint16_t idex) {
    return m_rings[idex];

  };

  // make new uv island
  inline void uv_cut(

    float dx    = 0.0f,
    float dy    = 0.0f,

    float scale = 1.0f

  ) {

    m_uv_map.push_back(UV_Island());
    auto& island=m_uv_map.back();

    island.displace = vec2({dx,dy});
    island.scale    = scale;

  };

  // add row to current uv island
  inline void push_uv_row(uint16_t idex) {
    this->push_uv_row(m_rings[idex]);

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
    uint16_t end=ring.get_verts().size()-1;

    return {beg,uint16_t(beg+end)};

  };

  // get recalcs needed
  bool get_updated(void);

};

// ---   *   ---   *   ---

#endif // __5E_MODELER_H__
