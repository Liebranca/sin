#!/usr/bin/perl
# ---   *   ---   *   ---
# SINGL GRAMMAR
# Makes shaders less dull
#
# LIBRE SOFTWARE
# Licensed under GNU GPL3
# be a bro and inherit
#
# CONTRIBUTORS
# lyeb,

# ---   *   ---   *   ---
# deps

package Grammar::SinGL;

  use v5.36.0;
  use strict;
  use warnings;

  use Readonly;
  use English qw(-no_match_vars);

  use lib $ENV{'ARPATH'}.'/avtomat/sys/';

  use Style;
  use Chk;
  use Fmat;

  use Arstd::Array;
  use Arstd::String;
  use Arstd::IO;

  use Tree::Grammar;

  use lib $ENV{'ARPATH'}.'/avtomat/hacks/';
  use Shwl;

  use lib $ENV{'ARPATH'}.'/avtomat/';

  use Lang;
  use Grammar;

# ---   *   ---   *   ---
# info

  our $VERSION = v0.00.1;#b
  our $AUTHOR  = 'IBN-3DILA';

# ---   *   ---   *   ---
# ROM

BEGIN {

  sub Frame_Vars($class) { return {

    %{Grammar->Frame_Vars()},
    -passes => ['_ctx','_opz','_run'],

  }};

# ---   *   ---   *   ---

  our $REGEX={

    term  => Lang::nonscap(q[;]),
    nterm => Lang::nonscap(q[;],

      iv    => 1,
      mod   => '+',
      sigws => 1,

    ),

    prim=>qr{(

      void | bool | int | uint | float

      | [biu]?vec[234]
      | mat[234]

      | sampler([^\s]+)?
      | buffer

    )}x,

    spec=>qr{(
      const | uniform | in | out | flat

    )}x,

    lcom=>Lang::eaf(q[\/\/]),

    q[shd-type]=>Lang::eiths(

      [qw(vert frag)],

      insens => 1,
      bwrap  => 1,

    ),

    q[pe-beg]=>qr{\$\:},
    q[pe-end]=>qr{\;\>},

    any   => qr{.+\n}x,

    name  => qr{[_\w][_\w\d]*},
    fname => qr{

      < [_\.\w][_\.\w\d\/]* >

    }x,

    q[io-key]=>qr{(
      (?: flat \s+)? in|out

    )}x,

  };

# ---   *   ---   *   ---
# meta ops

  rule('~<lcom>');

  rule('~<pe-beg>');
  rule('~<pe-end>');

  rule('~<shd-type>');

  rule(q[

    $<pe-anchor>
    &pe_anchor

    pe-beg shd-type pe-end

  ]);

# ---   *   ---   *   ---
# ^post-parse

sub pe_anchor($self,$branch) {
  Grammar::tween_clip($self,$branch);

};

sub pe_anchor_ctx($self,$branch) {

  my $type=$branch->leaf_value(0);

  $branch->clear();
  $branch->{value}=$type;

  # get nodes up to next hierarchical
  my @out=$branch->{parent}->match_until(
    $branch,qr{^pe-anchor$}

  );

  # ^all remaining on fail
  @out=$branch->{parent}->all_from(
    $branch

  ) if ! @out;

  $branch->pushlv(@out);

};

# ---   *   ---   *   ---
# copy-paste hook

  rule('%<hashtag=\#>');
  rule('%<include-key=include>');

  rule('~<fname>');

  rule('$<include> hashtag include-key fname');

# ---   *   ---   *   ---
# ^post-parse

sub include($self,$branch) {

  my $st    = $branch->bhash();
  my $fname = $st->{fname};

  $fname=~ s[^<|>$][]sxmg;

  $branch->clear();
  $branch->init($fname);

};

# ---   *   ---   *   ---
# shader inputs

  rule('~<prim>');
  rule('~<name>');
  rule('~<term>');
  rule('~<nterm>');

  rule('%<uniform-key=uniform>');
  rule('~<io-key>');

  rule('$<io> io-key prim name');
  rule('$<uniform> &io uniform-key prim name');

# ---   *   ---   *   ---
# ^post-parse

sub io($self,$branch) {

  my ($key) = $branch->ipluck(0);
  $key      = $key->leaf_value(0);

  my $flat  = ($key=~ s[^flat \s+][]x);
  my $st    = $branch->bhash();

  my $o     = {

    name => $st->{name},
    type => $st->{prim},

    flat => $flat,

  };

  $branch->clear();
  $branch->init($o);

  $branch->{value}=$key;

};

# ---   *   ---   *   ---

  rule('~<any>');
  rule('|<meta> &clip lcom pe-anchor include');

  rule(q[
    |<needs-term-list>
    &clip

    io uniform

  ]);

  rule(q[
    $<needs-term>
    &clip

    needs-term-list term

  ]);

  our @CORE=qw(meta needs-term any);

# ---   *   ---   *   ---

}; # BEGIN

# ---   *   ---   *   ---
# test

my $prog=q[

  $:VERT;>

  #include <path/to/file>
  uniform mat4 n0;
  flat in uint n1;

  $:FRAG;>
  bye

];

my $ice=Grammar::SinGL->parse($prog,-r=>2);
$ice->{p3}->prich();

# ---   *   ---   *   ---
1; # ret
