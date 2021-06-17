# Installation

## Dependencies
* `Geant4` : `v10.7` is latest
* `CMake` : >= 3.11
* `GCC` : >= 8.3, C++11 support

## How to install
There is a `configure` script wrapping `CMakeLists.txt`. It can take several options to set building parameters.
~~~~
$ ./configure --help

`configure' cmake configure script for G4Bench apps.

Usage: configure [OPTION]... [VAR=VALUE]...

Options:
  -h, --help                display this help and exit

Installation directories:
  --prefix=PREFIX           installation prefix [config.cmake]

Fine tuning of the library build:
  --with-geant4-dir=DIR     Geant4 installed dir [config.cmake]
  --with-build-dir=DIR      Set build dir [build]

Enable/disable options: prefix with either --enable- or --disable-
  mt       multi-threading support [disable]
  vis      OpenGL support [enable]
  opt      optimization (O3) [enable]
  debug    debug mode [disable]
  dev      development mode [config.cmake/disable]

After configuration is done,
cd build
make
make install
~~~~

1. Run `configure` script with proper options, that creates a `build` directory to compile applications.
2. Move to `build` directory, then do `make` and `make install`.

~~~~
$ ./configure
...
$ cd build
$ make
...
$ make install
...
~~~~

## How to Run
Each benchmark program has the following command line options.
~~~~
G4Bench/ecal version 1.5.0 (1.5.0d1.13098.3115)

usage:
ecal [options] [#histories]

   -h, --help          show this message.
   -v  --version       show program name/version.
   -c, --config        specify configuration file [g4bench.conf]
   -s, --session=type  specify session type
   -i, --init=macro    specify initial macro
   -n, --nthreads=N    set number of threads in MT mode [1]
   -a, --affinity      set CPU affinity [false]
   -j, --test          make output for CI [false]
~~~~

These programs have multi-threading capability if you use a MT version of
Geant4. You can specify the number of threads with `-n` option.
Also you can pin the CPU affinity with `-a` option, but
take fully care for the CPU architecture of your machine, e.g.
*hyper-threading*.

The configuration of an application is described in a JSON5 configuration
file (`g4bench.conf` by default).

~~~~
{
  // -----------------------------------------------------------------
  // Run Configuration
  Run : {
    Seed : 123456789,
    G4DATA : "/opt/geant4/data"
  },
  // -----------------------------------------------------------------
  // Primary setting (Generic)
  Primary : {
    particle  : "e-",
    energy    : 1000.0,   // MeV
    position  : [ 0., 0., -45. ],  // cm
    direction : [ 0., 0., 1.],
  }
}
~~~~

In the configuration file, random number seed and
the condition of primary particles can be modified as simulation parameters.
Also you need to specify the Geant4 data directory in the configuration file
with `G4DATA` key.
The program loads the corresponding data libraries according to
a Geant4 version automatically.
Note that you can set Geant4 environment variables in the
conventional way.
In that case, the environment variables override the above setting.


## Results
Each program shows the following benchmark values.

~~~~
==============================================================
 Run Summary
 - # events processd = 10000 / 10000
 - elapsed cpu time = 69.92 sec
 - initialization time = 0.64 sec
 *** Physics regression ***
 - edep in cal per event = 968.754 MeV/event
 *** EPS Score ***
 - average TPE = 6.992 msec
 - processed TPE = 6.928 msec
 - processed EPS = 0.144342 /msec
 *** SPS Score ***
 - steps per msec = 628.371 steps/msec
==============================================================
~~~~

The important metrics are:

* EPS : Events per second
* TPE : Time per event
* SPS : Steps per second

To validate physics results as regression test, the energy deposit per event
in the scoring volume is also shown. Keep monitoring these values to
ensure the benchmark validity.
