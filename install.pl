#!/usr/bin/perl
# ---   *   ---   *   ---

  use v5.36.0;
  use strict;
  use warnings;

  use Readonly;

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

  pre_build=>q{
    system $ENV{'ARPATH'}
    . '/sin/bin/build-prologue'
    ;

  },

);

Avt::scan();
Avt::config();
Avt::make();

# ---   *   ---   *   ---
1; # ret
