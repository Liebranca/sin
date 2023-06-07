#!/usr/bin/perl
# ---   *   ---   *   ---

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
  libs=>[qw(gaoler)],

# ---   *   ---   *   ---
# pre-processor bootstrap
#
# needed for singl to run
# on a first build
#
# ie: this saves you having
# to do the copying manually

  pre_build=>q{

    chdir Shb7::dir('sin');

    # lib => source file
    my $deps={

      '../lib/Grammar/SinGL.pm' =>
      './Grammar/SinGL.pm',

      '../lib/Emit/SinGL.pm' =>
      './Emit/SinGL.pm',

    };

    # ^copy missing/updated
    for my $dst(keys %$deps) {

      my $src=$deps->{$dst};

      `cp $src $dst`
      if Shb7::moo($dst,$src);

    };

# ---   *   ---   *   ---
# ^use preprocessor to generate
# hpp_sg files from glsl source

    my $ex="bin/singl";

# ---   *   ---   *   ---
# UI draw shaders

    my @call=(

      $ex,

      q[-o],"font",

      "font/src/Color.sg",
      "font/src/Chars.sg",
      "font/src/Raster.sg",

    );

    system {$call[0]} @call;

# ---   *   ---   *   ---
# viewport draw shaders

    @call=(

      $ex,

      q[-o],"mesh",

      "mesh/src/Frac.sg",
      "mesh/src/Arstd.sg",

      "mesh/src/CRK_Linear.sg",
      "mesh/src/CRK_Angular.sg",

      "mesh/src/Debug_Line.sg",

      "mesh/src/JOJ_Sprite.sg",
      "mesh/src/M3D.sg",

    );

    system {$call[0]} @call;

  },

# ---   *   ---   *   ---

);

Avt::scan();
Avt::config();
Avt::make();

# ---   *   ---   *   ---
1; # ret
