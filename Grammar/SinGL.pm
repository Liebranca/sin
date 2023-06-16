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

  use Cwd qw(abs_path);

  use Carp;
  use Readonly;

  use English qw(-no_match_vars);

  use lib $ENV{'ARPATH'}.'/lib/sys/';

  use Style;
  use Chk;
  use Fmat;

  use Type;

  use Arstd::Array;
  use Arstd::String;
  use Arstd::Path;
  use Arstd::IO;

  use Shb7::Find;
  use Shb7::Path;

  use Tree::Grammar;

  use Vault;

  use lib $ENV{'ARPATH'}.'/lib/';

  use Lang;
  use Grammar;

  use Emit::SinGL;

# ---   *   ---   *   ---
# info

  our $VERSION = v0.00.2;#b
  our $AUTHOR  = 'IBN-3DILA';

# ---   *   ---   *   ---
# ROM

BEGIN {

  sub Frame_Vars($class) { return {

    -v_out      => $NULLSTR,
    -f_out      => $NULLSTR,

    -v_attrs    => [],
    -f_attrs    => [],

    -v_uniforms => [],
    -f_uniforms => [],

    -v_ubos     => [],
    -f_ubos     => [],

    -v_ssbos    => [],
    -f_ssbos    => [],

    -v_extern   => [],
    -f_extern   => [],

    -samplers   => [],

    -cmode      => $NULLSTR,

    %{Grammar->Frame_Vars()},
    -passes=>['_ctx','_opz','_emit'],

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
      (?: flat \s+)? (in|out)

    )}x,

    q[decl-size]=>qr{\[\d+\]}x,

  };

# ---   *   ---   *   ---
# base patterns

  rule('~<any>');
  rule('~<name>');
  rule('~<term>');
  rule('~<nterm>');

# ---   *   ---   *   ---
# ^spit as-is

sub any_emit($self,$branch) {

  # get ctx
  my $dst = $self->get_dst($branch);
  my $s   = $branch->leaf_value(0);

  # paste
  $$dst.="$s";

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

  $self->paste_frag_attrs($branch)
  if (uc $type) eq 'FRAG';

};

# ---   *   ---   *   ---
# syncs frag in to vert out

sub paste_frag_attrs($self,$branch) {

  my $frame = $self->{frame};
  my $attrs = $frame->{-f_attrs};

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
  $branch->{value}=(uc $st->{q[meta-tag-key]});

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

sub include_ctx($self,$branch) {

  my $frame = $self->{frame};
  my $cmode = $frame->{-cmode};

  my $attr  = $NULLSTR;

  if((uc $cmode) eq 'VERT') {
    $attr='v_extern';

  } elsif((uc $cmode) eq 'FRAG') {
    $attr='f_extern';

  };

  $self->reg_attr($attr,$branch)
  if $attr ne $NULLSTR;

};

# ---   *   ---   *   ---
# template: push to frame array

sub reg_attr($self,$key,$o) {

  my $frame = $self->{frame};
  my $attrs = $frame->{"-$key"};

  push @$attrs,$o;

};

# ---   *   ---   *   ---
# shader inputs

  rule('%<uniform-key=uniform>');
  rule('~<io-key>');

  rule('$<io> io-key name name');
  rule('$<uniform> &io uniform-key name name');

# ---   *   ---   *   ---
# ^post-parse

sub io($self,$branch) {

  # remove keyword
  my ($key)=$branch->ipluck(0);
  $key=$key->leaf_value(0);

  # get name of type
  my $prim=$branch->branch_in(qr{^name$});
  $prim->{value}='type';

  # get flat spec
  my $flat=($key=~ s[^flat \s+][]x);

  # build hash from tree
  my $st = $branch->bhash();
  my $o  = {

    name => $st->{name},
    type => $st->{type},

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

  my $attr  = $NULLSTR;

# ---   *   ---   *   ---
# vertex shader inputs

  if((uc $cmode) eq 'VERT') {

    if($key eq 'in') {
      $attr='v_attrs';

    } elsif($key eq 'out') {
      $attr='f_attrs';

    };

# ---   *   ---   *   ---
# uniforms

  };if($key eq 'uniform') {

    if($o->{type}=~ m[^sampler]) {
      $attr='samplers';

    } else {

      my $mode=($cmode eq 'VERT')
        ? 'v'
        : 'f'
        ;

      $attr="${mode}_uniforms";

    };

  };

# ---   *   ---   *   ---
# ^write to frame

  $self->reg_attr($attr,$branch)
  if $attr ne $NULLSTR;

};

# ---   *   ---   *   ---
# outs code

sub io_emit($self,$branch) {

  # get ctx
  my $dst = $self->get_dst($branch);
  my $st  = $branch->leaf_value(0);

  # make line from branch
  my $s   = join q[ ],

    ('flat' x int($st->{flat})),

    $branch->{value},
    $st->{type},
    $st->{name},

  ;

  # clean
  $s=~ s[^\s+][];

  # ^append to codestr
  $$dst.="  $s;\n";

};

# ---   *   ---   *   ---
# ^get dst string for emitters

sub get_dst($self,$branch) {

  my $out   = undef;

  my $frame = $self->{frame};
  my $cmode = $branch->{parent}->{value};

  if((uc $cmode) eq 'VERT') {
    $out=\$frame->{-v_out};

  } elsif((uc $cmode) eq 'FRAG') {
    $out=\$frame->{-f_out};

  };

  return $out;

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
# ^second pass

sub layout_ctx($self,$branch) {

  my $cmode = $branch->{parent}->{value};

  my $st    = $branch->leaf_value(0);
  my $attr  = $NULLSTR;

  if($st->{type} eq 'uniform') {
    $attr='ubos';

  } elsif($st->{type} eq 'buffer') {
    $attr='ssbos';

  };

  my $mode=($cmode eq 'VERT')
    ? 'v'
    : 'f'
    ;

  $self->reg_attr("${mode}_$attr",$branch)
  if $attr ne $NULLSTR;

};

# ---   *   ---   *   ---
# code spit

sub layout_emit($self,$branch) {

  # get ctx
  my $dst = $self->get_dst($branch);
  my $st  = $branch->leaf_value(0);

  # paste decls
  my $body=$NULLSTR; map {

    $body.=q[    ] . (join q[ ],

      $ARG->{type},
      $ARG->{name},

      ("[$ARG->{size}]" x ($ARG->{size} ne 1))

    ) . ";\n";

  } @{$st->{body}};

  # build line
  my $s=q[  ] . join q[ ],

    $branch->{value},
    '(' . (join q[,],@{$st->{args}}) . ')',

    $st->{type},
    $st->{blk_name},

  ;

  # paste decls
  $s.=" {\n$body\n  } $st->{ice_name}";

  # cat to codestr
  $$dst.="$s;\n";

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
# generic errme

sub throw($me) {

  croak

    "SinGL died on { $me }" .
    "\nABORT"

  ;

};

# ---   *   ---   *   ---
# generates extern field of stout

sub get_extern($self,$mode) {

  my $out   = [];

  my $frame = $self->{frame};
  my $ar    = $frame->{"-${mode}_extern"};

  for my $nd(@$ar) {

    my $path=$nd->leaf_value(0);
    next if $path eq 'version';

    my $f="$path.sg";

    $f=dirof($f,abs=>0) . q[/src/] . basef($f)
    if ! -e $f;

    $f=ffind($f) or throw("include $path");

    push @$out,$f;

  };

  return $out;

};

# ---   *   ---   *   ---
# get single field attr for stout

sub get_cattr($self,$key) {

  my $out   = [];

  my $frame = $self->{frame};
  my $attrs = $frame->{"-$key"};

  for my $attr(@$attrs) {

    my $st   = $attr->leaf_value(0);
    my $type = Emit::SinGL
      ->typecon($st->{type});

    push @$out,$st->{name}=>$type;

  };

  return $out;

};

# ---   *   ---   *   ---
# ^same idea, but for interface blocks

sub get_iattr($self,$key) {

  my $out   = [];

  my $frame = $self->{frame};
  my $attrs = $frame->{"-$key"};

  for my $attr(@$attrs) {

    my $st   = $attr->leaf_value(0);
    my $cpy  = {%$st};

    my $name = $cpy->{blk_name};
    delete $cpy->{blk_name};

    $self->iattr_body($cpy->{body});

    push @$out,$name=>$cpy;


  };

  return $out;

};

# ---   *   ---   *   ---
# ^elem walk

sub iattr_body($self,$ar) {

  for my $elem(@$ar) {
    $elem->{type}=Emit::SinGL
      ->typecon($elem->{type});

  };

};

sub get_local($self,$mode) {
  return $self->{frame}->{"-${mode}_out"};

};

# ---   *   ---   *   ---
# get props by shader type

sub stout_of($self,$mode) { return {

  extern   => $self->get_extern($mode),
  local    => $self->get_local($mode),

  # vertex attributes
  # only if vert shader
  ($mode eq 'v')
  ? (attrs => $self->get_cattr('v_attrs'))
  : ()
  ,

  # samplers
  # only if frag shader
  ($mode eq 'f')
  ? (samplers => $self->get_cattr('samplers'))
  : ()
  ,

  uniforms => $self->get_cattr("${mode}_uniforms"),

  ubos     => $self->get_iattr("${mode}_ubos"),
  ssbos    => $self->get_iattr("${mode}_ssbos"),

}};

# ---   *   ---   *   ---
# get value of meta-tag

sub get_mtag($self,$key) {

  $key=uc $key;

  my $tree = $self->{p3};
  my $tag  = $tree->branch_in(qr{^$key$});

  my $out  = ($tag)
    ? $tag->leaf_value(0)
    : 'non'
    ;

  $out=~ s[^ (?: "|') | (?: "|') $][]sxmg;

  return $out;

};

# ---   *   ---   *   ---
# ^combo of all
# generates out struct

sub stout($self,$scope,$name) { return {

  name    => $name,
  scope   => $scope,

  author  => $self->get_mtag('author'),
  version => $self->get_mtag('version'),

  v       => $self->stout_of('v'),
  f       => $self->stout_of('f'),

}};

# ---   *   ---   *   ---
# parse and return source descriptor

sub fregen($class,$path,$name=undef) {

  my $prog = orc($path);
  my $ice  = $class->parse($prog,-r=>3);

  $name=($name) ? $name : $path;

  $name= shpath($name);
  $name=~ s[/src/][/];

  my $scope=dirof($name);
  $name=nxbasef($name);

  my $mod=modof($scope);
  $scope=shpath($scope);
  $scope=~ s[${mod}/(?:glsl/)?][];

  return $ice->stout($scope,$name);

};

# ---   *   ---   *   ---
# ^caches result of parsing
# re-runs parse if source is updated

sub fparse($class,$path,$name=undef) {

  # get file meta
  $path=abs_path($path);
  my $stout=Vault::fcached(
    $path,\&fregen,$class,$path,$name

  );

  return $stout;

};

# ---   *   ---   *   ---
# fetches data from includes

sub depchk($class,$stout,$mode) {

  my $deptab = {};

  my @ref    = @{$stout->{$mode}->{extern}};
  my @names  = ();

  while(@ref) {

    my $src=shift @ref;
    next if $deptab->{$src};

    $deptab->{$src}=1;

    my $xstout=$class->fparse($src);

    unshift @ref,
      $class->merge($stout,$xstout,$mode);

    unshift @names,
      $xstout->{name}=>$xstout->{scope};

  };

  $stout->{$mode}->{extern_s}=\@names;

};

# ---   *   ---   *   ---
# ^joins two structs

sub merge($class,$dst,$src,$mode) {

  my $attrs=$src->{$mode};

  for my $key(keys %$attrs) {

    next if $key eq 'local';

    my $ref=$dst->{$mode}->{$key};
    unshift @$ref,@{$attrs->{$key}};

  };

  return @{$src->{$mode}->{extern}};

};

# ---   *   ---   *   ---
# gives attributes without
# vert/frag separator

sub flatten($class,$stout) {

  my $cpy=Vault::deepcpy($stout);

  # dependency checks
  $class->depchk($cpy,'v');
  $class->depchk($cpy,'f');

  my $v=$cpy->{v};
  my $f=$cpy->{f};

  my $tab={

    local    => {},
    extern   => {},
    extern_s => {},

  };

# ---   *   ---   *   ---

  for my $key(keys %$v) {

    if($key=~ m[^( local|extern (_s)? )$]x) {
      $tab->{$1}->{v}=$v->{$key};
      next;

    };

    $cpy->{$key}=$v->{$key};

  };

  for my $key(keys %$f) {

    if($key=~ m[^( local|extern (_s)? )$]x) {
      $tab->{$1}->{f}=$f->{$key};
      next;

    };

    push @{$cpy->{$key}},@{$f->{$key}};

  };

# ---   *   ---   *   ---

  delete $cpy->{v};
  delete $cpy->{f};

  $cpy->{local}    = $tab->{local};
  $cpy->{extern}   = $tab->{extern};
  $cpy->{extern_s} = $tab->{extern_s};

  return $cpy;

};

# ---   *   ---   *   ---
1; # ret
