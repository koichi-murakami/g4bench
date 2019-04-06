# Installation

## Dependencies
* `Geant4` : `v10.5` is latest
* `Boost-C++` : >= 1.53
* `CMake` : >= 3.11
* `GCC` : >= 4.8.5, C++11 support

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

## Running
You need to set Geant4 environment variables to specify the paths for
data tables, to run applications.

~~~~
$ env | grep G4
G4DATA=/opt/geant4/data/10.5.0
G4ABLADATA=/opt/geant4/data/10.5.0/G4ABLA3.1
G4LEDATA=/opt/geant4/data/10.5.0/G4EMLOW7.7
G4ENSDFSTATEDATA=/opt/geant4/data/10.5.0/G4ENSDFSTATE2.2
G4INCLDATA=/opt/geant4/data/10.5.0/G4INCL1.0
G4NEUTRONHPDATA=/opt/geant4/data/10.5.0/G4NDL4.5
G4PARTICLEXSDATA=/opt/geant4/data/10.5.0/G4PARTICLEXS1.1
G4PIIDATA=/opt/geant4/data/10.5.0/G4PII1.3
G4SAIDXSDATA=/opt/geant4/data/10.5.0/G4SAIDDATA2.0
G4LEVELGAMMADATA=/opt/geant4/data/10.5.0/PhotonEvaporation5.3
G4RADIOACTIVEDATA=/opt/geant4/data/10.5.0/RadioactiveDecay5.3
G4REALSURFACEDATA=/opt/geant4/data/10.5.0/RealSurface2.1.1
~~~~