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
#include "G4PVReplica.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "hcalgeom.h"
#include "common/calscorer.h"

// --------------------------------------------------------------------------
G4VPhysicalVolume* HcalGeom::Construct()
{
  G4NistManager* nist_manager = G4NistManager::Instance();

  // world volume
  const double kDXY_World = 100.*cm;
  const double kDZ_World = 200.*cm;
  auto world_box = new G4Box("world", kDXY_World/2.,
                                      kDXY_World/2., kDZ_World/2.);

  auto air = nist_manager-> FindOrBuildMaterial("G4_AIR");
  auto world_lv = new G4LogicalVolume(world_box, air, "world");

  auto world_pv = new G4PVPlacement(nullptr, G4ThreeVector(), "world",
                                              world_lv, nullptr, false, 0);

  // s/w calorimeter
  const double kDXY_Cal = 30.*cm;
  const double kDZ_Cal = 100.*cm;

  auto cal_box = new G4Box("cal", kDXY_Cal/2., kDXY_Cal/2., kDZ_Cal/2.);
  auto pb = nist_manager-> FindOrBuildMaterial("G4_Pb");

  auto cal_lv = new G4LogicalVolume(cal_box, pb, "cal");

  auto cal_pv = new G4PVPlacement(nullptr, G4ThreeVector(),
                                  cal_lv, "cal", world_lv, false, 0);

  // replication along z-axis
  const double kDZ_Pb = 2.*cm; // 1(Pb)+1(scinti)
  auto pb_solid = new G4Box("pb", kDXY_Cal/2., kDXY_Cal/2., kDZ_Pb/2.);

  auto pb_lv = new G4LogicalVolume(pb_solid, pb, "pb");

  const int kNZ = 50;
  auto pb_rep = new G4PVReplica("pb", pb_lv, cal_lv, kZAxis, kNZ, kDZ_Pb);

  // scintillator
  const double kDZ_SC = 1.*cm;
  auto sc_solid = new G4Box("sc", kDXY_Cal/2., kDXY_Cal/2., kDZ_SC/2.);
  auto sc = nist_manager-> FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  auto sc_lv = new G4LogicalVolume(sc_solid, sc, "sc");

  auto sc_pv = new G4PVPlacement(nullptr, G4ThreeVector(0.,0.,kDZ_SC/2.),
                                 sc_lv, "sc", pb_lv, false, 0);

  // vis attributes
  G4VisAttributes* va = nullptr;
  va = new G4VisAttributes(G4Color(1., 1., 1.));
  va-> SetVisibility(true);
  world_lv-> SetVisAttributes(va);

  va = new G4VisAttributes(G4Color(1., 1., 1.));
  va-> SetVisibility(false);
  cal_lv-> SetVisAttributes(va);

  va= new G4VisAttributes(G4Color(0.5, 0.5, 0.));
  //va-> SetForceSolid(true);
  pb_lv-> SetVisAttributes(va);

  va= new G4VisAttributes(G4Color(0., 0.5, 0.5));
  va-> SetForceSolid(true);
  sc_lv-> SetVisAttributes(va);

  return world_pv;
}

// --------------------------------------------------------------------------
void HcalGeom::ConstructSDandField()
{
  auto cal_scorer = new CalScorer();
  cal_scorer-> SetSimData(simdata_);
  SetSensitiveDetector("sc", cal_scorer);
}
