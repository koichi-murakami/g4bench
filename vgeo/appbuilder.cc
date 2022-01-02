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
G4MTRunManager* run_manager {nullptr};
#else
G4RunManager* run_manager {nullptr};
#endif

JsonParser* jparser {nullptr};

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
  auto pos = G4ThreeVector();
  if ( ::jparser-> Contains("Primary/Gun/position") ) {
    std::vector<double> dvec;
    dvec.clear();
    ::jparser-> GetDoubleArray("Primary/Gun/position", dvec);
    pos = G4ThreeVector(dvec[0]*cm, dvec[1]*cm, dvec[2]*cm);
  } else {
    std::cout << "[ WARNING ] AppBuilder::GetPrimaryPosition() "
                 "/Primary/Gun/position Not found in JSON, "
              << std::endl;
  }
  return pos;
}

// --------------------------------------------------------------------------
G4VUserPrimaryGeneratorAction* SetupParticleGun()
{
  auto pga = new ParticleGun();
  auto gun = pga-> GetGun();

  auto pname = ::jparser-> GetStringValue("Primary/Gun/particle");
  auto ptable = G4ParticleTable::GetParticleTable();
  auto pdef = ptable-> FindParticle(pname);
  if ( pdef != nullptr ) gun-> SetParticleDefinition(pdef);

  auto pkin = ::jparser-> GetDoubleValue("Primary/Gun/energy");
  gun-> SetParticleEnergy(pkin*MeV);

  std::vector<double> dvec;
  if ( ::jparser-> Contains("Primary/Gun/direction") ) {
    dvec.clear();
    ::jparser-> GetDoubleArray("Primary/Gun/direction", dvec);
    G4ThreeVector pvec(dvec[0], dvec[1], dvec[2]);
    gun-> SetParticleMomentumDirection(pvec);
  } else {
    std::cout << "[ WARNING ] AppBuilder::SetupParticleGun() "
                 "/Primary/Gun/direction Not found in JSON, "
              << std::endl;
  }

  auto pos = GetPrimaryPosition();
  gun-> SetParticlePosition(pos);

  return pga;
}

// --------------------------------------------------------------------------
G4VUserPrimaryGeneratorAction* SetupMedicalBeam()
{
  auto beam = new MedicalBeam();

  auto pname = ::jparser-> GetStringValue("Primary/Beam/particle");
  if ( pname != "gamma" && pname != "e-" && pname != "proton") {
   std::cout << "[ ERROR ] AppBuilder::SetupMedicalBeam() "
                "invalid particle in setup, " << pname
             << std::endl;
   std::exit(EXIT_FAILURE);
 }

 if ( pname  == "gamma" ) {
   beam-> SetParticle(MedicalBeam::kPhoton);
   auto voltage = ::jparser-> GetIntValue("Primary/Beam/photon_voltage");
   beam-> SetPhotonVoltage(voltage);
 } else if ( pname == "e-") {
   beam-> SetParticle(MedicalBeam::kElectron);
   auto ekin = ::jparser-> GetDoubleValue("Primary/Beam/energy");
   beam-> SetEnergy(ekin * MeV);
 } else if ( pname == "proton") {
   beam-> SetParticle(MedicalBeam::kProton);
   auto ekin = ::jparser-> GetDoubleValue("Primary/Beam/energy");
   beam-> SetEnergy(ekin * MeV);
 }

 // SSD
 if ( ::jparser-> Contains("Primary/Beam/ssd") ) {
   auto ssd = ::jparser-> GetDoubleValue("Primary/Beam/ssd");
   beam-> SetSSD(ssd * cm);
 }

 // field size
 if ( ::jparser-> Contains("Primary/Beam/field_size") ) {
   auto fxy = ::jparser-> GetDoubleValue("Primary/Beam/field_size");
   beam-> SetFieldSize(fxy * cm);
 }

 return beam;
}

// --------------------------------------------------------------------------
G4VUserPrimaryGeneratorAction* SetupPGA()
{
  G4VUserPrimaryGeneratorAction* pga {nullptr};

  auto primary_type = ::jparser-> GetStringValue("Primary/type");
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
  auto rand_engine = new CLHEP::MTwistEngine();
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

#ifdef ENABLE_MT
  if ( jparser-> Contains("Run/SeedOnce") ) {
    if ( jparser-> GetBoolValue("Run/SeedOnce") ) {
      G4MTRunManager::SetSeedOncePerCommunication(1);
    } else {
      G4MTRunManager::SetSeedOncePerCommunication(0);
    }
  }
#endif

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
  runaction-> SetNThreads(nvec_);
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
  runaction-> SetBenchName(bench_name_);
  runaction-> SetCPUName(cpu_name_);
  runaction-> SetNThreads(nvec_);

  SetUserAction(runaction);
}
