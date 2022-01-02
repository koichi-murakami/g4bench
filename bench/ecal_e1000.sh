#!/bin/sh -
# ======================================================================
#  G4Bench benchmark / Ecal e1000
#    Geometry : EM calorimeter
#    Primary  : electron 1 GeV
#    Event    : 10k
#
#  Environment variables:
#    G4DATA : direcotry of Geant4 Data files
#    G4BENCH : prefix for G4Bench program
# ======================================================================
export LANG=C

# ======================================================================
# run parameters
# ======================================================================
NEVENTS=20000
LOG=ecal_e1000.log

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
show_line
echo " G4Bench / Ecal e1000"
show_line

#
cat << EOD > g4bench.conf
{
  Run : {
    Seed : 123456789,
    G4DATA : "${G4DATA}"
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
if [ ${sys} = "Darwin" ]; then
  cpu_info=`sysctl machdep.cpu.brand_string | cut -d : -f 2 | xargs echo`
else
  cpu_info=`lscpu | grep name | cut -d : -f 2 | xargs echo`
fi

${G4BENCH}/ecal -j -b ecal_e1000 -p "${cpu_info}" ${NEVENTS} > ${LOG} 2>&1

exit $?
