#!/bin/sh -
# ======================================================================
#  G4Bench benchmark / Vgeo p200
#    Geometry : Voxel water phantom
#    Primary  : proton 200 MeV broad beam
#    Event    : 100k
#
#  Environment variables:
#    G4DATA : direcotry of Geant4 Data files
#    G4BENCH : prefix for G4Bench program
# ======================================================================
export LANG=C

# ======================================================================
# run parameters
# ======================================================================
NEVENTS=200000
LOG=vgeo_p200.log

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
echo " G4Bench / Vgeo p200"
show_line

#
cat << EOD > g4bench.conf
{
  Run : {
    Seed : 123456789,
    G4DATA : "${G4DATA}"
  },
  Primary : {
    type : "beam",
    Beam : {
      particle  : "proton",
      energy    : 200.0,   // MeV
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

sys=`uname`
if [ ${sys} = "Darwin" ]; then
  cpu_info=`sysctl machdep.cpu.brand_string | cut -d : -f 2 | xargs echo`
else
  cpu_info=`lscpu | grep name | cut -d : -f 2 | xargs echo`
fi

${G4BENCH}/vgeo -j -b vgeo_p200 -p "${cpu_info}" ${NEVENTS} > ${LOG} 2>&1

exit $?
