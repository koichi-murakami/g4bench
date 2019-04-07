#!/bin/sh -
# ======================================================================
#  G4Bench benchmark / Hcal p10
#    Geometry : Hadron calorimeter
#    Primary  : proton 10 GeV
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
echo " G4Bench / Hcal p10"
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
    particle  : "proton",
    energy    : 10000.0,   // MeV
    position  : [ 0., 0., -70. ],  // cm
    direction : [ 0., 0., 1.],
  }
}
EOD
#
if [ ${G4BENCH-undef} = "undef" ]; then
  G4BENCH=.
fi

if [ $# = 0 ]; then
  ${G4BENCH}/hcal 10000
else
  log=$1
  ${G4BENCH}/hcal 10000 > $1 2>&1
  touch done
fi

exit $?
