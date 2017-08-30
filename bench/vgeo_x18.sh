#!/bin/sh -
# ======================================================================
#  G4Bench benchmark / Vgeo x18
#    Geometry : Voxel water phantom
#    Primary  : x-ray 18 MV broad beam
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
echo " G4Bench / Vgeo x18"
show_line

check_g4env
echo "@@@ G4ENV = ${G4ENV}"
source ${G4ENV}
echo "@@@ Geant4 Data files :"
env | grep G4 | grep DATA
echo ""

#
cat << EOD > config.json5
{
  Run : {
    Seed : 123456789,
  },
  Primary : {
    type : "beam",
    Beam : {
      particle  : "gamma",
      photon_voltage : 18,   // photon voltate, [6,18] MV for x-ray beam
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

${G4BENCH}/vgeo 100000

exit $?