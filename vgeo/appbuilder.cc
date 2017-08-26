/*============================================================================
Copyright 2017 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include <vector>
#include <boost/lexical_cast.hpp>
#include "FTFP_BERT.hh"
#include "G4Navigator.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4TransportationManager.hh"
#include "G4UImanager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "CLHEP/Random/MTwistEngine.h"
//#include "CLHEP/Random/Ranlux64Engine.h"
#include "CLHEP/Random/Random.h"
#include "appbuilder.h"
#include "eventaction.h"
#include "medicalbeam.h"
#include "particlegun.h"
#include "runaction.h"
#include "simdata.h"
#include "stepaction.h"
#include "voxelgeom.h"
#include "util/jsonparser.h"

// --------------------------------------------------------------------------
namespace {

G4RunManager* run_manager = nullptr;
G4UImanager* ui_manager = nullptr;
JsonParser* jparser = nullptr;

// --------------------------------------------------------------------------
void SetupGeomtry(SimData* data)
{
  VoxelGeom* geom = new VoxelGeom();
  geom-> SetSimData(data);
  run_manager-> SetUserInitialization(geom);
}

// --------------------------------------------------------------------------
G4ThreeVector GetPrimaryPosition()
{
  G4ThreeVector pos = G4ThreeVector();
  if ( jparser-> Contains("Primary/position") ) {
    std::vector<double> dvec;
    dvec.clear();
    jparser-> GetDoubleArray("Primary/Gun/position", dvec);
    pos = G4ThreeVector(dvec[0]*cm, dvec[1]*cm, dvec[2]*cm);
  }
  return pos;
}

// --------------------------------------------------------------------------
void SetupParticleGun()
{
  ParticleGun* pga = new ParticleGun();
  run_manager-> SetUserAction(pga);
  G4ParticleGun* gun = pga-> GetGun();

  std::string pname = jparser-> GetStringValue("Primary/Gun/particle");
  G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* pdef = ptable-> FindParticle(pname);
  if ( pdef != nullptr ) gun-> SetParticleDefinition(pdef);

  double pkin = jparser-> GetDoubleValue("Primary/Gun/energy");
  gun-> SetParticleEnergy(pkin*MeV);

  std::vector<double> dvec;
  if ( jparser-> Contains("Primary/direction") ) {
    dvec.clear();
    jparser-> GetDoubleArray("Primary/Gun/direction", dvec);
    G4ThreeVector pvec(dvec[0], dvec[1], dvec[2]);
    gun-> SetParticleMomentumDirection(pvec);
  }

  G4ThreeVector pos = GetPrimaryPosition();
  gun-> SetParticlePosition(pos);
}

// --------------------------------------------------------------------------
void SetupMedicalBeam()
{
  MedicalBeam* beam = new MedicalBeam();
  run_manager-> SetUserAction(beam);

  std::string pname = jparser-> GetStringValue("Primary/Beam/particle");
  if ( pname != "gamma" && pname != "e-" ) {
   std::cout << "[ ERROR ] AppBuilder::SetupMedicalBeam() "
                "invalid particle in setup, " << pname
             << std::endl;
   std::exit(EXIT_FAILURE);
 }
 if ( pname  == "gamma" ) {
   beam-> SetParticle(MedicalBeam::kPhoton);
   int voltage = jparser-> GetIntValue("Primary/Beam/photon_voltage");
   beam-> SetPhotonVoltage(voltage);
 } else if ( pname == "e-") {
   beam-> SetParticle(MedicalBeam::kElectron);
   double ekin = jparser-> GetDoubleValue("Primary/Beam/energy");
   beam-> SetElectronEnergy(ekin * MeV);
 }

 // SSD
 if ( jparser-> Contains("Primary/Beam/ssd") ) {
   double ssd = jparser-> GetDoubleValue("Primary/Beam/ssd");
   beam-> SetSSD(ssd * cm);
 }

 // field size
 if ( jparser-> Contains("Primary/Beam/field_size") ) {
   double fxy = jparser-> GetDoubleValue("Primary/Beam/field_size");
   beam-> SetFieldSize(fxy * cm);
 }
}

// --------------------------------------------------------------------------
void SetupPGA()
{
  std::string primary_type = jparser-> GetStringValue("Primary/type");
  if ( primary_type == "gun" ) {
    std::cout << "[ MESSAGE ] primary type : gun" << std::endl;
    SetupParticleGun();
  } else if ( primary_type == "beam" ) {
    std::cout << "[ MESSAGE ] primary type : beam" << std::endl;
    SetupMedicalBeam();
  } else {
    std::cout << "[ MESSAGE ] primary type : gun" << std::endl;
    SetupParticleGun();
  }
}

} // end of namespace

// ==========================================================================
AppBuilder::AppBuilder()
{
  ::run_manager = G4RunManager::GetRunManager();
  ::ui_manager = G4UImanager::GetUIpointer();
  ::jparser = JsonParser::GetJsonParser();

  simdata_ = new SimData;

  CLHEP::MTwistEngine* rand_engine = new CLHEP::MTwistEngine();
  //CLHEP::Ranlux64Engine* rand_engine = new CLHEP::Ranlux64Engine();

  long seed = 0L;
  if ( jparser-> Contains("Run/Seed") ) {
    long seed = jparser-> GetLongValue("Run/Seed");
  }

  const int kK = 12345;
  rand_engine-> setSeed(seed, kK);
  CLHEP::HepRandom::setTheEngine(rand_engine);
}

// --------------------------------------------------------------------------
AppBuilder::~AppBuilder()
{
  delete simdata_;
}

// --------------------------------------------------------------------------
void AppBuilder::SetupApplication()
{
  ::SetupGeomtry(simdata_);
  ::run_manager-> SetUserInitialization(new FTFP_BERT);
  ::SetupPGA();

  RunAction* runaction = new RunAction;
  runaction-> SetSimData(simdata_);
  ::run_manager-> SetUserAction(runaction);


  EventAction* eventaction = new EventAction;
  ::run_manager-> SetUserAction(eventaction);

  StepAction* stepaction = new StepAction;
  stepaction-> SetSimData(simdata_);
  ::run_manager-> SetUserAction(stepaction);

  ::run_manager-> Initialize();

  G4ThreeVector pos = ::GetPrimaryPosition();
  bool qcheck = CheckVPrimaryPosition(pos);
  if ( qcheck == false ) {
    std::cout << "[ ERROR ] primary position out of world." << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

// --------------------------------------------------------------------------
bool AppBuilder::CheckVPrimaryPosition(const G4ThreeVector& pos)
{
  G4Navigator* navigator = G4TransportationManager::GetTransportationManager()
                             -> GetNavigatorForTracking();

  G4VPhysicalVolume* world = navigator-> GetWorldVolume();
  G4VSolid* solid = world-> GetLogicalVolume()-> GetSolid();
  EInside qinside = solid-> Inside(pos);

   if( qinside != kInside) return false;
   else return true;
}
