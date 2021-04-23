//
// Created by deshmukh on 05/04/21.
//

#include "Analyser.hh"
#include <fstream>
#include <iostream>

// So only one instance of the analyser exists at a moment
Analyser *Analyser::instance = nullptr;
Analyser *Analyser::getInstance() {
  if (instance == nullptr)
    instance = new Analyser;
  return instance;
}

Analyser::Analyser() {
  pionZeroCount = 0;
  pionZeroEnergy = 0.0;
  pionPlusCount = 0;
  pionPlusEnergy = 0.0;
  pionMinusCount = 0;
  pionMinusEnergy = 0.0;
  // neutrons
  neutronCount = 0;
  neutronEnergy = 0.0;
  // electrons
  electronMinusCount = 0;
  electronMinusEnergy = 0.0;
  electronPlusCount = 0;
  electronPlusEnergy = 0.0;
  // muons
  muonMinusCount = 0;
  muonMinusEnergy = 0.0;
  muonPlusCount = 0;
  muonPlusEnergy = 0.0;
  // protons
  protonPlusCount = 0;
  protonPlusEnergy = 0.0;
  protonMinusCount = 0;
  protonMinusEnergy = 0.0;
  // gamma
  gammaCount = 0;
  gammaEnergy = 0.0;

  // opening the file to store the edeps
  energyOutFile.open("Energy.txt", std::ios_base::out);
}

void Analyser::recordEvent(const G4String &particleName, G4double energy,
                           const CLHEP::Hep3Vector& vector) {
  // TODO: find out which particle and then increment the corresponding particle
}

void Analyser::save() const {
  G4String fileName("CRY_output.csv");
  std::fstream fileout;
  fileout.open(fileName, std::ios_base::out);
  if (!fileout.is_open()) {
    G4cout << "Could not open file gonna write here";
    G4cout << "Particle,Charge,Symbol,Count,Energy" << G4endl;
    G4cout << "Pion,+,π^+," << pionPlusCount << "," << pionPlusEnergy << G4endl;
    G4cout << "Pion,,π^0," << pionZeroCount << "," << pionZeroEnergy << G4endl;
    G4cout << "Pion,-,π^-," << pionMinusCount << "," << pionMinusEnergy << G4endl;
    G4cout << "Neutron,,n," << neutronCount << "," << neutronEnergy << G4endl;
    G4cout << "Electron,+,e^+," << electronPlusCount << "," << electronPlusEnergy
           << G4endl;
    G4cout << "Electron,-,e^-," << electronMinusCount << ","
           << electronMinusEnergy << G4endl;
    G4cout << "Muon,+,μ^+," << muonPlusCount << "," << muonPlusEnergy << G4endl;
    G4cout << "Muon,-,μ^-," << muonMinusCount << "," << muonMinusEnergy << G4endl;
    G4cout << "Proton,+,p^+," << protonPlusCount << "," << protonPlusEnergy
           << G4endl;
    G4cout << "Proton,-,p^-," << protonMinusCount << "," << protonMinusEnergy
           << G4endl;
    G4cout << "Gamma,,γ," << gammaCount << "," << gammaEnergy << G4endl;
  } else {
    fileout << "Particle,Charge,Symbol,Count,Energy" << G4endl;
    fileout << "Pion,+,π^+," << pionPlusCount << "," << pionPlusEnergy << G4endl;
    fileout << "Pion,,π^0," << pionZeroCount << "," << pionZeroEnergy << G4endl;
    fileout << "Pion,-,π^-," << pionMinusCount << "," << pionMinusEnergy << G4endl;
    fileout << "Neutron,,n," << neutronCount << "," << neutronEnergy << G4endl;
    fileout << "Electron,+,e^+," << electronPlusCount << "," << electronPlusEnergy
           << G4endl;
    fileout << "Electron,-,e^-," << electronMinusCount << ","
           << electronMinusEnergy << G4endl;
    fileout << "Muon,+,μ^+," << muonPlusCount << "," << muonPlusEnergy << G4endl;
    fileout << "Muon,-,μ^-," << muonMinusCount << "," << muonMinusEnergy << G4endl;
    fileout << "Proton,+,p^+," << protonPlusCount << "," << protonPlusEnergy
           << G4endl;
    fileout << "Proton,-,p^-," << protonMinusCount << "," << protonMinusEnergy
           << G4endl;
    fileout << "Gamma,,γ," << gammaCount << "," << gammaEnergy << G4endl;
  }
}
void Analyser::appendEdep(G4double edep) {

  if (!energyOutFile.is_open()) {
    G4cout << "Energy output file is not open" << G4endl;
  } else {
    energyOutFile << edep << ",";
  }
}
Analyser::~Analyser() = default;