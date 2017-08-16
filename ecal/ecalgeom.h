/*============================================================================
Copyright 2017 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#ifndef ECAL_GEOM_H_
#define ECAL_GEOM_H_

#include "G4VUserDetectorConstruction.hh"

class QDetectorConstruction : public G4VUserDetectorConstruction {

public:
  QDetectorConstruction();
  ~QDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();

};

#endif
