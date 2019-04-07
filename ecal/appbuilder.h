/*============================================================================
Copyright 2017-2019 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#ifndef APP_BUILDER_H_
#define APP_BUILDER_H_

#include "G4VUserActionInitialization.hh"

class SimData;

class AppBuilder : G4VUserActionInitialization {
public:
  AppBuilder();
  ~AppBuilder();

  void BuildApplication();

  void SetTestingFlag(bool val);

  virtual void Build() const;
  virtual void BuildForMaster() const;

private:
  SimData* simdata_;
  int nvec_;
  bool qtest_;
};

// ==========================================================================
inline void AppBuilder::SetTestingFlag(bool val)
{
  qtest_ = val;
}

#endif
