#ifndef __5E_CAMERA_H__
#define __5E_CAMERA_H__

// ---   *   ---   *   ---
// deps

  #include <cmath>

  #include <glm/glm.hpp>
  #include <glm/gtx/transform.hpp>

  #include "bitter/kvrnel/Style.hpp"
  #include "gaoler/Frustum.hpp"

  #include "world/Node.hpp"

// ---   *   ---   *   ---
// info

class Camera : public Node {

public:

  VERSION   "v2.00.6";
  AUTHOR    "IBN-3DILA";

// ---   *   ---   *   ---
// helpers

  typedef struct {

    float width;
    float height;

    float scale;
    float fov;

    float near;
    float far;

// ---   *   ---   *   ---

    glm::mat4 ortho(void) {

      float w=width;
      float h=height;

      w *= 0.5f;
      w *= scale;

      h *= 0.5f;
      h *= -scale;

      return glm::ortho(
        -w,w,-h,h,
        near,far

      );

    };

    glm::mat4 persp(void) {

      float w=width;
      float h=height;

      w *= scale;
      h *= -scale;

      return glm::perspective(

        glm::radians(-fov),w/h,
        near,far

      );

    };

  } Lens;

// ---   *   ---   *   ---
// attrs

private:

  glm::mat4     m_view;
  glm::mat4     m_proj;
  glm::mat4     m_stow;

  Camera::Lens  m_lens;
  Gaol::Frustum m_frustum;

  uint32_t      m_ubo     = 0;
  bool          m_ortho   = false;

// ---   *   ---   *   ---
// guts

  // for VP to shader
  void nit_ubo(uint32_t loc);
  void update_ubo(bool which);

  // regenerate view matrix
  inline glm::mat4 calc_view(void) {
    return glm::lookAt(

      this->get_pos(),

      this->get_pos()
    + this->get_fwd(),

      Y_AXIS

    );

  };

  // ^get screen-to-world matrix
  inline glm::mat4 calc_stow(void) {

    return
      glm::inverse(m_view)
    * glm::inverse(m_proj)
    ;

  };

  // build prism from current view
  inline void update_frustum(void) {
    m_frustum.calc_box(

      this->get_pos(),

      this->get_fwd(),
      this->get_up()

    );

  };

// ---   *   ---   *   ---
// iface

public:

  // cstruc
  void nit(
    const glm::vec3& pos,
    const glm::quat& rot,

    Camera::Lens&    lens,

    uint32_t         bind_idex

  );

  // ctrash
  Camera(void) {};
  ~Camera(void);

// ---   *   ---   *   ---

  // frustum wrappers
  // used for visibility checks
  inline int sphere_in_frustum(
    Gaol::Sphere& sphere

  ) {

    return m_frustum.isect_sphere(sphere);

  };

  inline bool point_in_frustum(
    glm::vec3& p

  ) {

    return m_frustum.isect_point(p);

  };

  inline bool points_in_frustum(
    Gaol::Points& pts

  ) {

    return m_frustum.isect_points(pts);

  };

  inline int bound_in_frustum(
    Gaol::Bound& b

  ) {

    return m_frustum.isect_bound(b);

  };

  inline bool box_in_frustum(Gaol::Box& box) {
    return m_frustum.isect_box(box);

  };

  inline int isect_frustum(Gaol::Frustum& other) {
    return m_frustum.isect_frustum(other);

  };

// ---   *   ---   *   ---

  inline void use_ortho(void) {

    m_ortho = true;
    m_proj  = m_lens.ortho();

    m_updated.visibility = true;

    this->update_ubo(0);

  };

  inline void use_persp(void) {

    m_ortho  = false;
    m_proj   = m_lens.persp();

    m_updated.visibility = true;

    this->update_ubo(0);

  };

  inline bool is_ortho(void) {
    return m_ortho;

  };

// ---   *   ---   *   ---
// getters

  inline Camera::Lens& get_lens(void) {
    return m_lens;

  };

  inline Gaol::Frustum& get_frustum(void) {
    return m_frustum;

  };

  glm::mat4& get_view(void);

  inline glm::mat4& get_stow(void) {
    return m_stow;

  };

  inline glm::mat4& get_proj(void) {
    return m_proj;

  };

  inline glm::vec3 get_eye(void) {

    return glm::normalize(
      this->get_pos()
    + this->get_fwd()

    );

  };

};

// ---   *   ---   *   ---

#endif // __5E_CAMERA_H__
