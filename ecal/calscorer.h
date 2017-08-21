/*============================================================================
Copyright 2017 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#ifndef CAL_SCORER_H_
#define CAL_SCORER_H_

#include "G4VSensitiveDetector.hh"

class G4Step;
class SimData;

class CalScorer : public G4VSensitiveDetector {
public:
  CalScorer();
  virtual ~CalScorer();

  virtual bool ProcessHits(G4Step* step, G4TouchableHistory*);

  void SetSimData(SimData* data);

private:
  SimData* simdata_;

};

// ==========================================================================
inline void CalScorer::SetSimData(SimData* data)
{
  simdata_ = data;
}

#endif
