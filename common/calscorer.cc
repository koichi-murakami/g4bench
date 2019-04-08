/*============================================================================
Copyright 2017-2019 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include "G4Step.hh"
#include "G4Threading.hh"
#include "common/calscorer.h"
#include "common/simdata.h"

// --------------------------------------------------------------------------
CalScorer::CalScorer()
: G4VSensitiveDetector("calscorer"), simdata_{nullptr}
{
}

// --------------------------------------------------------------------------
bool CalScorer::ProcessHits(G4Step* step, G4TouchableHistory*)
{
#ifdef ENABLE_MT
  int tid = G4Threading::G4GetThreadId();
#else
  int tid = 0;
#endif

  double edep = step-> GetTotalEnergyDeposit();
  simdata_[tid].AddEdep(edep);
}
