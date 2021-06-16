#!/bin/sh -
# ======================================================================
#  Build & run : hcal (MT)
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

if [ -z $NOG4VERSION ]; then
  g4path=/opt/geant4/${G4VERSION}-mt
else
  g4path=/opt/geant4
fi

show_line
echo "@@ Configure a program..."
./configure --with-geant4-dir=${g4path} --enable-mt --disable-vis

show_line
echo "@@ Build a program..."
cd build/hcal
make -j4
check_error

show_line
echo "@@ Run a program..."
./hcal -n 10 -j 20000

exit $?
