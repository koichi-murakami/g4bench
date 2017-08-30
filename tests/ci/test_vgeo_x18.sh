#!/bin/sh -
# ======================================================================
#  Build & run : vgeo / X-ray 18MV
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
echo "@@ Build a program..."
cd build/vgeo
make -j4
check_error

show_line
echo "@@ Run a program..."
#
export G4NEUTRONHPDATA=/opt/geant4/data/10.3.2/G4NDL4.5
export G4LEDATA=/opt/geant4/data/10.3.2/G4EMLOW6.50
export G4LEVELGAMMADATA=/opt/geant4/data/10.3.2/PhotonEvaporation4.3.2
export G4RADIOACTIVEDATA=/opt/geant4/data/10.3.2/RadioactiveDecay5.1.1
export G4NEUTRONXSDATA=/opt/geant4/data/10.3.2/G4NEUTRONXS1.4
export G4PIIDATA=/opt/geant4/data/10.3.2/G4PII1.3
export G4REALSURFACEDATA=/opt/geant4/data/10.3.2/RealSurface1.0
export G4SAIDXSDATA=/opt/geant4/data/10.3.2/G4SAIDDATA1.1
export G4ABLADATA=/opt/geant4/data/10.3.2/G4ABLA3.0
export G4ENSDFSTATEDATA=/opt/geant4/data/10.3.2/G4ENSDFSTATE2.1
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
./vgeo 100000

exit $?