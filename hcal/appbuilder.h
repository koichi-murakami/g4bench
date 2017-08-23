/*============================================================================
Copyright 2017 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#ifndef APP_BUILDER_H_
#define APP_BUILDER_H_

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

class SimData;

class AppBuilder {
public:
  AppBuilder();
  ~AppBuilder();

  void SetupApplication();

private:
  DISALLOW_COPY_AND_ASSIGN(AppBuilder);

  SimData* simdata_;

  bool CheckVPrimaryPosition(const G4ThreeVector& pos);

};

#endif
