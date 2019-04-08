/*============================================================================
Copyright 2017-2019 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include "G4NistManager.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "phantom_pvp.h"

namespace {

G4Material* water = nullptr;

} // end of namespace

// --------------------------------------------------------------------------
PhantomPVP::PhantomPVP()
  : nx_{0}, dx_{0.}
{
  auto nist_manager = G4NistManager::Instance();
  ::water = nist_manager-> FindOrBuildMaterial("G4_WATER");
}

// --------------------------------------------------------------------------
PhantomPVP::~PhantomPVP()
{
}

// --------------------------------------------------------------------------
G4Material* PhantomPVP::ComputeMaterial(G4VPhysicalVolume* physvol,
                                        const int idx,
                                        const G4VTouchable* parent)
{
  return ::water;
}

// --------------------------------------------------------------------------
int PhantomPVP::GetNumberOfMaterials() const
{
  return 1;
}

// --------------------------------------------------------------------------
G4Material* PhantomPVP::GetMaterial(int idx) const
{
  return ::water;
}

// --------------------------------------------------------------------------
void PhantomPVP::ComputeTransformation(const int idx,
                                       G4VPhysicalVolume* physvol) const
{
  double x = dx_ * ( -nx_/2. + idx + 0.5 );
  auto vec = G4ThreeVector(x, 0., 0.);
  physvol-> SetTranslation(vec);
}
