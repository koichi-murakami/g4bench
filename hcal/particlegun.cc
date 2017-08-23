/*============================================================================
Copyright 2017 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include "G4ParticleGun.hh"
#include "particlegun.h"

// --------------------------------------------------------------------------
ParticleGun::ParticleGun()
  : gun_(nullptr)
{
  gun_ = new G4ParticleGun();
}

// --------------------------------------------------------------------------
ParticleGun::~ParticleGun()
{
  delete gun_;
}

// --------------------------------------------------------------------------
void ParticleGun::GeneratePrimaries(G4Event* event)
{
  gun_-> GeneratePrimaryVertex(event);
}
