/*============================================================================
Copyright 2017 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include <getopt.h>
#include <boost/lexical_cast.hpp>
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4StateManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4UItcsh.hh"
#include "appbuilder.h"
#include "version.h"
#include "util/jsonparser.h"
#include "util/timehistory.h"
#ifdef ENABLE_VIS
#include "G4VisExecutive.hh"
#endif

namespace {
// --------------------------------------------------------------------------
void show_version()
{
  const char* version_str = G4BENCH_VERSION_MAJOR "."
                            G4BENCH_VERSION_MINOR ".";

  std::cout << "G4Bench/ecal version 1.1.0"
            << " (" << version_str << ::build_head << "."
            << ::build_tail << ")" << std::endl;
}

// --------------------------------------------------------------------------
void show_help()
{
  std::cout << "G4Bench ecal" << std::endl;
  std::cout << "usage:" << std::endl;
  std::cout << "ecal [options] [#histories]"
            << std::endl << std::endl;
  std::cout << "   -h, --help          show this message." << std::endl
            << "   -v  --version       show program name/version." << std::endl
            << "   -c, --config        "
               "specify configuratioon file [config.json5]" << std::endl
            << "   -s, --session=type  specify session type" << std::endl
            << "   -i, --init=macro    specify initial macro"
            << std::endl;
  std::cout << std::endl;
}

} // end of namespace

// --------------------------------------------------------------------------
int main(int argc, char** argv)
{
  // optional parameters
  bool qhelp = false;
  bool qversion = false;
  std::string session_type = "";
  std::string init_macro = "";
  std::string config_file = "config.json5";
  std::string str_nhistories = "";

  struct option long_options[] = {
    {"help",    no_argument,       NULL, 'h'},
    {"version", no_argument,       NULL, 'v'},
    {"config",  required_argument, NULL, 'c'},
    {"session", required_argument, NULL, 's'},
    {"init",    required_argument, NULL, 'i'},
    {NULL,      0,                 NULL,  0}
  };

  while (1) {
    int option_index = -1;

    int c = getopt_long(argc, argv, "hvc:s:i:", long_options, &option_index);

    if (c == -1) break;

    switch (c) {
    case 'h' :
      qhelp = true;
      break;
    case 'v' :
      qversion = true;
      break;
    case 'c' :
      config_file = optarg;
    case 's' :
      session_type = optarg;
      break;
    case 'i' :
      init_macro = optarg;
      break;
    default:
      std::exit(EXIT_FAILURE);
      break;
    }
  }

  if ( qhelp ) {
    ::show_version();
    ::show_help();
  }

  if ( qversion ) {
    ::show_version();
  }

  if ( qhelp || qversion ) {
    std::exit(EXIT_SUCCESS);
  }

  // #histories
  int nhistories = 0.;
  if ( optind < argc ) {
    str_nhistories = argv[optind];
    try {
      nhistories = boost::lexical_cast<int>(str_nhistories);
    } catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
      std::cerr << "[ ERROR ] invalid argument: <#histories>" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    if(nhistories <= 0 ) {
      std::cout << "[ ERROR ] #histories should be more than 0." << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }

  // load config
  JsonParser* jparser = JsonParser::GetJsonParser();
  bool qload = jparser-> LoadFile(config_file);
  if ( ! qload ) {
    std::cout << "[ ERROR ] failed on loading a config file. "
              << config_file << std::endl;
    std::exit(EXIT_FAILURE);
  }

  // ----------------------------------------------------------------------
  std::cout << "=============================================================="
            << std::endl;
  ::show_version();
  std::cout << "   * config file = " << config_file << std::endl
            << "   * # of histories = " << nhistories
            << std::endl;
  std::cout << "=============================================================="
            << std::endl;

  std::cout << "JSON configuration" << std::endl;
  jparser-> DumpAll();
  std::cout << "=============================================================="
            << std::endl;

  // ----------------------------------------------------------------------
  TimeHistory* gtimer = TimeHistory::GetTimeHistory();
  gtimer-> ShowClock("[MESSAGE] Start:");

  // G4 managers & setup application
  G4RunManager* run_manager = new G4RunManager();
  G4UImanager* ui_manager = G4UImanager::GetUIpointer();

  AppBuilder* appbuilder = new AppBuilder();
  appbuilder-> SetupApplication();

  // ----------------------------------------------------------------------
#ifdef ENABLE_VIS
  G4VisManager* vis_manager = new G4VisExecutive("quiet");
  vis_manager-> Initialize();
#endif

  G4UIExecutive* ui_session = new G4UIExecutive(argc, argv, session_type);

  // do init macro
  if (init_macro != "" ) {
    G4String command = "/control/execute ";
    ui_manager-> ApplyCommand(command + init_macro);
  }

  // start session
  bool qbatch = nhistories > 0;
  if ( qbatch ) {
    gtimer-> TakeSplit("BeamOn");
    run_manager-> BeamOn(nhistories);
    gtimer-> TakeSplit("BeamEnd");

  } else {
    gtimer-> TakeSplit("SessionStart");
    ui_session-> SetPrompt("[40;01;33mecal[40;31m(%s)[40;36m[%/][00;01;30m:");
    ui_session-> SetLsColor(BLUE, RED);
    ui_session-> SessionStart();
    gtimer-> TakeSplit("SessionEnd");
  }

  // ----------------------------------------------------------------------
  delete ui_session;
  delete appbuilder;
  delete run_manager;
#ifdef ENABLE_VIS
  delete vis_manager;
#endif

  gtimer-> ShowAllHistories();
  gtimer-> ShowClock("[MESSAGE] End:");

  return EXIT_SUCCESS;
}
