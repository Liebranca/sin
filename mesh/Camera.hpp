#ifndef __24_CAMERA_H__
#define __24_CAMERA_H__

// ---   *   ---   *   ---
// deps

  #include <cmath>

  #include <glm/glm.hpp>
  #include <glm/gtx/transform.hpp>

  #include "bitter/kvrnel/Style.hpp"

//#include "SIN_Constants.h"
//#include "GAOL_Frustum.h"
//
//#include "../DA_CommonTypes.h"

// ---   *   ---   *   ---
// info

class Camera {

public:

  VERSION   "v2.00.1";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// helper

  typedef struct {

    float width;
    float height;

    float scale;

    float near;
    float far;

// ---   *   ---   *   ---

    glm::mat4 ortho(void) {

      width *= 0.5f;
      width *= scale;

      height *= 0.5f;
      height *= scale;

      return glm::ortho(
        -width,width,
        -height,height,

        near,far

      );

    };

    glm::mat4 persp(void) {
      return glm::perspective(
        glm::radians(scale),

        width/height,
        near,far

      );

    };

  } Lens;

// ---   *   ---   *   ---

  Camera(
    const glm::vec3& pos,
    Camera::Lens&    lens

  );

  ~Camera(void);

// ---   *   ---   *   ---

  inline glm::mat4
  get_view(void) const {

    return glm::lookAt(
      m_pos,m_pos+m_fwd,Y_AXIS

    )*m_proj;

  };

  inline glm::vec3 get_eye(void) {
    return glm::normalize(m_pos+m_fwd);

  };

// ---   *   ---   *   ---
// GAOLER...
//
//  void updateFrustum(void);
//  void onAreaChange(void);
//
//  void cellCulling(
//    uint32_t num_cells,
//    viewFrustum* lightFrustums[SIN_MAX_LAMPS],
//
//    uint32_t num_dirlights
//
//  );
//
//  void resetCulling(void);
//
//  int  sphInFrustum(COLSPHERE* sph);
//  bool pointsInFrustum(
//    glm::vec3* points,
//    uint32_t num_points
//
//  );
//
//  bool pointInFrustum(glm::vec3& point);
//  bool cageInFrustum(COLBOX* box);
//  int frustumIsect(viewFrustum* other);

  inline void use_ortho(void) {
    m_ortho = true;
    m_proj  = m_lens.ortho();

  };

  inline void use_persp(void) {
    m_ortho = false;
    m_proj  = m_lens.persp();

  };

  void move(glm::vec3& mvec,bool local=false);
  void rotate(glm::vec3& rvec);

// ---   *   ---   *   ---
// getters

  inline Camera::Lens& get_lens(void) {
    return m_lens;

  };

  inline float get_pitch(void) {
    return m_pitch;

  };

  inline glm::vec3& get_fwd(void) {
    return m_fwd;

  };

  inline glm::vec3& get_pos(void) {
    return m_pos;

  };

  inline glm::vec3& get_up(void) {
    return m_up;

  };

// ---   *   ---   *   ---

  inline glm::vec3 get_fwd_cast(
    float dist=3.5f

  ) {return m_pos+(m_fwd*dist);};

  inline void snapTo(glm::vec3& newpos) {
    m_altpos=m_pos;
    m_pos=newpos;

  };

  inline void undoSnap(void) {
    m_pos=m_altpos;

  };

  inline bool& getUpdate(void) {
    return m_update;

  };

  inline void endUpdate(void) {
    m_update=false;

  };

// ---   *   ---   *   ---
// G A O L E R !!!
//
//  inline uint32_t& getGridpos(void) {
//    return m_curcell.gridpos;
//
//  };
//
//  inline int& getCellpos(void) {
//    return m_curcell.worldpos;
//
//  };
//
//  inline DAGCI* getCellPositions(void) {
//    return m_nearcells;
//
//  };
//
//  viewFrustum* getFrustum(void) {
//    return m_frustum;
//
//  };

// ---   *   ---   *   ---

private:

  glm::vec3    m_pos;
  glm::vec3    m_fwd;
  glm::vec3    m_up;
  glm::vec3    m_altpos;

  glm::mat4    m_proj;

  float        m_pitch  = 0.0f;
  float        m_yaw    = 0.0f;

  bool         m_update = true;
  bool         m_ortho  = false;

  Camera::Lens m_lens;

// ---   *   ---   *   ---
// I need Gaoler for this stuff...
//
//  viewFrustum* m_frustum;

//  DAGCI        m_curcell         = { 0 };
//  DAGCI*       m_nearcells       = 0;
//  uint32_t     m_prevframe_cells = 0;

};

// ---   *   ---   *   ---

#endif // __24_CAMERA_H__
