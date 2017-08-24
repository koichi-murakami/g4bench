/*============================================================================
Copyright 2017 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "runaction.h"
#include "simdata.h"
#include "util/timehistory.h"

// --------------------------------------------------------------------------
namespace {

TimeHistory* gtimer = nullptr;

} // end of namespace

// --------------------------------------------------------------------------
RunAction::RunAction()
  : simdata_(nullptr)
{
  ::gtimer = TimeHistory::GetTimeHistory();
}

// --------------------------------------------------------------------------
RunAction::~RunAction()
{
}

// --------------------------------------------------------------------------
void RunAction::BeginOfRunAction(const G4Run*)
{
  simdata_-> Initialize();

  std::cout << std::endl;
  ::gtimer-> TakeSplit("RunBegin");
}

// --------------------------------------------------------------------------
void RunAction::EndOfRunAction(const G4Run* run)
{
  ::gtimer-> TakeSplit("RunEnd");
  ShowRunSummary(run);
}

// --------------------------------------------------------------------------
void RunAction::ShowRunSummary(const G4Run* run)
{
  // # of processed events
  int nevents_to_be = run-> GetNumberOfEventToBeProcessed();
  int nevents = run-> GetNumberOfEvent();

  // Edep information
  double edep_cal = simdata_-> GetEdep() / nevents / MeV;

  // elapsed time
  double t_start = 0;
  if ( gtimer-> FindAKey("BeamOn") ) {
    t_start = gtimer-> GetTime("BeamOn");
  } else {
    t_start = gtimer-> GetTime("RunStart");
  }
  double t_end = gtimer-> GetTime("RunEnd");
  double elapsed_time = t_end - t_start;

  // initialization time
  double t_event0 = gtimer-> GetTime("FirstEventStart");
  double init_time = t_event0 - t_start;

  // event processing time
  double proc_time = elapsed_time - init_time;

  // time/event (msec)
  const double msec = 1.e-3;
  double average_time_per_event = elapsed_time / nevents / msec;
  double proc_time_per_event = proc_time / nevents / msec ;

  // events/msec
  double proc_eps = nevents / proc_time * msec;

  // steps/msec
  double sps = simdata_-> GetStepCount() / proc_time * msec;


  std::cout << std::endl;
  std::cout << "=============================================================="
            << std::endl;
  std::cout << " Run Summary" << std::endl
            << " - # events processd = " << nevents << " / "
            << nevents_to_be << std::endl
            << " - elapsed cpu time = " << elapsed_time << " sec" << std::endl
            << " - initialization time = " << init_time << " sec" << std::endl
            << " *** Physics regression ***" << std::endl
            << " - edep in cal per event = " << edep_cal << " MeV/event"
            << std::endl
            << " *** EPS Score ***" << std::endl
            << " - average TPE = " << average_time_per_event
            << " msec" << std::endl
            << " - processed TPE = " << proc_time_per_event
            << " msec" << std::endl
            << " - processed EPS = " << proc_eps << " /msec" << std::endl
            <<" *** SPS Score ***" << std::endl
            << " - steps per msec = " << sps << " steps/msec"
            << std::endl;
  std::cout << "=============================================================="
            << std::endl << std::endl;
}
