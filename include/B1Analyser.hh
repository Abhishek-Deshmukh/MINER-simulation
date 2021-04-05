//
// Created by deshmukh on 05/04/21.
//

#ifndef B1_CRY_TEST_B1ANALYSER_H
#define B1_CRY_TEST_B1ANALYSER_H

#include "globals.hh"
#include "G4SystemOfUnits.hh"

class B1Analyser {
public:
  B1Analyser();
  void save();
  void recordEvent(const G4String& particleName, G4double energy);
  static B1Analyser* getInstance();
  ~B1Analyser();
private:
  static B1Analyser* instance;
  // shape template
  // G4int particleChargeCount; total number of particle of Charge
  // G4double particleChargeEnergy; total energy deposited by the particles
  // pinos
  G4int pionZeroCount;
  G4double pionZeroEnergy;
  G4int pionPlusCount;
  G4double pionPlusEnergy;
  G4int pionMinusCount;
  G4double pionMinusEnergy;
  // neutrons
  G4int neutronCount;
  G4double neutronEnergy;
  // electrons
  G4int electronMinusCount;
  G4double electronMinusEnergy;
  G4int electronPlusCount;
  G4double electronPlusEnergy;
  //muons
  G4int muonMinusCount;
  G4double muonMinusEnergy;
  G4int muonPlusCount;
  G4double muonPlusEnergy;
  // protons
  G4int protonPlusCount;
  G4double protonPlusEnergy;
  G4int protonMinusCount;
  G4double protonMinusEnergy;
  // gamma
  G4int gammaCount;
  G4double gammaEnergy;
};

#endif // B1_CRY_TEST_B1ANALYSER_H
