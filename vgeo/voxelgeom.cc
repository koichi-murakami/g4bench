/*============================================================================
Copyright 2017-2021 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVParameterised.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "voxelgeom.h"
#include "phantom_pvp.h"
#include "common/calscorer.h"

// --------------------------------------------------------------------------
G4VPhysicalVolume* VoxelGeom::Construct()
{
  auto nist_manager = G4NistManager::Instance();

  // world volume
  const double kDXY_World = 50.*cm;
  const double kDZ_World = 150.*cm;
  auto world_box = new G4Box("world", kDXY_World/2., kDXY_World/2.,
                                      kDZ_World/2.);
  auto air = nist_manager-> FindOrBuildMaterial("G4_AIR");
  auto world_lv = new G4LogicalVolume(world_box, air, "world");
  auto world_pv = new G4PVPlacement(nullptr, G4ThreeVector(), "world",
                                             world_lv, nullptr, false, 0);

  // phantom
  const double kDXY_Phantom = 30.5*cm;
  const double kDZ_Phantom = 30.*cm;
  const double kZ_Phantom = 50.*cm;

  auto phantom_box = new G4Box("phantom", kDXY_Phantom/2., kDXY_Phantom/2.,
                                          kDZ_Phantom/2.);
  auto water = nist_manager-> FindOrBuildMaterial("G4_WATER");
  auto phantom_lv = new G4LogicalVolume(phantom_box, water, "phantom");
  auto phantom = new G4PVPlacement(0, G4ThreeVector(0., 0., kZ_Phantom),
                                   phantom_lv, "phantom", world_lv, false, 0);

  // voxel params.
  const int kNxy_Voxel = 61;
  const int kNz_Voxel = 150;
  const double kDXY_Voxel = 5. * mm;
  const double kDZ_Voxel = 2. * mm;

  // 1st, replication along z-axis
  auto voxel_dz_solid = new G4Box("vz", kDXY_Phantom/2.,
                                        kDXY_Phantom/2., kDZ_Voxel/2.);
  auto voxel_dz_lv = new G4LogicalVolume(voxel_dz_solid, water, "vz");
  auto voxel_dz = new G4PVReplica("vz", voxel_dz_lv, phantom_lv,
                                  kZAxis, kNz_Voxel, kDZ_Voxel);

  // 2nd, replication along y-axis
  auto voxel_dyz_solid = new G4Box("vyz", kDXY_Phantom/2.,
                                          kDXY_Voxel/2., kDZ_Voxel/2.);
  auto voxel_dyz_lv = new G4LogicalVolume(voxel_dyz_solid, water, "vyz");
  auto phantom_dyz = new G4PVReplica("vyz", voxel_dyz_lv, voxel_dz_lv,
                                     kYAxis, kNxy_Voxel, kDXY_Voxel);

  // 3rd, nested parameterization
  auto voxel_dxyz_solid = new G4Box("vxyz", kDXY_Voxel/2.,
                                            kDXY_Voxel/2., kDZ_Voxel/2.);
  auto voxel_dxyz_lv = new G4LogicalVolume(voxel_dxyz_solid, water, "vxyz");

  G4VNestedParameterisation* param { nullptr };
  auto wp_param = new PhantomPVP();
  wp_param-> SetSegment(kNxy_Voxel, kDXY_Voxel);
  new G4PVParameterised("vxyz", voxel_dxyz_lv,
                        voxel_dyz_lv, kXAxis, kNxy_Voxel, wp_param);

  // vis attributes
  G4VisAttributes* va { nullptr };
  va = new G4VisAttributes(G4Color(1.,1.,1.));
  va-> SetVisibility(true);
  world_lv-> SetVisAttributes(va);

  va = new G4VisAttributes(G4Color(0.,0.8,0.8));
  va-> SetVisibility(true);
  voxel_dz_lv-> SetVisAttributes(va);

  va = new G4VisAttributes();
  va-> SetVisibility(false);
  phantom_lv-> SetVisAttributes(va);
  voxel_dyz_lv-> SetVisAttributes(va);
  voxel_dxyz_lv-> SetVisAttributes(va);

  return world_pv;
}

// --------------------------------------------------------------------------
void VoxelGeom::ConstructSDandField()
{
  auto cal_scorer = new CalScorer();
  cal_scorer-> SetSimData(simdata_);
  SetSensitiveDetector("vxyz", cal_scorer);
}
