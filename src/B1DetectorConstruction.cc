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
  G4double worldSizeXY = 5 * m + 1 * mm; // the last 1 mm to avoid overlap issue
  G4double worldSizeZ = 5 * m;
  G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");

  auto *solidWorld = new G4Box("World", // its name
                               0.5 * worldSizeXY, 0.5 * worldSizeXY,
                               0.5 * worldSizeZ); // its size

  auto *logicWorld = new G4LogicalVolume(solidWorld, // its solid
                                         worldMat,   // its material
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
  auto brickMat = nist->FindOrBuildMaterial("G4_Pb");

  // Laying the lower 2 layers of the bricks

  for (G4int x = -14; x <= 14; x = x + 4) {
    for (G4int y = -12; y <= 12; y = y + 8) {
      for (G4int z = 1; z <= 3; z = z + 2) {

        G4int xIndex = (x + 14) / 4;
        G4int yIndex = (y + 12) / 8;
        G4int zIndex = (z - 1) / 2;

        auto xGap = xIndex * 0.0001 * mm;
        auto yGap = yIndex * 0.0001 * mm;
        auto zGap = zIndex * 0.0001 * mm;

        auto name = "brick_" + std::to_string(xIndex) + "_" +
                    std::to_string(yIndex) + "_" + std::to_string(zIndex);

        auto *brick_log = new G4LogicalVolume(brickY, brickMat, name);
        new G4PVPlacement(
            nullptr,
            G4ThreeVector(x * inch + xGap, y * inch + yGap, z * inch + zGap),
            brick_log, name, logicWorld, false, 0, checkOverlaps);
      }
    }
  }

  // filling up the odd index layers

  // defining variables to be used
  G4int xIndex, yIndex, zIndex;
  G4double xPos, yPos, xGap, yGap, zGap;
  G4Box *brick;
  G4String name, brickName;
  G4LogicalVolume *brickLog;

  for (G4int x = -14; x <= 14; x += 28) {
    for (G4int y = -12; y <= 12; y += 8) {
      for (G4int z = 3; z <= 34; z += 2) {
        xIndex = (x + 14) / 28;
        yIndex = (y + 12) / 8;
        zIndex = (z - 1) / 2;

        // gap between to avoid intersection
        xGap = xIndex * 0.0001 * mm;
        yGap = yIndex * 0.0001 * mm;
        zGap = zIndex * 0.0001 * mm;

        // selecting wall orientation
        if (zIndex % 2 == 1) {
          xPos = x * inch + xGap;
          yPos = y * inch + yGap;
          brick = brickY;
          brickName = "brickY";
        } else {
          yPos = x * inch + xGap;
          xPos = y * inch + yGap;
          brick = brickX;
          brickName = "brickX";
        }

        name = brickName + "_" + std::to_string(xIndex) + "_" +
               std::to_string(yIndex) + "_" + std::to_string(zIndex);

        // making and placing
        brickLog = new G4LogicalVolume(brick, brickMat, name);
        new G4PVPlacement(nullptr, G4ThreeVector(xPos, yPos, z * inch + zGap),
                          brickLog, name, logicWorld, false, 0, checkOverlaps);
      }
    }
  }

  for (G4int x = -8; x <= 8; x += 8) {
    for (G4int y = -14; y <= 14; y += 28) {
      for (G4int z = 3; z <= 34; z += 2) {
        xIndex = (x + 8) / 8;
        yIndex = (y + 14) / 28;
        zIndex = (z - 1) / 2;

        // gap between to avoid intersection
        xGap = xIndex * 0.0001 * mm;
        yGap = yIndex * 0.0001 * mm;
        zGap = zIndex * 0.0001 * mm;

        // selecting wall orientation
        if (zIndex % 2 == 1) {
          xPos = x * inch + xGap;
          yPos = y * inch + yGap;
          brick = brickX;
          brickName = "brickX";
        } else {
          yPos = x * inch + xGap;
          xPos = y * inch + yGap;
          brick = brickY;
          brickName = "brickX";
        }

        name = brickName + "_" + std::to_string(xIndex) + "_" +
               std::to_string(yIndex) + "_" + std::to_string(zIndex);

        // making and placing
        brickLog = new G4LogicalVolume(brick, brickMat, name);
        new G4PVPlacement(nullptr, G4ThreeVector(xPos, yPos, z * inch + zGap),
                          brickLog, name, logicWorld, false, 0, checkOverlaps);
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
