#!/usr/bin/perl
# ---   *   ---   *   ---
# MJOIN
# Distributes vertices
#
# TESTFILE
# jmp EOF for bits
#
# CONTRIBUTORS
# lyeb,

# ---   *   ---   *   ---
# deps

package main;

  use v5.36.0;
  use strict;
  use warnings;

  use List::Util qw(min max);
  use English qw(-no_match_vars);

  use lib $ENV{'ARPATH'}.'/lib/sys/';
  use Style;

# ---   *   ---   *   ---
# info

  our $AUTHOR = 'IBN-3DILA';

# ---   *   ---   *   ---
# calc num of verts of A
# that must connect to each
# vert of B

sub distribute($b,$a) {

  my @out  = ();

  my $size = $b-1;
  my $rem  = $a+$size;

  $b=$size-1;

  while($b) {

    my $cnt=int($rem/$size);
    push @out,$cnt;

    $rem-=$cnt;
    $b--;

  };

  push @out,$rem;
  balance(\@out) if @out > 2;

  return @out;

};

# ---   *   ---   *   ---
# ^evens beg <- end of array

sub balance_head($ar) {

  while($ar->[0]+1 < $ar->[-1]) {
    $ar->[0]++;
    $ar->[-1]--;

  };

};

# ---   *   ---   *   ---
# ^distributes excess verts on
# end of array to middle points

sub balance_tail($ar) {

REPEAT:

  my $i    = 1;
  my $skip = 1;

  while($ar->[-1] > $ar->[0]) {

    $ar->[-1]--;
    $ar->[$i++]++;

    $skip=0;

    last if $i==@$ar-1;

  };

  goto REPEAT if ! $skip && $ar->[-1] > $ar->[0];

};

# ---   *   ---   *   ---
# ^distributes excess verts
# on both ends towards middle

sub balance_body($ar) {

REPEAT:

  my $end  = @$ar-2;
  my $beg  = 1;

  my $skip = 1;

  while(

     ($ar->[-1]-1)
  >= ($ar->[$end]+1)

  && ($ar->[0]-1)
  >= ($ar->[$beg]+1)

  ) {

    $ar->[0]--;
    $ar->[$beg++]++;

    $ar->[-1]--;
    $ar->[$end--]++;

    $skip=0;

    last if $end==0 || $beg==@$ar-1;

  };

  goto REPEAT if ! $skip;

};

# ---   *   ---   *   ---
# ^distributes vertices among
# middle points, without touching
# the beg-end of array

sub balance_mid_line($ar) {

  my $end=@$ar-2;
  my $ptr=1;

REPEAT:

  my $skip = 1;

  while($ptr < $end) {

    if($ar->[$ptr] >= $ar->[$ptr+1]+1) {
      $ar->[$ptr]--;
      $ar->[$ptr+1]++;

      $skip=0;

    };

    $ptr++;

  };

  goto REPEAT if ! $skip;

};

# ---   *   ---   *   ---
# ^similar, balances towards
# second and penultimate element,
# shrinking towards middle

sub balance_mid_mirror($ar,$ptr) {

  my $end=@$ar-$ptr;

REPEAT:

  my $skip = 1;

  while($ptr < $end) {

    if($ar->[$ptr] > $ar->[$end]) {
      $ar->[$ptr]--;
      $ar->[$end]++;

      $skip=0;

    };

    $ptr++;
    $end--;

  };

  goto REPEAT if ! $skip;

};

# ---   *   ---   *   ---
# ensure all points have at
# least 2 verts

sub balance_min($ar) {

  my $uneven = @$ar & 1;
  my $half   = int(@$ar/2);
  my $tresh  = 2;

  my $beg=($uneven)
    ? $half-1
    : $half-1
    ;

  my $end=($uneven)
    ? $half+1
    : $half+1
    ;

REPEAT:

  my $skip   = 1;

  my @bad    = map {$ARG <= $tresh} @$ar;
  my ($i,$j) = ($beg,$end);

  while($i >= 0) {

    if($bad[$i+1] && ! $bad[$i]) {

      $ar->[$i]--;
      $ar->[$i+1]++;

      $bad[$i+1]=0;
      $skip=0;

    };

    if(

       ($bad[$j-1] && ! $bad[$j])
    && ($i+1 != $j-1)
    && ($j < @$ar-1)

    ) {

      $ar->[$j]--;
      $ar->[$j-1]++;

      $bad[$j-1]=0;
      $skip=0;

    };

    $i--;
    $j++;

  };

  my $min = min @$ar;
  my $max = max @$ar;

  while(

     $tresh  < $max
  && $max   != $min

  ) {

    goto REPEAT

    if ! $skip
    && grep {$ARG < $tresh} @$ar
    ;

    $tresh++;

  };

};

# ---   *   ---   *   ---
# ^sugar of all

sub balance($ar) {

  balance_head($ar);
  balance_tail($ar);
  balance_body($ar);

  my $size=@$ar;

  if($size > 3) {

    my $ptr=1;
    balance_mid_line($ar);

    while($size > 3) {
      balance_mid_mirror($ar,$ptr++);
      $size--;

    };

    balance_min($ar);
    balance_tail($ar);

  };

};

# ---   *   ---   *   ---
# test

say $NULLSTR . (join q[,],distribute(11,21));

# ---   *   ---   *   ---
# ^the bit
#
#  given two rings of vertices
#  A and B, we want to calculate
#  how many vertices of B connect
#  with each vertex of A
#
#  we make the following assumptions
#  about the input data:
#
#   * first vertex of both A and B is
#     aligned; that is, both rings have
#     the same rotation
#
#   * that first vertex will always
#     have the exact same connections,
#     therefore we substract these
#     points from the calculation
#
#   * A and B have at least 3 verts each;
#     joining a ring to a point or line
#     is special-cased
#
#
#  ^and due to the rings being aligned,
#  we can make further simplifications:
#
#   * Connections for the second and
#     penultimate vertices will always
#     be mirrored; thus, we only need
#     to solve for one of them.
#
#   * Once the second and penultimate
#     connections are calculated, we
#     divide the remains among the
#     middle points as evenly as possible
#
#
#  better distribution can be achieved
#  by "flowing" connection points from
#  the beg-end of the ring into the middle,
#  which we do as such:
#
#   * find maximum connection count, from
#     both sides
#
#   * compare against minimum plus one
#
#   * if this min is ne either max, substract
#     from the maximum and add onto the
#     minimum
#
#   * repeat until the desired distribution
#     is achieved
#
#
#  we utilize a multitude of variations on
#  this basic technique throughout the test;
#  which one works best does depend on the
#  vertex count of A -- I wouldn't say there's
#  one single, much less simple, way of doing it
#
#  in any case, results allow us to calculate
#  connections between rings of virtually any
#  size save for the aforementioned special cases,
#  so: test successful
#
#  should now look into a C++ implementation
#  of the bits themselves, or spitting of a
#  contable from a generator bin, whichever
#  fits best

# ---   *   ---   *   ---
1; # ret
