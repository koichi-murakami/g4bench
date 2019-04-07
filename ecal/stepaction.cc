/*============================================================================
Copyright 2017-2019 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include "G4Threading.hh"
#include "simdata.h"
#include "stepaction.h"

// --------------------------------------------------------------------------
StepAction::StepAction()
  : simdata_(nullptr)
{
}

// --------------------------------------------------------------------------
void StepAction::UserSteppingAction(const G4Step* step)
{
  int tid = G4Threading::G4GetThreadId();
  simdata_[tid].AddStepCount();
}
