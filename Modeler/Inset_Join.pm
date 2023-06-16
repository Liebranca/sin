#!/usr/bin/perl
# ---   *   ---   *   ---
# MODELER INSET JOIN
# A more complex form of join
#
# LIBRE SOFTWARE
# Licensed under GNU GPL3
# be a bro and inherit
#
# CONTRIBUTORS
# lyeb,

# ---   *   ---   *   ---
# deps

package Modeler::Inset_Join;

  use v5.36.0;
  use strict;
  use warnings;

  use English qw(-no_match_vars);

  use lib $ENV{'ARPATH'}.'/lib/sys/';
  use Style;

  use lib $ENV{'ARPATH'}.'/sin/';
  use Modeler::Join;

# ---   *   ---   *   ---
# info

  our $VERSION = v0.00.1;#b
  our $AUTHOR  = 'IBN-3DILA';

# ---   *   ---   *   ---
#

sub distribute($outer,$inner,$cnt) {

  return 'winding_even'
  if $outer == $inner && $cnt == 1;

  my $b=0;

  if($cnt > 1) {
    map {

      # first and last
      if($ARG==1 || $ARG==$cnt) {
        $b+=$inner-1;

      } else {
        $b+=$inner-2;

      };

    } (1..$cnt);

    $b-=1;
    $outer-=$outer&1;

  } else {
    $b=$inner;

  };

  my $a=$outer;

  if($b > $a) {

    my $c=$b;
    my $b=$outer;

    $a=$c;

  };

  my @out=Modeler::Join::distribution($b,$a);
  balance(\@out) if @out > 2;

  return @out;

};

# ---   *   ---   *   ---
# slight variation on the F
# of the same name, see
# Modeler::Join

sub balance_mid_mirror($ar,$ptr) {

  my $end=@$ar-$ptr-1;

REPEAT:

  my $skip = 1;

  while($ptr < $end) {

    if($ar->[$ptr] > $ar->[$end]) {
      $ar->[$ptr]--;
      $ar->[$end]++;

      $skip=0;

    } elsif(

       (($ptr > 0) && ($end < @$ar-1))

    && ($ar->[$ptr-1]-1 > $ar->[$ptr])
    && ($ar->[$end+1]-1 > $ar->[$end])

    ) {

      $ar->[$ptr-1]--;
      $ar->[$ptr]++;

      $ar->[$end+1]--;
      $ar->[$end]++;

    };

    $ptr++;
    $end--;

  };

  goto REPEAT if ! $skip;

};

# ---   *   ---   *   ---
# ^sugar

sub balance($ar) {

  Modeler::Join::balance_head($ar);
  Modeler::Join::balance_tail($ar);
  Modeler::Join::balance_body($ar);

  Modeler::Join::balance_mid_line($ar);

  my $ptr=1;
  while($ptr < @$ar/2) {
    balance_mid_mirror($ar,$ptr++);

  };

  Modeler::Join::balance_min($ar);
  Modeler::Join::balance_tail($ar);

  $ptr=1;
  while($ptr < @$ar/2) {
    balance_mid_mirror($ar,$ptr++);

  };

};

# ---   *   ---   *   ---

say join q[,],(distribute(4,3,2));

# ---   *   ---   *   ---
1; # ret
