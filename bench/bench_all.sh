#!/bin/sh -
# ======================================================================
#  G4Bench benchmark
#
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
echo " Execute G4Bench tests"
show_line

./ecal_e1000.sh ecal_e1000.log
mv g4bench.json ecal.json

./hcal_p10.sh hcal_p10.log
mv g4bench.json hcal.json

./vgeo_x18.sh vgeo_x18.log
mv g4bench.json vgeo_x18.json

./vgeo_e20.sh vgeo_e20.log
mv g4bench.json vgeo_e20.json

exit $?
