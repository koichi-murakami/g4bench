/*============================================================================
Copyright 2017-2021 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#ifndef VOXEL_GEOM_H_
#define VOXEL_GEOM_H_

#include "G4VUserDetectorConstruction.hh"

class SimData;

class VoxelGeom : public G4VUserDetectorConstruction {
public:
  VoxelGeom() = default;
  ~VoxelGeom() override = default;

  void SetSimData(SimData* data);

  G4VPhysicalVolume* Construct() override;
  void ConstructSDandField() override;

private:
  SimData* simdata_;

};

// ==========================================================================
inline void VoxelGeom::SetSimData(SimData* data)
{
  simdata_ = data;
}

#endif
