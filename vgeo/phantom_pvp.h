/*============================================================================
Copyright 2017-2021 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#ifndef PHANTOM_PVP_H_
#define PHANTOM_PVP_H_

#include "G4VNestedParameterisation.hh"

class G4Material;
class G4VPhysicalVolume;

class PhantomPVP : public G4VNestedParameterisation {
public:
  PhantomPVP();
  ~PhantomPVP() override;

  G4Material* ComputeMaterial(G4VPhysicalVolume* physvol,
                              const int idx,
                              const G4VTouchable* parent = 0) override;

  int GetNumberOfMaterials() const override;

  G4Material* GetMaterial(int idx) const override;

  void ComputeTransformation(const int idx,
                             G4VPhysicalVolume* physvol) const override;

  void SetSegment(int n, double dx);

private:
  int nx_;
  double dx_;

};

// ==========================================================================
inline void PhantomPVP::SetSegment(int n, double dx)
{
  nx_ = n;
  dx_ = dx;
}

#endif
