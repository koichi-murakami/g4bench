#!/bin/sh -
# ======================================================================
#  G4Bench benchmark / Ecal e1000
#    Geometry : EM calorimeter
#    Primary  : electron 1 GeV
#    Event    : 10k
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
echo " G4Bench / Ecal e1000"
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
    particle  : "e-",
    energy    : 1000.0,   // MeV
    position  : [ 0., 0., -45. ],  // cm
    direction : [ 0., 0., 1.],
  }
}
EOD
#
if [ ${G4BENCH-undef} = "undef" ]; then
  G4BENCH=.
fi

${G4BENCH}/ecal 10000

exit $?
