// SinGL v0.05.0
// file assembled from lycon_sh v1.00.0
// ---   *   ---   *   ---
// LIBRE BOILERPASTE
// GENERATED BY AR/AVTOMAT
//
// LICENSED UNDER GNU GPL3
// BE A BRO AND INHERIT
//
// COPYLEFT IBN-3DILA 2023
// ---   *   ---   *   ---

#ifndef __LYCON_SH_H__
#define __LYCON_SH_H__



// ---   *   ---   *   ---
// deps

  #include <glm::vec4>
  #include "sin/shader/Params.hpp"
  #include "sin/font/lycon.hpp"


// ---   *   ---   *   ---
// ROM



// ---   *   ---   *   ---

namespace shader::sin::font {
  const char* lycon_sh_vx_src=R"glsl(
  in vec4 Position;
  uniform vec4 Transform;
  layout (std140) uniform Palette {
    vec4 color [16];

  } PAL;
  layout (std430, binding=1) buffer CharData {
    uint i [2048];

  } CHR;
  out vec2 texCoords;
flat out uint  ch;
flat out uint  show_ctl;
  out vec4 fg_col;
  out vec4 bg_col;
void main() {
float px=float(uint(Position.x)&1);
float py=float(uint(Position.y)&1);
gl_Position = vec4(
(px*Transform[0])+Position.z,
(py*Transform[1])+(Position.w-Transform[1]),
0, 1.0
);
texCoords=vec2(px>0,py>0);
uint c_id=uint(Position.x)>>1;
ch=CHR.i[c_id];
fg_col=PAL.color[(ch>>8)&0xF];
bg_col=PAL.color[((ch>>8)&0xF0)>>4];
show_ctl=(ch>>16)&0x1;
ch=ch&0xFF;
};
)glsl";

  const char* lycon_sh_px_src=R"glsl(
  in vec2 texCoords;
  in vec4 fg_col;
  in vec4 bg_col;
void main() {
uint x = uint(texCoords.x*8);
uint y = uint(texCoords.y*8);
uint i = x+(y*8);
uint z = uint(i > 31);
uint chidex = ch&0xFF;
i-=z*32;
bool r = bool(lycon[chidex][z]&(1<<i));
r=r&&(chidex>=show_ctl);
vec4 col = vec4(fg_col.rgb,bg_col.w*float(r));
col.rgb += bg_col.rgb*float(col.w<0.04);
gl_FragColor=col;
};
)glsl";


  const char* g_lycon_sh_vx[]={
    shader::version_vx,
    lycon_sh_vx_src
  };

  const char* g_lycon_sh_px[]={
    shader::version_px,
    shader::sin::font::lycon_px_src,
    lycon_sh_px_src
  };

  const Params ={
    .source_v     = g_lycon_sh_vx,
    .source_v_sz  = 2,

    .source_f     = g_lycon_sh_px,
    .source_f_sz  = 3,

    .attrs={
      Position
    },
    .num_attrs=1,

    .uniforms={
      Transform
    },
    .num_uniforms=1,

    .ubos={
      Palette
    },
    .num_ubos=1,

    .ssbos={
      CharData
    },
    .num_ssbos=1,

    .samplers={
      
    },
    .num_samplers=0,


  };

  struct Palette {
    glm::vec4 color[16];

  };
  struct CharData {
    uint32_t i[2048];

  };


}; // shader::sin::font


// ---   *   ---   *   ---


#endif // __LYCON_SH_H__


