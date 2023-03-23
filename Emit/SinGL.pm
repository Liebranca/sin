#!/usr/bin/perl
# ---   *   ---   *   ---
# SINGL EMIT
# Tools for spitting out
#
# LIBRE SOFTWARE
# Licensed under GNU GPL3
# be a bro and inherit
#
# CONTRIBUTORS
# lyeb,

# ---   *   ---   *   ---
# deps

package Emit::SinGL;

  use v5.36.0;
  use strict;
  use warnings;

  use Carp;
  use Readonly;

  use English qw(-no_match_vars);

  use lib $ENV{'ARPATH'}.'/lib/sys/';

  use Style;
  use Chk;

  use Arstd::Array;
  use Tree::Grammar;

  use Vault;

  use lib $ENV{'ARPATH'}.'/lib/';
  use parent 'Emit';

# ---   *   ---   *   ---
# info

  our $VERSION = v0.00.1;#b
  our $AUTHOR  = 'IBN-3DILA';

# ---   *   ---   *   ---
# ROM

  our $Typetab={

    int    => 'sbrad',
    uint   => 'brad',

    float  => 'real',
    double => 'daut',

    # vec3 gets padded to 16 anyway
    # swizzle if you need less

    ivec4  => 'sbrad4',
    uvec4  => 'brad4',
    vec4   => 'real4',

    # ^same story
    # mat3 is almost never 9 floats

    mat4   => 'real16',

  };

# ---   *   ---   *   ---
# make cpp header from sg stout

sub hpp($class,$stout) {
  $stout=Vault::fchk($stout);

  my $params_blk=$class->hpp_params($stout);

  say $params_blk;

};

sub hpp_params($class,$stout) {

  my $out=

    "  const Params $stout->{sh_name}={\n"

  . "    .source_v     = \n"
  . "    .source_v_sz  = \n"

  . "    .source_f     = \n"
  . "    .source_f_sz  = \n"

  . $class->hpp_params_cattr($stout,'attrs')
  . $class->hpp_params_cattr($stout,'uniforms')

  . $class->hpp_params_cattr($stout,'ubos')
  . $class->hpp_params_cattr($stout,'ssbos')

  . $class->hpp_params_cattr($stout,'samplers')

  ;

  return "$out\n  };\n";

};

# ---   *   ---   *   ---
# ^list common attribute

sub hpp_params_cattr($class,$stout,$key) {

  my @ar=(@{$stout->{$key}})
    ? array_keys($stout->{$key})
    : ()
    ;

  return

    "    .$key={\n"
  . "      " . (join ",\n",@ar) . "\n"

  . "    },\n"
  . "    .num_$key=" . int(@ar) . ",\n\n"

  ;

};

# ---   *   ---   *   ---
1; # ret
