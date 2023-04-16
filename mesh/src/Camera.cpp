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

  #include <GL/glew.h>
  #include "mesh/Camera.hpp"

// ---   *   ---   *   ---
// fetch or regenerate view matrix

glm::mat4& Camera::get_view(void) {

  if(m_update) {
    m_view   = this->calc_view();
    m_update = false;

    this->update_frustum();
    this->update_ubo(1);

  };

  return m_view;

};

// ---   *   ---   *   ---
// for uploading mats to shader

void Camera::nit_ubo(uint32_t idex) {

  glGenBuffers(1,&m_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER,m_ubo);

  glBufferData(
    GL_UNIFORM_BUFFER,

    sizeof(glm::mat4)*2,
    NULL,

    GL_DYNAMIC_DRAW

  );

  glBindBufferBase(
    GL_UNIFORM_BUFFER,

    idex,
    m_ubo

  );

};

// ---   *   ---   *   ---
// ^send

void Camera::update_ubo(bool which) {

  uint64_t offset=(which)
    ? sizeof(glm::mat4)
    : 0
    ;

  auto& data=(which)
    ? m_view
    : m_proj
    ;

  glBindBuffer(GL_UNIFORM_BUFFER,m_ubo);
  glBufferSubData(
    GL_UNIFORM_BUFFER,

    offset,
    sizeof(data),

    (void*) &data[0][0]

  );

};

// ---   *   ---   *   ---
// cstruc

void Camera::nit(
  const glm::vec3& pos,
  Camera::Lens&    lens,

  uint32_t         bind_idex

) {

  m_pos    = pos;

  m_fwd    = glm::vec3(0,0,-1);
  m_up     = Y_AXIS;

  m_lens   = lens;

  this->nit_ubo(bind_idex);

  m_frustum.set(

    lens.width,
    lens.height,

    lens.fov,
    lens.near,
    lens.far

  );

}

// ---   *   ---   *   ---

Camera::~Camera(void) {

  if(m_ubo) {
    glDeleteBuffers(1,&m_ubo);

  };

};

// ---   *   ---   *   ---
// gaoler...

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
