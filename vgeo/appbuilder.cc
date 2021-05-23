/*============================================================================
Copyright 2017-2021 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include <vector>
#include "QGSP_BIC.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#ifdef ENABLE_MT
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "G4SystemOfUnits.hh"
#include "CLHEP/Random/MTwistEngine.h"
#include "medicalbeam.h"
#include "voxelgeom.h"
#include "common/appbuilder.h"
#include "common/eventaction.h"
#include "common/particlegun.h"
#include "common/runaction.h"
#include "common/simdata.h"
#include "common/stepaction.h"
#include "util/jsonparser.h"

using namespace kut;

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
  auto geom = new VoxelGeom();
  geom-> SetSimData(data);
  ::run_manager-> SetUserInitialization(geom);
}

// --------------------------------------------------------------------------
G4ThreeVector GetPrimaryPosition()
{
  G4ThreeVector pos = G4ThreeVector();
  if ( ::jparser-> Contains("Primary/position") ) {
    std::vector<double> dvec;
    dvec.clear();
    ::jparser-> GetDoubleArray("Primary/Gun/position", dvec);
    pos = G4ThreeVector(dvec[0]*cm, dvec[1]*cm, dvec[2]*cm);
  }
  return pos;
}

// --------------------------------------------------------------------------
G4VUserPrimaryGeneratorAction* SetupParticleGun()
{
  auto pga = new ParticleGun();
  auto gun = pga-> GetGun();

  std::string pname = ::jparser-> GetStringValue("Primary/Gun/particle");
  auto ptable = G4ParticleTable::GetParticleTable();
  auto pdef = ptable-> FindParticle(pname);
  if ( pdef != nullptr ) gun-> SetParticleDefinition(pdef);

  double pkin = ::jparser-> GetDoubleValue("Primary/Gun/energy");
  gun-> SetParticleEnergy(pkin*MeV);

  std::vector<double> dvec;
  if ( ::jparser-> Contains("Primary/direction") ) {
    dvec.clear();
    ::jparser-> GetDoubleArray("Primary/Gun/direction", dvec);
    G4ThreeVector pvec(dvec[0], dvec[1], dvec[2]);
    gun-> SetParticleMomentumDirection(pvec);
  }

  auto pos = GetPrimaryPosition();
  gun-> SetParticlePosition(pos);

  return pga;
}

// --------------------------------------------------------------------------
G4VUserPrimaryGeneratorAction* SetupMedicalBeam()
{
  auto beam = new MedicalBeam();

  std::string pname = ::jparser-> GetStringValue("Primary/Beam/particle");
  if ( pname != "gamma" && pname != "e-" && pname != "proton") {
   std::cout << "[ ERROR ] AppBuilder::SetupMedicalBeam() "
                "invalid particle in setup, " << pname
             << std::endl;
   std::exit(EXIT_FAILURE);
 }

 if ( pname  == "gamma" ) {
   beam-> SetParticle(MedicalBeam::kPhoton);
   int voltage = ::jparser-> GetIntValue("Primary/Beam/photon_voltage");
   beam-> SetPhotonVoltage(voltage);
 } else if ( pname == "e-") {
   beam-> SetParticle(MedicalBeam::kElectron);
   double ekin = ::jparser-> GetDoubleValue("Primary/Beam/energy");
   beam-> SetEnergy(ekin * MeV);
 } else if ( pname == "proton") {
   beam-> SetParticle(MedicalBeam::kProton);
   double ekin = ::jparser-> GetDoubleValue("Primary/Beam/energy");
   beam-> SetEnergy(ekin * MeV);
 }

 // SSD
 if ( ::jparser-> Contains("Primary/Beam/ssd") ) {
   double ssd = ::jparser-> GetDoubleValue("Primary/Beam/ssd");
   beam-> SetSSD(ssd * cm);
 }

 // field size
 if ( ::jparser-> Contains("Primary/Beam/field_size") ) {
   double fxy = ::jparser-> GetDoubleValue("Primary/Beam/field_size");
   beam-> SetFieldSize(fxy * cm);
 }

 return beam;
}

// --------------------------------------------------------------------------
G4VUserPrimaryGeneratorAction* SetupPGA()
{
  G4VUserPrimaryGeneratorAction* pga { nullptr };

  std::string primary_type = ::jparser-> GetStringValue("Primary/type");
  if ( primary_type == "gun" ) {
    std::cout << "[ MESSAGE ] primary type : gun" << std::endl;
    pga = SetupParticleGun();
  } else if ( primary_type == "beam" ) {
    std::cout << "[ MESSAGE ] primary type : beam" << std::endl;
    pga = SetupMedicalBeam();
  } else {
    std::cout << "[ MESSAGE ] primary type : gun" << std::endl;
    pga = SetupParticleGun();
  }

  return pga;
}

} // end of namespace

// ==========================================================================
AppBuilder::AppBuilder()
  : simdata_{nullptr}, nvec_{0}, qtest_{false},
    bench_name_{""}, cpu_name_{""}
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
  CLHEP::MTwistEngine* rand_engine = new CLHEP::MTwistEngine();
  G4Random::setTheEngine(rand_engine);

#ifdef ENABLE_MT
  ::run_manager = G4MTRunManager::GetMasterRunManager();
  nvec_ = ::run_manager-> GetNumberOfThreads();
#else
  ::run_manager = G4RunManager::GetRunManager();
  nvec_ = 1;
#endif

  simdata_ = new SimData[nvec_];

  ::SetupGeomtry(simdata_);
  ::run_manager-> SetUserInitialization(new QGSP_BIC);
  ::run_manager-> SetUserInitialization(this);

  long seed { 0L };
  if ( jparser-> Contains("Run/Seed") ) {
    seed = jparser-> GetLongValue("Run/Seed");
  }

  G4Random::setTheSeed(seed);

  // initialize
  ::run_manager-> Initialize();
}

// --------------------------------------------------------------------------
void AppBuilder::Build() const
{
  auto pga = ::SetupPGA();
  SetUserAction(pga);

  auto runaction = new RunAction();
  runaction-> SetSimData(simdata_);
  runaction-> SetDataSize(nvec_);
  runaction-> SetTestingFlag(qtest_);
  runaction-> SetBenchName(bench_name_);
  runaction-> SetCPUName(cpu_name_);
  SetUserAction(runaction);

  auto eventaction = new EventAction();
  eventaction-> SetCheckCounter(10000);
  SetUserAction(eventaction);

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
