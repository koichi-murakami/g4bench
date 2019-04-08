/*============================================================================
Copyright 2017-2019 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include "G4Threading.hh"
#include "common/simdata.h"
#include "common/stepaction.h"

// --------------------------------------------------------------------------
StepAction::StepAction()
  : simdata_{nullptr}
{
}

// --------------------------------------------------------------------------
void StepAction::UserSteppingAction(const G4Step* step)
{
#ifdef ENABLE_MT
  int tid = G4Threading::G4GetThreadId();
#else
  int tid = 0;
#endif

  simdata_[tid].AddStepCount();
}
