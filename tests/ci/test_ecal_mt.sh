#!/bin/sh -
# ======================================================================
#  Build & run : ecal (MT)
# ======================================================================
export LANG=C

# ======================================================================
# functions
# ======================================================================
check_error() {
  if [ $? -ne 0 ]; then
    exit -1
  fi
}

show_line() {
echo "========================================================================"
}

# ======================================================================
# main
# ======================================================================
. ./tests/ci/g4version.sh

show_line
echo "@@ Configure a program..."
./configure --with-geant4-dir=/opt/geant4/${G4VERSION}-mt --enable-mt --disable-vis

show_line
echo "@@ Build a program..."
cd build/ecal
make -j4
check_error

show_line
echo "@@ Run a program..."
export G4DATA=/opt/geant4/data/${G4VERSION}
. ../../tests/g4env/g4env.${G4VERSION}.sh

#
./ecal -n 10 -j 100000

exit $?
