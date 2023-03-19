// SinGL v0.04.03
// file assembled from mesh/src/Solid.sg
// ---   *   ---   *   ---
// LIBRE BOILERPASTE
// GENERATED BY AR/AVTOMAT
//
// LICENSED UNDER GNU GPL3
// BE A BRO AND INHERIT
//
// COPYLEFT ANON 2023
// ---   *   ---   *   ---

#ifndef __SOLID_H__
#define __SOLID_H__



// ---   *   ---   *   ---
// deps

  #include <glm/vec4.hpp>
  #include "sin/shader/Params.hpp"
  #include "sin/mesh/Vertex.hpp"


// ---   *   ---   *   ---
// ROM



// ---   *   ---   *   ---


namespace shader::mesh {
  const char* Solid_vert=R"glsl(
AUTHOR "IBN-3DILA";;
void;



  )glsl";


// ---   *   ---   *   ---


  const char* _DULL_SYNTAX_v_Solid_vert[]={
      shader::version_vert,
      shader::mesh::Vertex_vert,
      Solid_vert
  };

  const char* Solid_frag=R"glsl(
in vec2;
flat in uint;;
uniform sampler2DArray;
out vec4;
void



  )glsl";


// ---   *   ---   *   ---


  const char* _DULL_SYNTAX_f_Solid_frag[]={
      shader::version_frag,
      Solid_frag
  };

  const Params Solid={
    .source_v=_DULL_SYNTAX_v_Solid_vert,
    .source_v_sz=3,
    .source_f=_DULL_SYNTAX_f_Solid_frag,
    .source_f_sz=2,
    .attrs={
      "uvec4"
    },
    .num_attrs=1,
    .uniforms={
      "mat4",
      "mat4"
    },
    .num_uniforms=2,
    .ubos={

    },
    .num_ubos=0,
    .ssbos={

    },
    .num_ssbos=0,
    .samplers={
      "sampler2DArray"
    },
    .num_samplers=1,
  };
};


// ---   *   ---   *   ---



// ---   *   ---   *   ---


#endif // __SOLID_H__


