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

  use Emit::C;

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

  my $struc_blk=
    $class->get_struc($stout,'ubos')
  . $class->get_struc($stout,'ssbos')
  ;

  my $decls_blk=
    $class->get_src($stout,'vx')
  . $class->get_src($stout,'px')
  ;

  my $params_blk=$class->get_params($stout);

  say $decls_blk;
  say $params_blk;
  say $struc_blk;

};

sub get_params($class,$stout) {

  my $name = $stout->{sh_name};

  my $gvx  = $class->get_GENIUS($stout,'vx');
  my $gpx  = $class->get_GENIUS($stout,'px');

  my $out  =

    $gvx->{decl}
  . $gpx->{decl}

  . "  const Params ={\n"

  . "    .source_v     = $gvx->{name},\n"
  . "    .source_v_sz  = $gvx->{cnt},\n\n"

  . "    .source_f     = $gpx->{name},\n"
  . "    .source_f_sz  = $gpx->{cnt},\n\n"

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

  my $pad=q[      ];

  return

    "    .$key={\n"
  . "      " . (join ",\n$pad",@ar) . "\n"

  . "    },\n"
  . "    .num_$key=" . int(@ar) . ",\n\n"

  ;

};

# ---   *   ---   *   ---
# GENIUS
#
# an ironical moniker befitting
# syntactical dullness
#
# shortened to g cat name cat shd

sub get_GENIUS($class,$stout,$mode) {

  my $src_name=
    $class->get_src_name($stout,$mode);

  my $name = "g_$stout->{sh_name}_${mode}";
  my $pad  = q[    ];

  my @ar   = (

    "shader\::version_${mode}",

    ('NI_extern_src_list'),
    "$src_name"

  );

  my $decl=
    "  const char* ${name}[]={\n"
  . "    " . (join ",\n$pad",@ar) . "\n"
  . "  };\n\n"
  ;

  return {

    name => $name,

    decl => $decl,
    cnt  => int(@ar),

  };

};

# ---   *   ---   *   ---

sub get_src($class,$stout,$mode) {

  my $name=$class->get_src_name(
    $stout,$mode

  );

  return

    "  const char* $name="
  . q[R"glsl(]
  . "\n"

  . $stout->{local}->{$mode}

  . q[)glsl";]
  . "\n\n"

  ;

};

# ---   *   ---   *   ---

sub get_src_name($class,$stout,$mode) {
  return "$stout->{sh_name}_${mode}_src";

};

# ---   *   ---   *   ---

sub get_struc($class,$stout,$mode) {

  my $out   = $NULLSTR;
  my $pad   = q[  ];
  my $attrs = $stout->{$mode};

  goto SKIP if ! @$attrs;

  my @keys  = array_keys($attrs);

  for my $attr(array_values($attrs)) {

    my $key = shift @keys;
    my @ar  = map {

      $pad

    . Emit::C->typecon($ARG->{type})
    . " $ARG->{name}"

    . (($ARG->{size} > 1)
        ? "[$ARG->{size}]"
        : $NULLSTR

      )

    } @{$attr->{body}};

    $out.=
      "  struct $key {\n"
    . $pad . (join ";\n$pad",@ar) . ";\n\n"

    . "  };\n"
    ;

  };

SKIP:
  return $out;

};

# ---   *   ---   *   ---
1; # ret
