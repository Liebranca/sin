#!/usr/bin/perl
# ---   *   ---   *   ---
# MJOIN
# Tests whats now a module
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

  use English qw(-no_match_vars);

  use lib $ENV{'ARPATH'}.'/lib/sys/';
  use Style;

  use lib $ENV{'ARPATH'}.'/lib/';
  use Modeler::Join qw(distribute);

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
