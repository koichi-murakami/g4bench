/*============================================================================
  CUGEANT4 - CUDA Geant4 Project
  Copyright 2012 [[@copyright]]

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#include <vector>
#include <boost/lexical_cast.hpp>

#include "G4ParticleGun.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"

//#include "analyzer.h"
#include "appbuilder.h"
//#include "eventaction.h"
//#include "gsimgeom.h"
//#include "particlegun.h"
//#include "runaction.h"
//#include "simdata.h"
#include "util/jsonparser.h"

// --------------------------------------------------------------------------
namespace {

G4RunManager* run_manager = nullptr;
G4UImanager* ui_manager = nullptr;
JsonParser* jparser = nullptr;
//Analyzer* analyzer = NULL;

// --------------------------------------------------------------------------
/// setup geometry
void SetupGeomtry(SimData* data)
{
}

// --------------------------------------------------------------------------
// setup particle gun
void SetupParticleGun()
{
  // particle generator
  ParticleGun* pga = new ParticleGun();
  run_manager-> SetUserAction(pga);

  /*
  // set parameters
  G4ParticleGun* gun = pga-> GetGun();
  std::string pname = jparser-> GetStringValue("particle");
  if ( pname != "gamma" && pname != "e+" && pname != "e-" ) {
    std::cout << "[ ERROR ] AppBuilder::SetupParticleGun() "
                 "invalid particle in setup, " << pname
              << std::endl;
    std::exit(EXIT_FAILURE);
  }
  ui_manager-> ApplyCommand("/gun/particle " + pname);

  double pkin = jparser-> GetDoubleValue("energy");
  gun-> SetParticleEnergy(pkin * MeV);

  std::vector<double> dvec;
  if ( jparser-> Contains("direction") ) {
    dvec.clear();
    jparser-> GetDoubleArray("direction", dvec);
    G4ThreeVector pvec(dvec[0], dvec[1], dvec[2]);
    gun-> SetParticleMomentumDirection(pvec);
  }

  if ( jparser-> Contains("position") ) {
    dvec.clear();
    jparser-> GetDoubleArray("position", dvec);
    const double kZoffset = 25.;  // cm
    const double kZlimit = -100.;
    if ( dvec[2] < kZlimit ) {
      std::cout << "[ ERROR ] AppBuilder::SetupParticleGun() "
                   "primary source is out of range (z>=-100cm), "
                << dvec[2] << std::endl;
      std::exit(EXIT_FAILURE);
    }
    std::string spos = boost::lexical_cast<std::string>(dvec[0]) + " "
                  + boost::lexical_cast<std::string>(dvec[1]) + " "
                  + boost::lexical_cast<std::string>(dvec[2]+kZoffset) + " cm";
    ui_manager-> ApplyCommand("/gun/position " + spos);
  }
  */
}

// --------------------------------------------------------------------------
/// setup primary generator action
void SetupPGA()
{
  // primary type
  std::string primary_type = jparser-> GetStringValue("primary_type");
  if ( primary_type == "gun" ) {
    SetupParticleGun();
  } else if ( primary_type == "beam" ) {
    SetupMedicalBeam();
  } else {
    std::cout << "[ ERROR ] invalid primary type. "
              << primary_type << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

} // end of namespace

// ==========================================================================
AppBuilder::AppBuilder()
//  : simdata_(NULL)
{
  JsonParser* jparser = JsonParser::GetJsonParser();
  bool qload = jparser-> LoadFile(config_file);
  if ( ! qload ) {
    std::cout << "[ ERROR ] failed on loading a config file. "
              << config_file << std::endl;
    std::exit(EXIT_FAILURE);
  }

  ::run_manager = G4RunManager::GetRunManager();
  ::ui_manager = G4UImanager::GetUIpointer();
  ::jparser = JsonParser::GetJsonParser();

  //simdata_ = new SimData;
  //::analyzer = Analyzer::GetAnalyzer();
  //::analyzer-> SetSimData(simdata_);

}

// --------------------------------------------------------------------------
AppBuilder::~AppBuilder()
{
  //delete simdata_;
}

// --------------------------------------------------------------------------
void AppBuilder::SetupApplication()
{
  //::SetupGeomtry(simdata_);
  ::run_manager-> SetUserInitialization(new FTFP_BERT);
  //::SetupPGA();

  //RunAction* runaction = new RunAction;
  //::run_manager-> SetUserAction(runaction);

  //EventAction* eventaction = new EventAction;
  //::run_manager-> SetUserAction(eventaction);

  // initialize analyzer (allocate data memory)
  //::analyzer-> Initialize();
}
