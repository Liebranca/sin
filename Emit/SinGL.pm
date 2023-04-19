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

  use Arstd::String;
  use Arstd::Array;
  use Tree::Grammar;

  use Shb7::Path;

  use Vault;

  use lib $ENV{'ARPATH'}.'/lib/';

  use Emit::C;

  use parent 'Emit';

# ---   *   ---   *   ---
# info

  our $VERSION = v0.00.3;#b
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

  my $out      = $NULLSTR;

  my $scope    = $class->get_src_scope($stout);
  my $st_scope = "$scope\::S$stout->{name}";

  my %O=(

    author     => $stout->{author},

    add_guards => 1,
    define     => [],

    include    => [
      $class->get_deps_list($stout)

    ],

  );

  my $struc_blk=
    $class->get_struc($stout,'ubos')
  . $class->get_struc($stout,'ssbos')
  ;

  my $decls_blk=
    $class->get_src($stout,'v')
  . $class->get_src($stout,'f')
  ;

  my $params_blk=$class->get_params($stout);

  $out.=

    Emit::C->boiler_open($stout->{name},%O)

  . "namespace $scope {\n"
  . "$decls_blk\n"
  . "$params_blk\n"

  . "\n}; // $scope\n"

  . "namespace $st_scope {\n"
  . "$struc_blk\n"
  . "\n}; // $st_scope\n"

  . Emit::C->boiler_close($stout->{name},%O)

  ;

  return $out;

};

# ---   *   ---   *   ---

sub get_params($class,$stout) {

  my $name = $stout->{name};

  my $gv   = $class->get_GENIUS($stout,'v');
  my $gf   = $class->get_GENIUS($stout,'f');

  my $out  =

    $gv->{decl}
  . $gf->{decl}

  . "  const Params $name={\n"

  . "    .source_v     = $gv->{name},\n"
  . "    .source_v_sz  = $gv->{cnt},\n\n"

  . "    .source_f     = $gf->{name},\n"
  . "    .source_f_sz  = $gf->{cnt},\n\n"

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

  @ar=map {q["] . $ARG . q["]} @ar;

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

  my $name = "g_$stout->{name}_${mode}";
  my $pad  = q[    ];

  my @ar   = (

    "shader\::version_${mode}",
    $class->get_extern_src($stout,$mode),
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
  return "$stout->{name}_${mode}_src";

};

sub get_src_scope($class,$stout) {

  my $scope=$stout->{scope};
  $scope=~ s[/][::]sxmg;
  $scope="shader\::$scope";

  return $scope;

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
# make list of external sources

sub get_extern_src($class,$stout,$mode) {

  my @out    = ();
  my $ar     = $stout->{extern_s}->{$mode};

  my @keys   = array_keys($ar);
  my @values = array_values($ar);

  while(@keys && @values) {

    my $key=shift @keys;
    my $val=shift @values;

    last if ! $key;

    my $xstout={
      scope => $val,
      name  => $key,

    };

    $xstout->{scope}=
      $class->get_src_scope($xstout);

    $xstout->{name}=
      $class->get_src_name($xstout,$mode);

    push @out,

      $xstout->{scope}

    . '::'
    . $xstout->{name}

    ;

  };

  return @out;

};

# ---   *   ---   *   ---
# gives includes list

sub get_deps_list($class,$stout) {

  my @out   = ();
  my @deps  = ();
  my $flist = $stout->{extern};

  push @deps,
    @{$flist->{v}},
    @{$flist->{f}}

  ;

  my %no_dup=map {$ARG=>0} @deps;
  for my $dep(@deps) {
    push @out,$dep if ! $no_dup{$dep};
    $no_dup{$dep}=1;

  };

  map {
    $ARG=~ s[/src/][/];
    $ARG=~ s[\.sg$][.hpp_sg];

  } @out;

  return (

    '<glm/vec4.hpp>',
    '"sin/shader/Params.hpp"',

    map {q["] . shpath($ARG) . q["] } @out

  );

};

# ---   *   ---   *   ---
1; # ret
