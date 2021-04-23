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
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "Analyser.hh"
#include "CRYGenerator.h"
#include "CRYParticle.h"
#include "CRYSetup.h"
#include "CRYUtils.h"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(), fParticleGun(nullptr) {
  fParticleGun = new G4ParticleGun();

  // Parsing the input file
  std::ifstream inputFile;
  inputFile.open("CRY_file", std::ios::in);
  char buffer[1000];
  G4String setupString;

  if (inputFile.fail()) {
    successfulCRYLoad = false;
    G4cout << "PrimaryGenerator Action: Failed to open CRY input file="
           << "CRY_file" << G4endl;
  } else {
    successfulCRYLoad = true;
    while (!inputFile.getline(buffer, 100).eof()) {
      setupString.append(buffer);
      setupString.append(" ");
    }
  }

  // CRY Setup
  auto *setup =
      new CRYSetup(setupString, "/home/mercury/Downloads/CRY/cry_v1.7/data");
  particleGenerator = new CRYGenerator(setup);
  particlesVector = new std::vector<CRYParticle *>;
  particleTable = G4ParticleTable::GetParticleTable();

  // Setting up the analyser
  analyser = Analyser::getInstance();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
  delete fParticleGun;
  analyser->save();
  delete analyser;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  // this function is called at the beginning of each event
  //

  G4cout << "Event id: " << anEvent->GetEventID() << G4endl;
  if (!successfulCRYLoad) {
    auto *str = new G4String("CRY file was not loaded successfully");
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries", "1",
                RunMustBeAborted, *str);
  }

  G4String particleName;
  G4double sourceZPosition = 35 * inch;

  particlesVector->clear();
  particleGenerator->genEvent(particlesVector);

  for (auto particle : *particlesVector) {
    G4ParticleDefinition *particleDefinition =
        particleTable->FindParticle(particle->PDGid());
    G4double particleEnergy = particle->ke() * MeV;
    CLHEP::Hep3Vector particlePosition =
        G4ThreeVector(particle->x() * m, particle->y() * m,
                      particle->z() * m + sourceZPosition);
    CLHEP::Hep3Vector particleMomentumDirection =
        G4ThreeVector(particle->u(), particle->v(), particle->w());

    fParticleGun->SetParticleDefinition(particleDefinition);
    fParticleGun->SetParticleEnergy(particleEnergy);
    fParticleGun->SetParticlePosition(particlePosition);
    fParticleGun->SetParticleMomentumDirection(particleMomentumDirection);
    fParticleGun->SetParticleTime(particle->t());
    fParticleGun->GeneratePrimaryVertex(anEvent);

    particleName = CRYUtils::partName(particle->id());
    analyser->recordEvent(particleName, particleEnergy, particle->charge(), particle->t());
    // G4cout << "================================================================" << G4endl;
    // G4cout << "particleName: " << particleName << G4endl;
    // G4cout << "================================================================" << G4endl;
    delete (particle);
  }
}
