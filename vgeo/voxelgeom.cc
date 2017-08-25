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
#include "G4VisAttributes.hh"
#include "voxelgeom.h"
#include "calscorer.h"

// --------------------------------------------------------------------------
VoxelGeom::VoxelGeom()
{
}

// --------------------------------------------------------------------------
VoxelGeom::~VoxelGeom()
{
}

// --------------------------------------------------------------------------
G4VPhysicalVolume* VoxelGeom::Construct()
{
  G4NistManager* nist_manager = G4NistManager::Instance();

  // world volume
  const double kDXY_World = 50.*cm;
  const double kDZ_World = 150.*cm;
  G4Box* world_box = new G4Box("world", kDXY_World/2., kDXY_World/2.,
                                kDZ_World/2.);
  G4Material* air = nist_manager-> FindOrBuildMaterial("G4_AIR");
  G4LogicalVolume* world_lv = new G4LogicalVolume(world_box, air, "world");
  G4PVPlacement* world_pv = new G4PVPlacement(nullptr, G4ThreeVector(), "world",
                                              world_lv, nullptr, false, 0);

  // phantom
  const double kDXY_Phantom = 30.5*cm;
  const double kDZ_Phantom = 30.*cm;
  const double kZ_Phantom = 50.*cm;

  G4Box* phantom_box = new G4Box("phantom", kDXY_Phantom/2., kDXY_Phantom/2.,
                                            kDZ_Phantom/2.);
  G4Material* water = nist_manager-> FindOrBuildMaterial("G4_WATER");
  G4LogicalVolume* phantom_lv = new G4LogicalVolume(phantom_box,
                                                    water, "phantom");
  G4PVPlacement* phantom =
    new G4PVPlacement(0, G4ThreeVector(0., 0., kZ_Phantom), phantom_lv,
                      "phantom", world_lv, false, 0);





  CalScorer* cal_scorer = new CalScorer();
  cal_scorer-> SetSimData(simdata_);
  //cal_lv-> SetSensitiveDetector(cal_scorer);

  // vis attributes
  G4VisAttributes* va = nullptr;
  va = new G4VisAttributes(G4Color(1.,1.,1.));
  va-> SetVisibility(true);
  world_lv-> SetVisAttributes(va);

  va= new G4VisAttributes(G4Color(0.,0.8,0.8));
  va-> SetVisibility(true);
  phantom_lv-> SetVisAttributes(va);

  return world_pv;
}
