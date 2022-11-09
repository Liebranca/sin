#!/usr/bin/perl
# ---   *   ---   *   ---
# atlas texture fetch math doodles

# ---   *   ---   *   ---
# deps

  use v5.36.0;
  use strict;
  use warnings;

  use English qw(-no_match_vars);

# ---   *   ---   *   ---

my $TILE_SZ = 16;
my $IMG_SZ  = 128/$TILE_SZ;
my $STEP    = 1.0/$IMG_SZ;

sub shift_uv($x,$y) {
  return ($x*$STEP,$y*$STEP)

};

my @TEX=(

  [1,0],
  [2,0]

);

for my $uv(@TEX) {
  my @ar=shift_uv(@$uv);
  say "{$ar[0],$ar[1]}\n";

};

# ---   *   ---   *   ---
