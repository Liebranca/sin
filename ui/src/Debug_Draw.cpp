// ---   *   ---   *   ---
// DEBUG DRAW
// For lines and points
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,

// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/T3D.hpp"
  #include "ui/Debug_Draw.hpp"

// ---   *   ---   *   ---
// cstruc

void Debug_Draw::nit(
  uint64_t size,
  uint32_t mode

) {

  if(mode==GL_LINES) {

    m_indices.resize(size*36);
    m_verts.resize(size*8);

    m_mode=GL_TRIANGLES;

  } else {

    m_indices.resize(size);
    m_verts.resize(size);

    m_mode=mode;

  };

  m_vao.nit(

    GBuff::D_ARRAY,
    GBuff::D_ELEMENT,

    sizeof(Vertex),size,
    sizeof(uint16_t),size

  );

  m_vao.vattr(

    VAO::U32_4,

    sizeof(Vertex),
    0

  );

  m_vao.unbind();

};

// ---   *   ---   *   ---
// queue point for drawing

void Debug_Draw::push_point(

  vec3&    a,

  float    scale,
  uint8_t  color,
  uint16_t flags

) {

  m_verts[m_top].set(a,scale,color,flags);
  m_indices[m_top]=m_top;

  m_top++;
  m_top_i++;

};

// ---   *   ---   *   ---
// queue line for drawing
//
// now, we don't actually do "lines",
// rather we do elongated cuboids
//
// why? because drawing lines of
// varying width is """too difficult"""
//
// effen amateurs run the industry; EHIB

void Debug_Draw::push_line(

  vec3&    a,
  vec3&    b,

  float    scale,
  uint8_t  color,
  uint16_t flags

) {

  vec3 fwd = glm::normalize(a-b);
  vec3 yax = (fabs(fwd.y) >= 0.9f)
    ? Y_VOID
    : Y_AXIS
    ;

  vec3 hax = glm::cross(yax,fwd);
  vec3 up  = glm::cross(fwd,hax);

  if(flags & DRAW_2D) {

    scale *= 0.001f;
    up    *= scale;

    vec3 br = b-up;
    vec3 bl = a-up;
    vec3 tl = a+up;
    vec3 tr = b+up;

    m_verts[m_top+0].set(br,scale,color,flags);
    m_verts[m_top+1].set(bl,scale,color,flags);
    m_verts[m_top+2].set(tl,scale,color,flags);
    m_verts[m_top+3].set(tr,scale,color,flags);

    m_indices[m_top_i+0]=m_top+0;
    m_indices[m_top_i+1]=m_top+1;
    m_indices[m_top_i+2]=m_top+2;

    m_indices[m_top_i+3]=m_top+0;
    m_indices[m_top_i+4]=m_top+2;
    m_indices[m_top_i+5]=m_top+3;

    m_top   += 4;
    m_top_i += 6;

  } else {

    scale *= 0.02f;

    hax *= scale;
    up  *= scale;

    hax *= 0.5f;
    up  *= 0.5f;

    vec3 abr = (a+hax)-up;
    vec3 abl = (a-hax)-up;
    vec3 atl = (a-hax)+up;
    vec3 atr = (a+hax)+up;

    vec3 bbr = (b+hax)-up;
    vec3 bbl = (b-hax)-up;
    vec3 btl = (b-hax)+up;
    vec3 btr = (b+hax)+up;

    m_verts[m_top+0].set(abr,scale,color,flags);
    m_verts[m_top+1].set(abl,scale,color,flags);
    m_verts[m_top+2].set(atl,scale,color,flags);
    m_verts[m_top+3].set(atr,scale,color,flags);

    m_verts[m_top+4].set(bbr,scale,color,flags);
    m_verts[m_top+5].set(bbl,scale,color,flags);
    m_verts[m_top+6].set(btl,scale,color,flags);
    m_verts[m_top+7].set(btr,scale,color,flags);

    // near
    m_indices[m_top_i +  0]=m_top+0;
    m_indices[m_top_i +  1]=m_top+1;
    m_indices[m_top_i +  2]=m_top+2;

    m_indices[m_top_i +  3]=m_top+0;
    m_indices[m_top_i +  4]=m_top+2;
    m_indices[m_top_i +  5]=m_top+3;

    // far
    m_indices[m_top_i +  6]=m_top+4;
    m_indices[m_top_i +  7]=m_top+5;
    m_indices[m_top_i +  8]=m_top+6;

    m_indices[m_top_i +  9]=m_top+4;
    m_indices[m_top_i + 10]=m_top+6;
    m_indices[m_top_i + 11]=m_top+7;

    // side A
    m_indices[m_top_i + 12]=m_top+4;
    m_indices[m_top_i + 13]=m_top+0;
    m_indices[m_top_i + 14]=m_top+3;

    m_indices[m_top_i + 15]=m_top+4;
    m_indices[m_top_i + 16]=m_top+3;
    m_indices[m_top_i + 17]=m_top+7;

    // side B
    m_indices[m_top_i + 18]=m_top+5;
    m_indices[m_top_i + 19]=m_top+1;
    m_indices[m_top_i + 20]=m_top+2;

    m_indices[m_top_i + 21]=m_top+5;
    m_indices[m_top_i + 22]=m_top+2;
    m_indices[m_top_i + 23]=m_top+6;

    // top
    m_indices[m_top_i + 24]=m_top+7;
    m_indices[m_top_i + 25]=m_top+3;
    m_indices[m_top_i + 26]=m_top+2;

    m_indices[m_top_i + 27]=m_top+7;
    m_indices[m_top_i + 28]=m_top+2;
    m_indices[m_top_i + 29]=m_top+6;

    // bottom
    m_indices[m_top_i + 30]=m_top+4;
    m_indices[m_top_i + 31]=m_top+0;
    m_indices[m_top_i + 32]=m_top+1;

    m_indices[m_top_i + 33]=m_top+4;
    m_indices[m_top_i + 34]=m_top+1;
    m_indices[m_top_i + 35]=m_top+5;

    m_top   += 8;
    m_top_i += 36;

  };

};

// ---   *   ---   *   ---
// send data to gbuffs

void Debug_Draw::upload(void) {

  auto& vbo=m_vao.gbuff(VAO::VBO);
  auto& ibo=m_vao.gbuff(VAO::IBO);

  vbo.sub_data((void*) m_verts.data(),0,m_top);
  ibo.sub_data((void*) m_indices.data(),0,m_top_i);

};

// ---   *   ---   *   ---
// make gl draw call

void Debug_Draw::draw(void) {

  m_vao.bind();

  glDrawElements(

    m_mode,m_top_i,

    GL_UNSIGNED_SHORT,
    (void*) 0

  );

  m_vao.unbind();

};

// ---   *   ---   *   ---
// helper cstruc

void Debug_Draw::Vertex::set(

  vec3     pos,
  float    scale,

  uint8_t  color,
  uint16_t flags

) {

  this->set_pos(pos);
  this->set_scale(scale);
  this->set_color(color);
  this->set_flags(flags);

};

// ---   *   ---   *   ---
// pack vertex position

void Debug_Draw::Vertex::set_pos(vec3 pos) {

  pos+=vec3({NEGATIVE,NEGATIVE,NEGATIVE});

  uint16_t x = uint16_t(pos.x/CENT);
  uint16_t y = uint16_t(pos.y/CENT);
  uint16_t z = uint16_t(pos.z/CENT);

  m_data.x  =  uint32_t(x) | uint32_t(y<<16);

  m_data.y &=~ 0x0000FFFF;
  m_data.y |=  z;

};

// ---   *   ---   *   ---
// ^undo

vec3 Debug_Draw::Vertex::get_pos(void) {

  vec3 out={
    float(m_data.x  & 0xFFFF) * CENT,
    float(m_data.x >>     16) * CENT,
    float(m_data.y &  0xFFFF) * CENT

  };

  out-=vec3({NEGATIVE,NEGATIVE,NEGATIVE});

  return out;

};

// ---   *   ---   *   ---
// set point size/line width

void Debug_Draw::Vertex::set_scale(float scale) {

  uint16_t x = uint16_t(scale/CENT);

  m_data.y &=~ 0xFFFF0000;
  m_data.y |=  uint32_t(x) << 16;

};

// ---   *   ---   *   ---
// set point color

void Debug_Draw::Vertex::set_color(uint8_t color) {
  m_data.z &=~ 0x000000FF;
  m_data.z |=  color;

};

// ---   *   ---   *   ---
// misc flags

void Debug_Draw::Vertex::set_2d(bool enable) {
  m_data.z &=~ 0x00000100;
  m_data.z |=  uint32_t(enable) << 8;

};

void Debug_Draw::Vertex::set_flags(uint16_t flags) {
  m_data.z &=~ 0x00FFFF00;
  m_data.z |=  uint32_t(flags) << 8;

};

// ---   *   ---   *   ---
