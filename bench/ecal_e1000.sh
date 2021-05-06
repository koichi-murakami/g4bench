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
. ${G4ENV}
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

sys=`uname`
if [ ${sys} = "Darwin"]; then
  cpu_info=`sysctl machdep.cpu.brand_string | cut -d : -f 2 | xargs echo`
else
  cpu_info=`lscpu | grep name | cut -d : -f 2 | xargs echo`
fi

if [ $# = 0 ]; then
  ${G4BENCH}/ecal 10000
else
  log=$1
  ${G4BENCH}/ecal -j -b ecal_e1000 -p "${cpu_info}" 10000 > $1 2>&1
fi

exit $?
