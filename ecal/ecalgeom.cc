/*============================================================================
Copyright 2017 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "ecalgeom.h"

// --------------------------------------------------------------------------
EcalGeom::EcalGeom()
{
}

// --------------------------------------------------------------------------
EcalGeom::~EcalGeom()
{
}


// --------------------------------------------------------------------------
G4VPhysicalVolume* EcalGeom::Construct()
{
  G4NistManager* nist_manager = G4NistManager::Instance();
  G4VisAttributes* va = nullptr;

  // world volume
  const double kDXYZ_World = 100.*cm;
  G4Box* world_box = new G4Box("world", kDXYZ_World/2.,
                                        kDXYZ_World/2., kDXYZ_World/2.);

  G4Material* air = nist_manager-> FindOrBuildMaterial("G4_AIR");
  G4LogicalVolume* world_lv = new G4LogicalVolume(world_box, air, "world");

  G4PVPlacement* world_pv = new G4PVPlacement(nullptr, G4ThreeVector(), "world",
                                              world_lv, 0, false, 0);

  va = new G4VisAttributes(G4Color(1.,1.,1.));
  va-> SetVisibility(true);
  world_lv-> SetVisAttributes(va);

  // tracker


  return world_pv;
}
