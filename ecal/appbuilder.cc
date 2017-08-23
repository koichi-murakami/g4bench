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
#include "ecalgeom.h"
#include "particlegun.h"
#include "util/jsonparser.h"
#include "eventaction.h"
#include "runaction.h"
#include "simdata.h"
#include "stepaction.h"

// --------------------------------------------------------------------------
namespace {

G4RunManager* run_manager = nullptr;
G4UImanager* ui_manager = nullptr;
JsonParser* jparser = nullptr;

// --------------------------------------------------------------------------
void SetupGeomtry(SimData* data)
{
  EcalGeom* geom = new EcalGeom();
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
    jparser-> GetDoubleArray("Primary/position", dvec);
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

  std::string pname = jparser-> GetStringValue("Primary/particle");
  G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* pdef = ptable-> FindParticle(pname);
  if ( pdef != nullptr ) gun-> SetParticleDefinition(pdef);

  double pkin = jparser-> GetDoubleValue("Primary/energy");
  gun-> SetParticleEnergy(pkin*MeV);

  std::vector<double> dvec;
  if ( jparser-> Contains("Primary/direction") ) {
    dvec.clear();
    jparser-> GetDoubleArray("Primary/direction", dvec);
    G4ThreeVector pvec(dvec[0], dvec[1], dvec[2]);
    gun-> SetParticleMomentumDirection(pvec);
  }

  G4ThreeVector pos = GetPrimaryPosition();
  gun-> SetParticlePosition(pos);
}

// --------------------------------------------------------------------------
void SetupPGA()
{
  std::cout << "[ MESSAGE ] primary type : gun" << std::endl;
  SetupParticleGun();
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
