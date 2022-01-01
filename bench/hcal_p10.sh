#!/bin/sh -
# ======================================================================
#  G4Bench benchmark / Hcal p10
#    Geometry : Hadron calorimeter
#    Primary  : proton 10 GeV
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
NEVENTS=4000
LOG=hcal_p10.log

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
echo " G4Bench / Hcal p10"
show_line

#
cat << EOD > g4bench.conf
{
  Run : {
    Seed : 123456789,
    G4DATA : "${G4DATA}"
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

sys=`uname`
if [ ${sys} = "Darwin" ]; then
  cpu_info=`sysctl machdep.cpu.brand_string | cut -d : -f 2 | xargs echo`
else
  cpu_info=`lscpu | grep name | cut -d : -f 2 | xargs echo`
fi

${G4BENCH}/hcal -j -b hcal_p10 -p "${cpu_info}" ${NEVENTS} > ${LOG} 2>&1

exit $?
