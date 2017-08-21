/*============================================================================
Copyright 2017 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#ifndef RUN_ACTION_H_
#define RUN_ACTION_H_

#include "G4UserRunAction.hh"

class SimData;

class RunAction : public G4UserRunAction {
public:
  RunAction();
  virtual ~RunAction();

  void SetSimData(SimData* data);

  virtual void BeginOfRunAction(const G4Run* run);
  virtual void EndOfRunAction(const G4Run* run);

  void ShowRunSummary(const G4Run* run);

private:
  SimData* simdata_;

};

// ==========================================================================
inline void RunAction::SetSimData(SimData* data)
{
  simdata_ = data;
}

#endif
