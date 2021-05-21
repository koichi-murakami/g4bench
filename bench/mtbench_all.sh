#!/bin/sh -
# ======================================================================
#  G4Bench benchmark (MT)
# ======================================================================
export LANG=C

# ======================================================================
# run parameters
# ======================================================================
G4DATA=/opt/geant4/data/10.7.1

NEVENTS_ECAL=100000
NEVENTS_HCAL=20000
NEVENTS_VGEO_X=10000000
NEVENTS_VGEO_E=10000000
NEVENTS_VGEO_P=400000

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
if [ -d prun ]; then
  echo "[ERROR] prun directory already exists."
  exit -1
fi
mkdir prun
cd prun

show_line
echo " Execute G4Bench tests (MT)"
show_line

# ======================================================================
# ecal
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

../mtbench.sh ../../ecal/ecal ${NEVENTS_ECAL} ecal_e1000

# ======================================================================
# hcal
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

../mtbench.sh ../../hcal/hcal ${NEVENTS_HCAL} hcal_p10

# ======================================================================
# vgeo-x18
show_line
echo " G4Bench / Vgeo x18"
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
      particle  : "gamma",
      photon_voltage : 18,   // photon voltate, [6,18] MV for x-ray beam
      ssd : 100.,  // SSD (cm)
      field_size : 10.0,   // field size (X/Y) in cm
    }
  }
}
EOD

../mtbench.sh ../../vgeo/vgeo ${NEVENTS_VGEO_X} vgeo_x18

# ======================================================================
# vgeo-e20
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
      particle  : "proton",
      energy    : 200.0,   // MeV
      ssd : 100.,  // SSD (cm)
      field_size : 10.0,   // field size (X/Y) in cm
    }
  }
}
EOD

../mtbench.sh ../../vgeo/vgeo ${NEVENTS_VGEO_E} vgeo_e20

# ======================================================================
# vgeo-p200
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

../mtbench.sh ../../vgeo/vgeo ${NEVENTS_VGEO_P} vgeo_p200

# ======================================================================
cd ..
exit $?
