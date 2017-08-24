/*============================================================================
Copyright 2017 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include "simdata.h"

// --------------------------------------------------------------------------
SimData::SimData()
  : step_count_(0), edep_(0.)
{
}

// --------------------------------------------------------------------------
SimData::~SimData()
{
}

// --------------------------------------------------------------------------
void SimData::Initialize()
{
  step_count_ = 0;
  edep_ = 0.;
}
