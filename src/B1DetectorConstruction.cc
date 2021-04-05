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

  // Standard Materials
  G4NistManager *materials = G4NistManager::Instance();

  // Envelope parameters
  //
  G4double env_sizeXY = 3 * m + 1 * mm; // the last 1 mm to avoid overlap issue
  G4double env_sizeZ = 2 * m;
  G4Material *env_mat = nist->FindOrBuildMaterial("G4_AIR");

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = env_sizeXY + 1 * m;
  G4double world_sizeZ = env_sizeZ * 2 + 1 * m;
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
  // Envelope
  //
  auto *solidEnv = new G4Box("Envelope", // its name
                             0.5 * env_sizeXY, 0.5 * env_sizeXY,
                             0.5 * env_sizeZ); // its size

  auto *logicEnv = new G4LogicalVolume(solidEnv,    // its solid
                                       env_mat,     // its material
                                       "Envelope"); // its name

  new G4PVPlacement(nullptr,                            // no rotation
                    G4ThreeVector(0, 0, env_sizeZ / 2), // at (0,0,0)
                    logicEnv,                           // its logical volume
                    "Envelope",                         // its name
                    logicWorld,                         // its mother  volume
                    false,                              // no boolean operation
                    0,                                  // copy number
                    checkOverlaps);                     // overlaps checking

  //
  //  Detector
  //
  G4double detector_XY = 3 * m;
  G4double thickness = 1.0 * mm;
  G4Material *germanium = materials->FindOrBuildMaterial("G4_Ge");

  auto *detector = new G4Box("Detector",
                             0.5 * detector_XY,
                             0.5 * detector_XY,
                             thickness);

  auto *logicalDetector = new G4LogicalVolume(detector,
                                              germanium,
                                              "Detector");
  // Setting detector color
  G4Color purple(0.625, 0.125, 0.9375);
  auto *detectorVisAttr = new G4VisAttributes(purple);
  logicalDetector->SetVisAttributes(detectorVisAttr);

  new G4PVPlacement(nullptr,
                    G4ThreeVector(0, 0, 0.5 * m),
                    logicalDetector,
                    "Detector",
                    logicWorld,
                    false,
                    0,
                    checkOverlaps);

  //
  // Set detector as scoring volume
  //
  fScoringVolume = logicalDetector;

  //
  // always return the physical World
  //
  return physWorld;
}
