/*============================================================================
Copyright 2017-2019 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include <fstream>
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "common/runaction.h"
#include "common/simdata.h"
#include "util/timehistory.h"

// --------------------------------------------------------------------------
namespace {

TimeHistory* gtimer = nullptr;

// --------------------------------------------------------------------------
void ShowWorkerRunSummary(const G4Run* run)
{
  // # of processed events
  int nevents = run-> GetNumberOfEvent();
  std::cout << " * Worker Summary : #events = " << nevents << std::endl;
}

} // end of namespace

// ==========================================================================
RunAction::RunAction()
  : simdata_{nullptr}, nvec_{0},
    total_step_count_{0}, total_edep_{0.},
    bench_name_{"bench"}, cpu_name_{"cpu"}
{
  ::gtimer = TimeHistory::GetTimeHistory();
}

// --------------------------------------------------------------------------
void RunAction::BeginOfRunAction(const G4Run*)
{
  if (IsMaster()) {
    for ( int i = 0; i < nvec_; i++) {
      simdata_[i].Initialize();
    }

    std::cout << std::endl;
    ::gtimer-> TakeSplit("RunBegin");
  }
}

// --------------------------------------------------------------------------
void RunAction::EndOfRunAction(const G4Run* run)
{
  if (IsMaster()) {
    ::gtimer-> TakeSplit("RunEnd");
    ReduceResult();
    ShowRunSummary(run);
  } else {
    ::ShowWorkerRunSummary(run);
  }
}

// --------------------------------------------------------------------------
void RunAction::ReduceResult()
{
  total_step_count_ = 0;
  total_edep_ = 0.;

  for (int i = 0; i < nvec_; i++ ) {
    total_step_count_ += simdata_[i].GetStepCount();
    total_edep_ += simdata_[i].GetEdep();
  }
}

// --------------------------------------------------------------------------
void RunAction::ShowRunSummary(const G4Run* run) const
{
  // # of processed events
  int nevents_to_be = run-> GetNumberOfEventToBeProcessed();
  int nevents = run-> GetNumberOfEvent();

  // Edep information
  double edep_cal = total_edep_ / nevents / MeV;

  // elapsed time
  double t_start = gtimer-> GetTime("BeamOn");
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
  double sps = total_step_count_ / proc_time * msec;

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

  // testing output
  if ( qtest_ ) {
    std::ofstream outfile("jtest.out", std::ios::out);
    outfile << "EPS1000,  Edep" << std::endl
            << proc_eps*1.e3 << ",  " << edep_cal << std::endl;
    outfile.close();

    // json output
    std::ofstream jsonfile("g4bench.json", std::ios::out);
    jsonfile << "{" << std::endl
             << "  \"name\" : \"" << bench_name_ << "\"," << std::endl
             << "  \"cpu\" : \"" << cpu_name_ << "\"," << std::endl
             << "  \"time\" : " << elapsed_time << "," << std::endl
             << "  \"init\" : " << init_time << "," << std::endl
             << "  \"tpe\" : " << average_time_per_event << "," << std::endl
             << "  \"eps\" : " << proc_eps << "," << std::endl
             << "  \"sps\" : " << sps << "," << std::endl
             << "  \"edep\" : " << edep_cal << std::endl
             << "}" << std::endl;
    jsonfile.close();
  }
}