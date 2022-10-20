// SinGL v0.03.01
// file assembled from font/src/lycon.sg
// ---   *   ---   *   ---
// LIBRE BOILERPASTE
// GENERATED BY AR/AVTOMAT
//
// LICENSED UNDER GNU GPL3
// BE A BRO AND INHERIT
//
// COPYLEFT IBN-3DILA 2022
// ---   *   ---   *   ---

#ifndef __LYCON_H__
#define __LYCON_H__



// ---   *   ---   *   ---
// deps

  #include "bitter/kvrnel/Vec.hpp"
  #include "sin/shader/Params.hpp"


// ---   *   ---   *   ---
// ROM



// ---   *   ---   *   ---


namespace shader::font {
  const char* lycon_frag=R"glsl(
const uint LYCON[256]=
  { 0x827C0028,0x70087082,0xADBD81FF,0xFF81BDB5,
0x9189817E,0x809088BA,0x7E7E7E00,0x007E7E7E,0xD8B28478,0x10003048,
0x5A427E00,0x007E425A,0x0402423C,0x38440438,0xAAFF5500,0x00AAFF55,
0x90901313,0x9C989393,0x55AA55AA,0x55AA55AA,0x0EF3000C,0x0C04F302,
0x00227722,0x00227722,0xC9C9C9C9,0x890949C9,0xAAABEB7E,0x00001020,
0x8C9C9CBC,0xBFBE9C8C,0x60407C7E,0x00001030,0xF7F73434,0x3434F700,
0x1F181818,0x0000001F,0xF4F43434,0x3434F404,0xF8000000,0x181818F8,
0xF7F73434,0x0000FF00,0x1F000000,0x1818181F,0x424A42FF,0x003C425A,
0xF8181818,0x181818F8,0x5A2C1818,0x18346E92,0xFF000000,0x181818FF,
0x7F3E081C,0x00081C3E,0x37373434,0x00003F30,0x3E3E1C08,0x081C3E3E,
0xF4F43434,0x0000FC04,0x818181FF,0xFF818181,0x34343434,0x34343434,
0x9981423C,0x3C428599,0x183C3C18,0x18180018,0x7E7E7E7E,0x187E427E,
0x99423C00,0x00FC3E57,0x00000000,0xC0603018,0x8D423C00,0x00FC3E5B,
0x42427E7E,0x187E4242,0x817F020C,0x0000FC7E,0x6A566A7E,0x187E6A56,
0x9981423C,0x3C4A8999,0x7F2A1C08,0x00081C2A,0x9981423C,0x3C4281F9,
0x6B2A1C08,0x00081C2A,0x99894A3C,0x3C428199,0x63361C08,0x00081C36,
0x9981423C,0x3C42819F,0xBD99423C,0xFF81BDBD,0x7E3C1800,0x669981FF,
0x7E7E7E7E,0x187E4242,0x423C1818,0x66998181,0xDB66BDDB,0xDBBD66DB,
0x42241800,0x66998181,0x3C180000,0x0000182C,0xFFFF7E3C,0xFFFFFFFF,
0x24180000,0x0000182C,0xEFF7763C,0xFFF32FDF,0x5A249942,0x42BD425A,
0xC7D3763C,0xDFD32E99,0x8D423C00,0x003F7CDA,0x0058CA5D,0x00000000,
0x99423C00,0x003F7CC2,0x5A241800,0x6699A5BD,0x663E060F,0x00003C67,
0x3E33735E,0x00003E30,0x0C0C0C1E,0x0000765D,0x4606463C,0x00003C66,
0x0C0C2C38,0x080C0C3E,0x7E06663C,0x00003C66,0x66663C18,0x00006666,
0x667C627C,0x0000DC66,0x3E1C3663,0x00006336,0x1818183C,0x1818001C,
0x0A243E3A,0x00002E3E,0x1E0E1E37,0x07062636,0x18181818,0x18181818,
0x7F6B6B63,0x00003677,0x71000000,0x00008EFF,0x6666663C,0x00003C66,
0x3E1E0606,0x3F666666,0x7E666666,0x183C7E66,0x3E163677,0x3F666666,
0x06667E3C,0x3C7E6606,0x1818183C,0xFFDB9918,0x1E56667E,0x7F66561F,
0x667E3C18,0x66666666,0x76667E5C,0x3C666606,0x183C66EF,0xEF66243C,
0x0C0C1E3E,0x1F1E0C0C,0x184C7E7E,0x7E7E6230,0x0E1E76E7,0xE766361E,
0x18306000,0x0002060C,0x7F7F6B63,0x4163777F,0xC6000000,0x10387CEE,
0x66667E3C,0x3C7E6666,0x7E667E3C,0x3C7E6666,0x18001818,0x183C3C18,
0x180C3E3E,0x1C3E2230,0xD381D3FF,0xFFD381D3,0x7E7E3030,0x30383C36,
0x18ECA6E0,0x07456730,0x3E223E1C,0x1C3E221E,0x00000000,0x0C0C0804,
0x3636361C,0x1C36361C,0x60606030,0x30606060,0x00060600,0x00000606,
0x7E181800,0x0018187E,0x0E1C3830,0x0030381C,0x7E000000,0x0000007E,
0x1C0E0703,0x0003070E,0x180C0600,0x00406030,0xD67E7E00,0x007E56EE,
0x425E427E,0x3E62425E,0x00FFFF00,0x183C7EFF,0xE7DBFFFF,0xFFFFDBE7,
0xFF7E3C18,0x00FFFF00,0x818181FF,0x0F79A9FF,0xF6763616,0x163676F6,
0x0C3C3800,0x081C3E0C,0xD9426E18,0x1876429B,0xC7C7FC78,0x1E3F73B6,
0xFCF8F0E0,0xFFFFFFFE,0x3C383020,0x2030383C,0x7FFFFFFF,0x070F1F3F,
0x3C1C0C04,0x040C1C3C,0x1E1F1F0E,0x38785818,0x7E766646,0x4666767E }



  )glsl";


// ---   *   ---   *   ---


  const char* _DULL_SYNTAX_f_lycon_frag[]={
      lycon_frag
  };

  const Params lycon={
    .source_v=NULL,
    .source_v_sz=0,
    .source_f=_DULL_SYNTAX_f_lycon_frag,
    .source_f_sz=1,
    .attrs={


    },
    .num_attrs=0,
    .uniforms={


    },
    .num_uniforms=0,
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


#endif // __LYCON_H__


