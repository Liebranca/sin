// SinGL v0.04.01
// file assembled from mesh/src/Vertex.sg
// ---   *   ---   *   ---
// LIBRE BOILERPASTE
// GENERATED BY AR/AVTOMAT
//
// LICENSED UNDER GNU GPL3
// BE A BRO AND INHERIT
//
// COPYLEFT IBN-3DILA 2022
// ---   *   ---   *   ---

#ifndef __VERTEX_H__
#define __VERTEX_H__



// ---   *   ---   *   ---
// deps

  #include <glm/vec4.hpp>
  #include "sin/shader/Params.hpp"


// ---   *   ---   *   ---
// ROM



// ---   *   ---   *   ---


namespace shader::mesh {
  const char* Vertex_vert=R"glsl(
in uvec4 Vertex;
uniform mat4 Model;
uniform mat4 View;
out vec2 Tex_Coords;
const uint FRAC_MAXV[8]=
  { 0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF };
const float FRAC_STEP[8]=
  { 1.0f / 0x0002,1.0f / 0x0004,1.0f / 0x0008,
1.0f / 0x0010,1.0f / 0x0020,1.0f / 0x0040,1.0f / 0x0080,
1.0f / 0x0100 };
const uint FRAC_1BIT=0;
const uint FRAC_2BIT=1;
const uint FRAC_3BIT=2;
const uint FRAC_4BIT=3;
const uint FRAC_5BIT=4;
const uint FRAC_6BIT=5;
const uint FRAC_7BIT=6;
const uint FRAC_8BIT=7;
const bool FRAC_SIGNED=false;
const bool FRAC_UNSIGNED=true;
float unfrac(uint b,float step,uint nbits,
             bool unsig) {
  uint max=FRAC_MAXV[nbits];
  uint mid=1 << nbits;

  b&=max;
  b+=1*uint(b==max);
  b-=mid*uint(!unsig);
  return float (int (b))*step;
};
float unfrac_i8(uint b) {
  return unfrac(b,FRAC_STEP[FRAC_8BIT],FRAC_8BIT,
                FRAC_SIGNED);
};
float unfrac_u8(uint b) {
  return unfrac(b,FRAC_STEP[FRAC_8BIT],FRAC_8BIT,
                FRAC_UNSIGNED);
};
vec2 extract_tex(void) {
  return vec2(unfrac_u8(Vertex[1] >> 16),
              unfrac_u8(Vertex[1] >> 24));
};
vec4 extract_xyz(void) {
  return vec4(unfrac_i8(Vertex[0]),
              unfrac_i8(Vertex[0] >> 8),
              unfrac_i8(Vertex[0] >> 16),1);
};
vec4 apply_transform(vec4 co) {
  return (Model*co)*View;
};



  )glsl";


// ---   *   ---   *   ---


  const char* _DULL_SYNTAX_v_Vertex_vert[]={
      Vertex_vert
  };

  const char* Vertex_frag=R"glsl(
in vec2 Tex_Coords;



  )glsl";


// ---   *   ---   *   ---


  const char* _DULL_SYNTAX_f_Vertex_frag[]={
      Vertex_frag
  };

  const Params Vertex={
    .source_v=_DULL_SYNTAX_v_Vertex_vert,
    .source_v_sz=1,
    .source_f=_DULL_SYNTAX_f_Vertex_frag,
    .source_f_sz=1,
    .attrs={
      "Vertex"
    },
    .num_attrs=1,
    .uniforms={
      "Model",
      "View"
    },
    .num_uniforms=2,
    .ubos={

    },
    .num_ubos=0,
    .ssbos={

    },
    .num_ssbos=0,
    .samplers={

    },
    .num_samplers=0,
  };
};


// ---   *   ---   *   ---



// ---   *   ---   *   ---


#endif // __VERTEX_H__


