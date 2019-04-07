/*============================================================================
Copyright 2017-2019 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#ifndef HCAL_GEOM_H_
#define HCAL_GEOM_H_

#include "G4VUserDetectorConstruction.hh"

class SimData;

class HcalGeom : public G4VUserDetectorConstruction {
public:
  HcalGeom() = default;
  ~HcalGeom() = default;

  void SetSimData(SimData* data);

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

private:
  SimData* simdata_;

};

// ==========================================================================
inline void HcalGeom::SetSimData(SimData* data)
{
  simdata_ = data;
}

#endif
