/*============================================================================
Copyright 2017-2021 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#ifndef RUN_ACTION_H_
#define RUN_ACTION_H_

#include <string>
#include "G4UserRunAction.hh"

class SimData;

class RunAction : public G4UserRunAction {
public:
  RunAction();
  ~RunAction() override = default;

  void SetSimData(SimData* data);
  void SetDataSize(int n);
  void SetTestingFlag(bool val);

  void BeginOfRunAction(const G4Run* run) override;
  void EndOfRunAction(const G4Run* run) override;

  void ReduceResult();

  void ShowRunSummary(const G4Run* run) const;

  void SetBenchName(const std::string& name);
  void SetCPUName(const std::string& name);
  void SetNThreads(int nt);

private:
  SimData* simdata_;
  int nvec_;
  bool qtest_;

  long total_step_count_;
  double total_edep_;

  std::string bench_name_;
  std::string cpu_name_;
  int nthreads_;
};

// ==========================================================================
inline void RunAction::SetSimData(SimData* data)
{
  simdata_ = data;
}

inline void RunAction::SetDataSize(int n)
{
  nvec_ = n;
}

inline void RunAction::SetTestingFlag(bool val)
{
  qtest_ = val;
}

inline void RunAction::SetBenchName(const std::string& name)
{
  bench_name_ = name;
}

inline void RunAction::SetCPUName(const std::string& name)
{
  cpu_name_ = name;
}

inline void RunAction::SetNThreads(int nt)
{
  nthreads_ = nt;
}

#endif
