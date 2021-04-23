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
  inFile.open("EnergyIn.csv", std::ios_base::out);
  inFile << "Particle,Charge,energy,time" << std::endl;
}

void Analyser::recordEvent(const G4String &particleName, G4double energy, G4double charge, G4double time) {
  inFile << particleName << "," << charge << "," << energy << "," << time << std::endl;

  if (particleName == "gamma") {
    gammaCount += 1;
    gammaEnergy += energy;
  } else if (particleName == "muon") {
    if (charge == 1) {
      muonPlusCount += 1;
      muonPlusEnergy += energy;
    } else if (charge == -1) {
      muonMinusCount += 1;
      muonMinusEnergy += energy;
    } else {
      G4cout << "Analyser:64: Particle not known" << G4endl;
    }
  } else if (particleName == "pion") {
    if (charge == 1) {
      pionPlusCount += 1;
      pionPlusEnergy += energy;
    } else if (charge == -1) {
      pionMinusCount += 1;
      pionMinusEnergy += energy;
    } else if (charge == 0) {
      pionZeroCount += 1;
      pionZeroEnergy += energy;
    } else {
      G4cout << "Analyser:77: Particle not known" << G4endl;
    }
  } else if (particleName == "neutron") {
    if (charge == 0) {
      neutronCount += 1;
      neutronEnergy += energy;
    } else {
      G4cout << "Analyser:84: Particle not known" << G4endl;
    }
  } else if (particleName == "electron") {
    if (charge == 1) {
      electronPlusCount += 1;
      electronPlusEnergy += energy;
    } else if (charge == -1)  {
      electronMinusCount += 1;
      electronMinusEnergy += energy;
    } else {
      G4cout << "Analyser:94: Particle not known" << G4endl;
    }
  } else if (particleName == "proton") {
    if (charge == 1) {
      protonPlusCount += 1;
      protonPlusEnergy += energy;
    } else if (charge == -1) {
      protonMinusCount += 1;
      protonMinusEnergy += energy;
    } else {
      G4cout << "Analyser:104: Particle not known" << G4endl;
    }
  } else {
    G4cout << "Analyser:107: Particle not known" << G4endl;
  }
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
