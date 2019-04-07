/*============================================================================
Copyright 2017-2019 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include <vector>
#include <boost/lexical_cast.hpp>
#include "FTFP_BERT.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#ifdef ENABLE_MT
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "G4SystemOfUnits.hh"
#include "CLHEP/Random/MTwistEngine.h"
#include "appbuilder.h"
#include "ecalgeom.h"
#include "eventaction.h"
#include "particlegun.h"
#include "runaction.h"
#include "simdata.h"
#include "stepaction.h"
#include "util/jsonparser.h"

// --------------------------------------------------------------------------
namespace {

#ifdef ENABLE_MT
G4MTRunManager* run_manager = nullptr;
#else
G4RunManager* run_manager = nullptr;
#endif

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
  auto pos = G4ThreeVector();
  if ( jparser-> Contains("Primary/position") ) {
    std::vector<double> dvec;
    dvec.clear();
    jparser-> GetDoubleArray("Primary/position", dvec);
    pos = G4ThreeVector(dvec[0]*cm, dvec[1]*cm, dvec[2]*cm);
  }
  return pos;
}

// --------------------------------------------------------------------------
void SetupParticleGun(ParticleGun* pga)
{
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

} // end of namespace

// ==========================================================================
AppBuilder::AppBuilder()
  : simdata_{nullptr}, nvec_{0}, qtest_{false}
{
  ::jparser = JsonParser::GetJsonParser();
}

// --------------------------------------------------------------------------
AppBuilder::~AppBuilder()
{
  delete [] simdata_;
}

// --------------------------------------------------------------------------
void AppBuilder::BuildApplication()
{
#ifdef ENABLE_MT
  ::run_manager = G4MTRunManager::GetMasterRunManager();
  nvec_ = ::run_manager-> GetNumberOfThreads();
#else
  ::run_manager = G4RunManager::GetRunManager();
  nvec_ = 1;
#endif

  simdata_ = new SimData[nvec_];

  ::SetupGeomtry(simdata_);
  ::run_manager-> SetUserInitialization(new FTFP_BERT);
  ::run_manager-> SetUserInitialization(this);

  // set random number generator
  CLHEP::MTwistEngine* rand_engine = new CLHEP::MTwistEngine();

  long seed { 0L };
  if ( jparser-> Contains("Run/Seed") ) {
    long seed = jparser-> GetLongValue("Run/Seed");
  }

  G4Random::setTheEngine(rand_engine);
  G4Random::setTheSeed(seed);

  // initialize
  ::run_manager-> Initialize();
}

// --------------------------------------------------------------------------
void AppBuilder::Build() const
{
  auto pga = new ParticleGun();
  ::SetupParticleGun(pga);
  SetUserAction(pga);

  auto runaction = new RunAction();
  runaction-> SetSimData(simdata_);
  runaction-> SetDataSize(nvec_);
  runaction-> SetTestingFlag(qtest_);
  SetUserAction(runaction);

  SetUserAction(new EventAction);

  auto stepaction = new StepAction;
  stepaction-> SetSimData(simdata_);
  SetUserAction(stepaction);
}

// --------------------------------------------------------------------------
void AppBuilder::BuildForMaster() const
{
  auto runaction = new RunAction();
  runaction-> SetSimData(simdata_);
  runaction-> SetDataSize(nvec_);
  runaction-> SetTestingFlag(qtest_);

  SetUserAction(runaction);
}
