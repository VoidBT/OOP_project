#include "CStorage.h"
#include <algorithm> // For std::remove_if
#include <iostream>

using namespace std;

void CStorage::addCargo(const Cargo& cargo) {
    cargos.push_back(cargo);
}

void CStorage::editCargo(const string& id, int newTime, const string& newDest) {
    Cargo* cargo = findCargo(id);
    if (cargo) {
        cargo->setTime(newTime);
        cargo->setDest(newDest);
    }
    else {
        cout << "Cargo with ID " << id << " not found.\n";
    }
}

void CStorage::deleteCargo(const string& id) {
    auto it = remove_if(cargos.begin(), cargos.end(),
        [&id](const Cargo& c) { return c.getID() == id; });
    if (it != cargos.end()) {
        cargos.erase(it, cargos.end());
    }
    else {
        cout << "Cargo with ID " << id << " not found.\n";
    }
}

const vector<Cargo>& CStorage::getAllCargos() const {
    return cargos;
}

void CStorage::printAll() const {
    cout << "\n--- All Cargos ---\n";
    if (cargos.empty()) {
        cout << "No cargos available.\n";
        return;
    }
    for (const auto& cargo : cargos) {
        cout << "ID: " << cargo.getID()
            << ", Time: " << cargo.getTime()
            << ", Destination: " << cargo.getDest() << "\n";
    }
}

Cargo* CStorage::findCargo(const string& id) {
    for (auto& cargo : cargos) {
        if (cargo.getID() == id) {
            return &cargo;
        }
    }
    return nullptr;
}