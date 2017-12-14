#!/bin/sh -
# ======================================================================
#  Build & run : ecal
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
cd build/ecal
make -j4
check_error

show_line
echo "@@ Run a program..."
#
export G4NEUTRONHPDATA=/opt/geant4/data/10.4.0/G4NDL4.5
export G4LEDATA=/opt/geant4/data/10.4.0/G4EMLOW7.3
export G4LEVELGAMMADATA=/opt/geant4/data/10.4.0/PhotonEvaporation5.2
export G4RADIOACTIVEDATA=/opt/geant4/data/10.4.0/RadioactiveDecay5.2
export G4NEUTRONXSDATA=/opt/geant4/data/10.4.0/G4NEUTRONXS1.4
export G4PIIDATA=/opt/geant4/data/10.4.0/G4PII1.3
export G4REALSURFACEDATA=/opt/geant4/data/10.4.0/RealSurface2.1
export G4SAIDXSDATA=/opt/geant4/data/10.4.0/G4SAIDDATA1.1
export G4ABLADATA=/opt/geant4/data/10.4.0/G4ABLA3.1
export G4ENSDFSTATEDATA=/opt/geant4/data/10.4.0/G4ENSDFSTATE2.2
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
./ecal 1000

exit $?
