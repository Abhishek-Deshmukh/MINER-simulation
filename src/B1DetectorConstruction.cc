//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Color.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Orb.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

B1DetectorConstruction::B1DetectorConstruction()
    : G4VUserDetectorConstruction(), fScoringVolume(nullptr) {}

B1DetectorConstruction::~B1DetectorConstruction() = default;

G4VPhysicalVolume *B1DetectorConstruction::Construct() {
  // Get nist material manager
  G4NistManager *nist = G4NistManager::Instance();

  // Extra units
  G4double inch = 2.54 * cm;

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY =
      5 * m + 1 * mm; // the last 1 mm to avoid overlap issue
  G4double world_sizeZ = 5 * m;
  G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto *solidWorld = new G4Box("World", // its name
                               0.5 * world_sizeXY, 0.5 * world_sizeXY,
                               0.5 * world_sizeZ); // its size

  auto *logicWorld = new G4LogicalVolume(solidWorld, // its solid
                                         world_mat,  // its material
                                         "World");   // its name

  G4VPhysicalVolume *physWorld =
      new G4PVPlacement(nullptr,                // no rotation
                        G4ThreeVector(0, 0, 0), // at (0,0,0)
                        logicWorld,             // its logical volume
                        "World",                // its name
                        nullptr,                // its mother  volume
                        false,                  // no boolean operation
                        0,                      // copy number
                        checkOverlaps);         // overlaps checking

  //
  // Defining Bricks (8 x 4 x 2 inch) (lead)
  //
  // bricks are in 2 orientations both with 2 inch side on along z-axis

  auto *brickX = new G4Box("BrickX", // the one that is long along X axis
                           4 * inch, 2 * inch, 1 * inch);
  auto *brickY = new G4Box("BrickY", // the one that is long along Y axis
                           2 * inch, 4 * inch, 1 * inch);
  auto brick_mat = nist->FindOrBuildMaterial("G4_Pb");

  // Laying the lower 2 layers of the bricks

  for (G4double x = -14; x <= 14; x = x + 4) {
    for (G4double y = -12; y <= 12; y = y + 8) {
      for (G4double z = 1; z <= 3; z = z + 2) {

        G4int x_index = (x + 14) / 4;
        G4int y_index = (y + 12) / 8;
        G4int z_index = (z - 1) / 2;

        auto gap_x = x_index * 0.0001 * mm;
        auto gap_y = y_index * 0.0001 * mm;
        auto gap_z = z_index * 0.0001 * mm;

        auto name = "brick_" + std::to_string(x_index) + "_" +
                    std::to_string(y_index) + "_" + std::to_string(z_index);

        //        G4cout << G4endl << name << ": " << gap_x << ", " << gap_y <<
        //        ", " << gap_z << G4endl;

        auto *brick_log = new G4LogicalVolume(brickY, brick_mat, name);
        new G4PVPlacement(
            nullptr,
            G4ThreeVector(x * inch + gap_x, y * inch + gap_y, z * inch + gap_z),
            brick_log, name, logicWorld, false, 0, checkOverlaps);
      }
    }
  }

  // filling up the odd index layers

  for (G4double x = -14; x <= 14; x += 28) {
    for (G4double y = -12; y <= 12; y += 8) {
      for (G4double z = 3; z <= 34; z += 2) {
        G4int x_index = (x + 14) / 28;
        G4int y_index = (y + 12) / 8;
        G4int z_index = (z - 1) / 2;

        auto gap_x = x_index * 0.0001 * mm;
        auto gap_y = y_index * 0.0001 * mm;
        auto gap_z = z_index * 0.0001 * mm;

        G4double x_pos;
        G4double y_pos;
        G4Box *brick;
        std::basic_string<char, std::char_traits<char>, std::allocator<char>>
            name;
        if (z_index % 2 == 1) {
          x_pos = x * inch + gap_x;
          y_pos = y * inch + gap_y;
          brick = brickY;
          name = "brickY_" + std::to_string(x_index) + "_" +
                 std::to_string(y_index) + "_" + std::to_string(z_index);
        } else {
          y_pos = x * inch + gap_x;
          x_pos = y * inch + gap_y;
          brick = brickX;
          name = "brickX_" + std::to_string(x_index) + "_" +
                 std::to_string(y_index) + "_" + std::to_string(z_index);
        }

        auto *brick_log = new G4LogicalVolume(brick, brick_mat, name);
        new G4PVPlacement(nullptr,
                          G4ThreeVector(x_pos, y_pos, z * inch + gap_z),
                          brick_log, name, logicWorld, false, 0, checkOverlaps);
      }
    }
  }

  for (G4double x = -8; x <= 8; x += 8) {
    for (G4double y = -14; y <= 14; y += 28) {
      for (G4double z = 3; z <= 34; z += 2) {
        G4int x_index = (x + 8) / 8;
        G4int y_index = (y + 14) / 28;
        G4int z_index = (z - 1) / 2;

        auto gap_x = x_index * 0.0001 * mm;
        auto gap_y = y_index * 0.0001 * mm;
        auto gap_z = z_index * 0.0001 * mm;

        G4double x_pos;
        G4double y_pos;
        G4Box *brick;
        std::basic_string<char, std::char_traits<char>, std::allocator<char>>
            name;
        if (z_index % 2 == 1) {
          x_pos = x * inch + gap_x;
          y_pos = y * inch + gap_y;
          brick = brickX;
          name = "brickX_" + std::to_string(x_index) + "_" +
                 std::to_string(y_index) + "_" + std::to_string(z_index);
        } else {
          y_pos = x * inch + gap_x;
          x_pos = y * inch + gap_y;
          brick = brickY;
          name = "brickY_" + std::to_string(x_index) + "_" +
                 std::to_string(y_index) + "_" + std::to_string(z_index);
        }

        auto *brick_log = new G4LogicalVolume(brick, brick_mat, name);
        new G4PVPlacement(nullptr,
                          G4ThreeVector(x_pos, y_pos, z * inch + gap_z),
                          brick_log, name, logicWorld, false, 0, checkOverlaps);
      }
    }
  }

  //
  // Set detector as scoring volume
  //
  //  fScoringVolume = brick_log;

  //
  // always return the physical World
  //
  return physWorld;
}
