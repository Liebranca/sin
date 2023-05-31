#!/usr/bin/perl

  use v5.36.0;
  use strict;
  use warnings;

  use lib $ENV{'ARPATH'}.'/lib/sys/';
  use Shb7;

  use lib $ENV{'ARPATH'}.'/lib/';
  use Avt;

# ---   *   ---   *   ---

Avt::set_config(

  name=>'sin',
  scan=>'-x legacy',

  build=>'ar:sin',

  incl=>[qw(bitter chasm)],
  libs=>[qw(chasm gaoler)],

  post_build=>q{

    chdir Shb7::dir('sin');

    my $ex   = "bin/singl";
    my @call = (

      $ex,

      q[-o],"mesh",

      "mesh/src/Frac.sg",
      "mesh/src/Arstd.sg",

      "mesh/src/CRK_Linear.sg",
      "mesh/src/CRK_Angular.sg",

      "mesh/src/JOJ_Sprite.sg",
      "mesh/src/M3D.sg",

    );

    system {$call[0]} @call;

  },

);

Avt::scan();
Avt::config();
Avt::make();

# ---   *   ---   *   ---
1; # ret
