#!/bin/sh -
# ======================================================================
#  G4Bench benchmark / Vgeo e20
#    Geometry : Voxel water phantom
#    Primary  : electron 20 MeV broad beam
#    Event    : 100k
#
#  Environment variables:
#    G4DATA : direcotry of Geant4 Data files
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

# ======================================================================
# main
# ======================================================================
show_line
echo " G4Bench / Vgeo e20"
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

sys=`uname`
if [ ${sys} = "Darwin" ]; then
  cpu_info=`sysctl machdep.cpu.brand_string | cut -d : -f 2 | xargs echo`
else
  cpu_info=`lscpu | grep name | cut -d : -f 2 | xargs echo`
fi

if [ $# = 0 ]; then
  ${G4BENCH}/vgeo 100000
else
  log=$1
  ${G4BENCH}/vgeo -j -b vgeo_e20 -p "${cpu_info}" 100000 > $1 2>&1
fi

exit $?
