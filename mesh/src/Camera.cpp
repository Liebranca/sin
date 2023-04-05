// ---   *   ---   *   ---
// CAMERA
// Looks your way
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTOS
// lyeb,

// ---   *   ---   *   ---
// deps

//  #include "DA_WorldManager.h"
//  #include "DA_Occluder.h"
//
//  #include "lyutils/ZJC_Evil.h"
//  #include "lymath/ZJC_GOPS.h"
//  #include "../Chasm/wind/chMANG.h"

  #include "mesh/Camera.hpp"

// ---   *   ---   *   ---
// fetch or regenerate view matrix

glm::mat4& Camera::get_view(void) {

  if(m_update) {
    m_view   = this->calc_view();
    m_update = false;

  };

  return m_view;

};

// ---   *   ---   *   ---

Camera::Camera(
  const glm::vec3& pos,
  Camera::Lens&    lens

) {

  m_pos  = pos;

  m_fwd  = glm::vec3(0,0,-1);
  m_up   = Y_AXIS;

  m_lens = lens;

// ---   *   ---   *   ---
// gaoler...
//
//  frustum       = new viewFrustum(
//    glm::vec2(width, height),
//    fov,
//
//    0,SIN_ORTHOSCALE,
//
//    zNear,zFar
//
//  );
//
//  DA_grid_findpos(curcell.worldpos, fpos);
//  DA_grid_findabspos(
//    curcell.gridpos,
//    curcell.worldpos
//
//  );

}

// ---   *   ---   *   ---

Camera::~Camera(void) {
//  WARD_EVIL_MFREE(nearcells); delete frustum;

};

// ---   *   ---   *   ---
// gaoler...
//
//void Camera::updateFrustum(void) {
//  frustum->buildPlanes(m_pos,m_fwd,m_up);
//
//}
//
//void Camera::onAreaChange(void) {
//
//  WARD_EVIL_MFREE(nearcells);
//  nearcells=(DAGCI*) evil_malloc(
//    DA_grid_getFrustumFac(), sizeof(DAGCI)
//
//  );
//
//};
//
//void Camera::cellCulling(
//  uint32_t num_cells,
//
//  viewFrustum* lightFrustums[SIN_MAX_LAMPS],
//  uint32_t num_dirlights
//
//) {
//
//  int h=DA_CELL_HALF;
//  int w=DA_CELL_HALF;
//
//  for(uint32_t i=0;i<num_cells;i++) {
//
//    int ox=(
//      nearcells[i].worldpos[0]
//    * DA_CELL_SIZE
//
//    );
//
//    int oy=(
//      nearcells[i].worldpos[1]
//    * DA_CELL_SIZE
//
//    );
//
//    int oz=(
//      nearcells[i].worldpos[2]
//    * DA_CELL_SIZE
//
//    );
//
//    glm::vec3 origin(ox, oy, oz);
//
//    glm::vec3 cellbounds[8]={
//      origin+glm::vec3(w,-h,-w),
//      origin+glm::vec3(w,h,-w),
//      origin+glm::vec3(w,h,w),
//      origin+glm::vec3(w,-h,w),
//      origin+glm::vec3(-w,-h,-w),
//      origin+glm::vec3(-w,h,-w),
//      origin+glm::vec3(-w,h,w),
//      origin+glm::vec3(-w,-h,w)
//
//    };
//
//    COLSPHERE cellsphere(origin, w );
//    COLBOX    cellbox(cellbounds);
//
//    DA_grid_setInFrustum(
//      nearcells[i].gridpos,
//      frustum->cellInside(&cellsphere, &cellbox)
//
//    );
//
//    for(uint32_t j=0;j<num_dirlights;j++) {
//      DA_grid_setInLightFrustum(
//        nearcells[i].gridpos,j,
//        lightFrustums[j]->cellInside(
//          &cellsphere, &cellbox)
//
//      );
//
//    };
//
//  };
//
//  prevframe_cells=num_cells;
//
//};
//
//void Camera::resetCulling(void) {
//  for(uint32_t i=0;i<prevframe_cells;i++) {
//    DA_grid_setInFrustum(nearcells[i].gridpos, 0);
//
//  };
//
//};
//
//bool Camera::cageInFrustum(COLBOX* box) {
//  return frustum->boxIsect(box);
//
//};
//
//int Camera::sphInFrustum(COLSPHERE* sph) {
//  return frustum->sphereIsect(sph);
//
//};
//
//bool Camera::pointsInFrustum(
//  glm::vec3* points,
//  uint32_t   num_points
//
//) {
//
//  return frustum->pointsInside(points,num_points);
//
//};
//
//bool Camera::pointInFrustum(
//  glm::vec3& point
//
//) {
//
//  return frustum->pointInside(point);
//
//};
//
//int Camera::frustumIsect(viewFrustum* other) {
//  return frustum->frustumIsect(other);
//
//};

// ---   *   ---   *   ---

void Camera::move(
  glm::vec3& mvec,
  bool local

) {

  m_update           = true;
  glm::vec3 displace = { 0,0,0 };

  if(local) {
    float xfac=fabs(m_up.x);
    if(m_fwd.x<0) {xfac*=-1;};

    float zfac=fabs(m_up.z);
    if(m_fwd.z<0) {zfac*=-1;};

    glm::vec3 noup={
      m_fwd.x+xfac,
      0,
      m_fwd.z+zfac

    };

    if(mvec[2]) {displace+=noup*mvec[2];};

    if(mvec[0]) {
      displace-=
        glm::cross(noup,Y_AXIS)
      * mvec[0]

      ;

    };

    displace.y=mvec.y;

  } else {displace=mvec;};

// ---   *   ---   *   ---

  m_pos+=displace;

// gaoler...
//  float fpos[3]={pos.x, pos.y, pos.z};
//
//  DA_grid_findpos(curcell.worldpos,fpos);
//  DA_grid_findabspos(
//    curcell.gridpos,
//    curcell.worldpos
//
//  );

};

// ---   *   ---   *   ---

void Camera::rotate(glm::vec3& rvec) {

  m_update=true;

  m_pitch+=rvec.y;
  m_yaw+=rvec.x;

// ---   *   ---   *   ---
// clamp like a mother
//
// I deadass got these magic numbers
// entirely through trial and error

  if(m_pitch<-1.49f) {
    m_pitch=-1.49f;

  } else if(m_pitch>1.49f) {
    m_pitch=1.49f;

  };

// ---   *   ---   *   ---
// it just works...

  glm::vec3 hAxis = glm::normalize(
    glm::cross(Y_AXIS,m_fwd)

  );

  m_fwd.x         = sin(m_yaw)*cos(m_pitch);
  m_fwd.z         = cos(m_yaw)*cos(m_pitch);
  m_fwd.y         = sin(m_pitch);

  m_fwd           = glm::normalize(m_fwd);
  m_up            = glm::normalize(
    glm::cross(m_fwd,hAxis)

  );

};

// ---   *   ---   *   ---
