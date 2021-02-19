/*============================================================================
Copyright 2017-2019 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include <getopt.h>
#include <boost/lexical_cast.hpp>
#ifdef ENABLE_MT
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4UItcsh.hh"
#ifdef ENABLE_VIS
#include "G4VisExecutive.hh"
#endif

#include "version.h"
#include "common/appbuilder.h"
#include "util/jsonparser.h"
#include "util/timehistory.h"

namespace {
// --------------------------------------------------------------------------
void show_version()
{
  const char* version_str = G4BENCH_VERSION_MAJOR "."
                            G4BENCH_VERSION_MINOR ".";

  std::cout << "G4Bench/vgeo version 1.5.4"
            << " (" << version_str << ::build_head << "."
            << ::build_tail << ")" << std::endl;
}

// --------------------------------------------------------------------------
void show_help()
{
  const char* message =
R"(
usage:
ecal [options] [#histories]

   -h, --help          show this message.
   -v  --version       show program name/version.
   -c, --config        specify configuration file [g4bench.conf]
   -s, --session=type  specify session type [tcsh]
   -i, --init=macro    specify initial macro
   -n, --nthreads=N    set number of threads in MT mode [1]
   -a, --affinity      set CPU affinity [false]
   -j, --test          make output for CI [false]
   -b, --bench=name    set benchmark name [vgeo]
   -p, --cpu=name      set CPU name [unknown]
)";

   std::cout << message << std::endl;
}

} // end of namespace

// --------------------------------------------------------------------------
int main(int argc, char** argv)
{
  // optional parameters
  bool qhelp = false;
  bool qversion = false;
  std::string session_type = "tcsh";
  std::string init_macro = "";
  std::string config_file = "g4bench.conf";
  std::string str_nhistories = "";
  std::string str_nthreads = "1";
  bool qaffinity = false;
  bool qtest = false;
  std::string str_bench = "vgeo";
  std::string str_cpu = "unknown";

  struct option long_options[] = {
    {"help",       no_argument,        0 ,  'h'},
    {"version",    no_argument,        0 ,  'v'},
    {"config",     required_argument,  0 ,  'c'},
    {"session",    required_argument,  0 ,  's'},
    {"init",       required_argument,  0 ,  'i'},
    {"nthreads",   required_argument,  0,   'n'},
    {"affinity",   no_argument,        0,   'a'},
    {"test",       no_argument,        0,   'j'},
    {0,            0,                  0,    0}
  };

  while (1) {
    int option_index = -1;

    int c = getopt_long(argc, argv, "hvc:s:i:n:ajb:p:",
                        long_options, &option_index);

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
    case 'n' :
      str_nthreads = optarg;
      break;
    case 'a' :
      qaffinity = true;
      break;
    case 'j' :
      qtest = true;
      break;
    case 'b' :
      str_bench = optarg;
      break;
    case 'p' :
      str_cpu = optarg;
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

  // #threads
  int nthreads = 1;
  try {
    nthreads = boost::lexical_cast<int>(str_nthreads);
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    std::cout << "[ ERROR ] invalid argument: <#threads>" << std::endl;
    std::exit(EXIT_FAILURE);
  }
  if(nthreads <= 0 ) {
    std::cout << "[ ERROR ] #threads should be more than 0." << std::endl;
    std::exit(EXIT_FAILURE);
  }

#ifndef ENABLE_MT
  if ( nthreads != 1 || qaffinity ) {
    std::cout << "[ ERROR ] multi-threads is not supported" << std::endl;
    std::exit(EXIT_FAILURE);
  }
#endif

  // #histories
  int nhistories = 0.;
  if ( optind < argc ) {
    str_nhistories = argv[optind];
    try {
      nhistories = boost::lexical_cast<int>(str_nhistories);
    } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
      std::cout << "[ ERROR ] invalid argument: <#histories>" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    if(nhistories <= 0 ) {
      std::cout << "[ ERROR ] #histories should be more than 0." << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }

  // load config
  auto jparser = JsonParser::GetJsonParser();
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
            << "   * # of threads = " << nthreads << std::endl
            << "   * # of histories = " << nhistories
            << std::endl;
  std::cout << "=============================================================="
            << std::endl;

  std::cout << "JSON configuration" << std::endl;
  jparser-> DumpAll();
  std::cout << "=============================================================="
            << std::endl;

  // ----------------------------------------------------------------------
  auto gtimer = TimeHistory::GetTimeHistory();
  gtimer-> ShowClock("[MESSAGE] Start:");

  // G4 managers & setup application
#ifdef ENABLE_MT
  auto run_manager = new G4MTRunManager();
  run_manager-> SetNumberOfThreads(nthreads);
  if ( qaffinity ) run_manager-> SetPinAffinity(nthreads);
#else
  auto run_manager = new G4RunManager();
#endif

  auto ui_manager = G4UImanager::GetUIpointer();

  auto appbuilder = new AppBuilder();
  appbuilder-> SetTestingFlag(qtest, str_bench, str_cpu);
  appbuilder-> BuildApplication();

  // ----------------------------------------------------------------------
#ifdef ENABLE_VIS
  auto vis_manager = new G4VisExecutive("quiet");
  vis_manager-> Initialize();
#endif

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
    auto ui_session = new G4UIExecutive(argc, argv, session_type);
    gtimer-> TakeSplit("SessionStart");
    ui_session-> SetPrompt("vgeo(%s)[%/]:");
    ui_session-> SessionStart();
    gtimer-> TakeSplit("SessionEnd");
    delete ui_session;
  }

  // ----------------------------------------------------------------------
  delete run_manager;

#ifdef ENABLE_VIS
  delete vis_manager;
#endif

  gtimer-> ShowClock("[MESSAGE] End:");

  return EXIT_SUCCESS;
}
