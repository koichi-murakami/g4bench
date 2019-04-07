/*============================================================================
Copyright 2017-2019 Koichi Murakami

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
#include "calscorer.h"

// --------------------------------------------------------------------------
G4VPhysicalVolume* EcalGeom::Construct()
{
  G4NistManager* nist_manager = G4NistManager::Instance();

  // world volume
  const double kDXYZ_World = 100.*cm;
  auto world_box = new G4Box("world", kDXYZ_World/2.,
                                      kDXYZ_World/2., kDXYZ_World/2.);

  auto air = nist_manager-> FindOrBuildMaterial("G4_AIR");
  auto world_lv = new G4LogicalVolume(world_box, air, "world");

  auto world_pv = new G4PVPlacement(nullptr, G4ThreeVector(), "world",
                                    world_lv, nullptr, false, 0);

  // voxel
  const double kDXZ_Voxel = 20.*mm;
  const double kDL_Voxel = 60.*cm;

  auto voxel_box = new G4Box("voxel", kDXZ_Voxel/2., kDL_Voxel/2.,
                                      kDXZ_Voxel/2.);
  auto voxel_lv = new G4LogicalVolume(voxel_box, air, "voxel");

  const int kNzTracker = 5;
  const int kNxTracker = 11;
  const double kZTracker = -20.*cm;
  int index = 0;
  for (auto iz = 0; iz < kNzTracker; iz++) {
    for (auto ix = -kNxTracker; ix <= kNxTracker; ix++) {
      double x0 = kDXZ_Voxel*ix;
      double z0 = kZTracker + kDXZ_Voxel*iz;
      auto pvoxel = new G4PVPlacement(0, G4ThreeVector(x0, 0., z0), voxel_lv,
                                      "voxel", world_lv, false, index);
      index++;
    }
  }

  // tube tracker
  const double kTubeInner = 8.5*mm;
  const double kTubeOuter = 9.5*mm;
  auto tube = new G4Tubs("tube", kTubeInner, kTubeOuter, kDL_Voxel/2.,
                                 0., 360.*deg);

  auto al = nist_manager-> FindOrBuildMaterial("G4_Al");
  auto tube_lv = new G4LogicalVolume(tube, al, "tube");

  auto tube_rm = new G4RotationMatrix;
  tube_rm-> rotateX(-90.*deg);
  auto tube_pv = new G4PVPlacement(tube_rm, G4ThreeVector(), tube_lv, "tube",
                                   voxel_lv, false, 0);
  // calorimeter
  const double kDXY_Cal = 25.*mm;
  const double kDZ_Cal = 30.*cm;

  auto cal_box = new G4Box("cal", kDXY_Cal/2., kDXY_Cal/2., kDZ_Cal/2.);
  auto csi = nist_manager-> FindOrBuildMaterial("G4_CESIUM_IODIDE");

  auto cal_lv = new G4LogicalVolume(cal_box, csi, "cal");

  index = 0;
  const int kNxyCal = 11;
  const double kZ_Cal = 20.*cm;
  for (auto ix = -kNxyCal; ix <= kNxyCal; ix++) {
    for (auto iy = -kNxyCal; iy <= kNxyCal; iy++) {
      double x0 = kDXY_Cal*ix;
      double y0 = kDXY_Cal*iy;
      auto cal_pv = new G4PVPlacement(0, G4ThreeVector(x0, y0, kZ_Cal),
                                      cal_lv, "cal", world_lv, false, index);
      index++;
    }
  }

  // vis attributes
  G4VisAttributes* va = nullptr;
  va = new G4VisAttributes(G4Color(1.,1.,1.));
  va-> SetVisibility(true);
  world_lv-> SetVisAttributes(va);

  va= new G4VisAttributes(G4Color(0.,0.8,0.8));
  va-> SetVisibility(false);
  voxel_lv-> SetVisAttributes(va);

  va= new G4VisAttributes(G4Color(0.,0.8,0.8));
  va-> SetVisibility(true);
  tube_lv-> SetVisAttributes(va);

  va= new G4VisAttributes(G4Color(0.5,0.5,0.));
  cal_lv-> SetVisAttributes(va);

  return world_pv;
}

// --------------------------------------------------------------------------
void EcalGeom::ConstructSDandField()
{
  auto cal_scorer = new CalScorer();
  cal_scorer-> SetSimData(simdata_);
  SetSensitiveDetector("cal", cal_scorer);
}
