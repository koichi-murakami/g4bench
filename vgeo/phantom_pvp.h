/*============================================================================
Copyright 2017 Koichi Murakami

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
  virtual ~PhantomPVP();

  virtual G4Material* ComputeMaterial(G4VPhysicalVolume* physvol,
                                      const int idx,
                                      const G4VTouchable* parent = 0);

  virtual int GetNumberOfMaterials() const;

  virtual G4Material* GetMaterial(int idx) const;

  virtual void ComputeTransformation(const int idx,
                                     G4VPhysicalVolume* physvol) const;

  void SetSegment(int n, double dx);

private:
  // segment info
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
