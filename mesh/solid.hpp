// SinGL v0.03.02
// file assembled from mesh/src/solid.sg
// ---   *   ---   *   ---
// LIBRE BOILERPASTE
// GENERATED BY AR/AVTOMAT
//
// LICENSED UNDER GNU GPL3
// BE A BRO AND INHERIT
//
// COPYLEFT IBN-3DILA 2022
// ---   *   ---   *   ---

#ifndef __SOLID_H__
#define __SOLID_H__



// ---   *   ---   *   ---
// deps

  #include <glm/vec4.hpp>
  #include "sin/shader/Params.hpp"


// ---   *   ---   *   ---
// ROM



// ---   *   ---   *   ---


namespace shader::mesh {
  const char* solid_vert=R"glsl(
;
in vec4 Position;
uniform mat4 Model;
uniform mat4 View;
void main(void) {
  gl_Position=(Model*Position)*View;
};



  )glsl";


// ---   *   ---   *   ---


  const char* solid_frag=R"glsl(
;
out vec4 FragColor;
void main(void) {
  FragColor=vec4(1,1,1,1);
}



  )glsl";


// ---   *   ---   *   ---


  const char* _DULL_SYNTAX_v_solid_vert[]={
      shader::version_vert,
      solid_vert
  };

  const char* _DULL_SYNTAX_f_solid_frag[]={
      shader::version_frag,
      solid_frag
  };

  const Params solid={
    .source_v=_DULL_SYNTAX_v_solid_vert,
    .source_v_sz=2,
    .source_f=_DULL_SYNTAX_f_solid_frag,
    .source_f_sz=2,
    .attrs={

      "Position"
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


#endif // __SOLID_H__

