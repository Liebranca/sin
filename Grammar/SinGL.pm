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

    -vx_attrs => [],
    -px_attrs => [],

    -uniforms => [],
    -samplers => [],

    -ubos     => [],
    -ssbos    => [],

    -cmode    => $NULLSTR,

    %{Grammar->Frame_Vars()},
    -passes=>['_ctx','_opz','_run'],

  }};

# ---   *   ---   *   ---

  our $REGEX={

    term  => Lang::nonscap(q[;]),
    nterm => Lang::nonscap(q[;],

      iv    => 1,
      mod   => '+',
      sigws => 1,

    ),

    nparens=>Lang::nonscap(q[\)],

      iv    => 1,
      mod   => '+',
      sigws => 1,

    ),

    q[meta-tag-key]=>Lang::eiths(

      [qw(author version)],

      insens => 1,
      bwrap  => 1,

    ),

    prim=>qr{(

      void | bool | int | uint | float

      | [biu]?vec[234]
      | mat[234]

      | sampler([^\s]+)?
      | buffer

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

    q[decl-size]=>qr{\[\d+\]}x,

  };

# ---   *   ---   *   ---
# base patterns

  rule('~<any>');
  rule('~<prim>');
  rule('~<name>');
  rule('~<term>');
  rule('~<nterm>');

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

  my $frame = $self->{frame};
  my $type  = $branch->leaf_value(0);

  $branch->clear();
  $frame->{-cmode}=$type;
  $branch->{value}=$type;

  $branch->pushlv(
    $branch->match_up_to(qr{^pe-anchor$})

  );

};

sub pe_anchor_opz($self,$branch) {

  my $type=$branch->{value};

  $self->paste_px_attrs($branch)
  if (uc $type) eq 'FRAG';

};

# ---   *   ---   *   ---
# syncs frag in to vert out

sub paste_px_attrs($self,$branch) {

  my $frame = $self->{frame};
  my $attrs = $frame->{-px_attrs};

  my $i     = 0;

  for my $attr(@$attrs) {

    $branch->insert($i,'BLANK');

    my $dst = \$branch->{leaves}->[$i++];
    my $cpy = $attr->dup();

    $$dst=$cpy;

    $$dst->{value}  = 'in';
    $$dst->{parent} = $branch;

  };

};

# ---   *   ---   *   ---
# more meta

  rule('~<meta-tag-key>');

  rule(q[
    $<meta-tag>
    &meta_tag

    meta-tag-key nterm term

  ]);

# ---   *   ---   *   ---
# ^post-parse

sub meta_tag($self,$branch) {

  my $st=$branch->bhash();

  $branch->clear();
  $branch->init($st->{nterm});
  $branch->{value}=$st->{q[meta-tag-key]};

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
# converts ins and outs

sub io_ctx($self,$branch) {

  my $frame = $self->{frame};
  my $cmode = $frame->{-cmode};

  my $key   = $branch->{value};
  my $o     = $branch->leaf_value(0);

  # remember vertex shader inputs
  $self->reg_vx_attrs($branch)
  if $key eq 'in';

  # convert vertex out to frag in
  $self->reg_px_attrs($branch)

  if $key eq 'out'
  && (uc $cmode) eq 'VERT'
  ;

};

# ---   *   ---   *   ---
# registers vertex shader in
# as vertex attributes

sub reg_vx_attrs($self,$o) {

  my $frame=$self->{frame};
  my $attrs=$frame->{-vx_attrs};

  push @$attrs,$o;

};

# ---   *   ---   *   ---
# registers vertex shader out
# as frag shader in

sub reg_px_attrs($self,$o) {

  my $frame=$self->{frame};
  my $attrs=$frame->{-px_attrs};

  push @$attrs,$o;

};

# ---   *   ---   *   ---
# interface blocks

  rule('%<layout-key=layout>');
  rule('%<buffer-key=buffer>');

  rule('%<beg-parens=\(>');
  rule('%<end-parens=\)>');

  rule('~<nparens>');

  rule(q[

    $<layout-args>
    &tween_clip

    beg-parens
    nparens

    end-parens

  ]);

  rule(q[

    |<layout-type>
    &cclip

    uniform-key buffer-key

  ]);

  rule('%<beg-scope=\{>');
  rule('%<end-scope=\}>');

  rule('~<decl-size>');
  rule('?<opt-decl-size> &clip decl-size');

  rule('$<decl> name name opt-decl-size');
  rule('+<block-elems> decl term');

  rule(q[

    $<block-body>
    &tween_clip

    beg-scope
    block-elems

    end-scope

  ]);

# ---   *   ---   *   ---
# ^post-parse

sub decl($self,$branch) {

  my @order = qw(type name size);
  my $lv    = $branch->{leaves};

  my $o     = {};

  for my $nd(@$lv) {

    my $key    = shift @order;
    $o->{$key} = $nd->leaf_value(0);

  };

  $o->{size}//='1';
  $o->{size}=~ s{^\[|\]$}{}sxmg;

  $branch->clear();
  $branch->{value}=$o;

};

# ---   *   ---   *   ---
# ^layouts continued II

  rule(q[

    $<layout>

    layout-key layout-args
    layout-type name

    block-body

    name

  ]);

# ---   *   ---   *   ---
# ^post-parse

sub layout($self,$branch) {

  my $re       = qr{^name$};
  my $blk_name = $branch->branch_in($re);

  $blk_name->{value}='block-name';

  my $ice_name = $branch->branch_in($re);

  $ice_name->{value}='ice-name';

  my $st   = $branch->bhash(0,0,0,0,1,0);

  my $args = $st->{q[layout-args]};
  my @args = split $COMMA_RE,$args;

  my $o    = {

    type     => $st->{q[layout-type]},
    args     => \@args,

    body     => $st->{q[block-body]},

    blk_name => $st->{q[block-name]},
    ice_name => $st->{q[ice-name]},

  };

  $branch->clear();
  $branch->init($o);

};

# ---   *   ---   *   ---

  rule(q[
    |<meta>
    &clip

    lcom meta-tag
    pe-anchor include

  ]);

  rule(q[
    |<needs-term-list>
    &clip

    io uniform layout

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

  VERSION   v0.00.1b;
  AUTHOR    'IBN-3DILA';

$:VERT;>

  #include <path/to/file>

  uniform mat4 n0;
  flat in uint n1;

  out uint n2;

  layout (std430) buffer blockn {
    uint x[128];
    uint y[128];

  } icen;

$:FRAG;>

void main(void) {

};

];

my $ice=Grammar::SinGL->parse($prog,-r=>2);
$ice->{p3}->prich();

# ---   *   ---   *   ---
1; # ret
