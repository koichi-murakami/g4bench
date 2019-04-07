#!/bin/sh -
# ======================================================================
#  G4Bench benchmark / Vgeo e20
#    Geometry : Voxel water phantom
#    Primary  : electron 20 MeV broad beam
#    Event    : 100k
#
#  Environment variables:
#    G4ENV : bash script for export Geant4 Data files
#    G4BENCH : prefix for G4Bench program
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

check_g4env() {
  if [ ${G4ENV-undef} = "undef" ]; then
    echo "*** Error : G4ENV is not defined."
    exit -1
  fi

  if [ ! -e $G4ENV ]; then
    echo "*** Error : ${G4ENV} does not exist."
    exit -1
  fi
}

# ======================================================================
# main
# ======================================================================
show_line
echo " G4Bench / Vgeo e20"
show_line

check_g4env
echo "@@@ G4ENV = ${G4ENV}"
source ${G4ENV}
echo "@@@ Geant4 Data files :"
env | grep G4 | grep DATA
echo ""

#
cat << EOD > g4bench.conf
{
  Run : {
    Seed : 123456789,
  },
  Primary : {
    type : "beam",
    Beam : {
      particle  : "e-",
      energy    : 20.0,   // MeV
      ssd : 100.,  // SSD (cm)
      field_size : 10.0,   // field size (X/Y) in cm
    }
  }
}
EOD
#
if [ ${G4BENCH-undef} = "undef" ]; then
  G4BENCH=.
fi

if [ $# = 0 ]; then
  ${G4BENCH}/vgeo 100000
else
  log=$1
  ${G4BENCH}/vgeo 100000 > $1 2>&1
  touch done
fi

exit $?
