#include "CStorage.h"
#include <fstream> // Not strictly needed anymore if file I/O is moved
#include <algorithm>
#include <iostream>
#include <sstream> // Not strictly needed anymore if parsing is moved

using namespace std;

void CStorage::addCargo(const Cargo& c) {
    cargos.push_back(c);
}

void CStorage::editCargo(const string& id, int newTime, const string& newDest) {
    for (auto& c : cargos) {
        if (c.getID() == id) {
            c.setTime(newTime);
            c.setDest(newDest);
            break;
        }
    }
}

void CStorage::deleteCargo(const string& id) {
    cargos.erase(
        remove_if(cargos.begin(), cargos.end(),
            [&id](const Cargo& c) { return c.getID() == id; }),
        cargos.end()
    );
}

void CStorage::clearCargos() { // Implementation for clearCargos
    cargos.clear();
}

const Cargo* CStorage::getByTimeAndDest(int time, const string& dest) const {
    for (const auto& c : cargos) {
        if (c.getTime() == time && c.getDest() == dest) {
            return &c;
        }
    }
    return nullptr;
}

void CStorage::printAll() const {
    for (const auto& c : cargos) {
        cout << c.getID() << " | "
            << c.getTime() << " | "
            << c.getDest() << "\n";
    }
}

const vector<Cargo>& CStorage::getAllCargos() const {
    return cargos;
}

// Removed loadFromFile and saveToFile implementations from here.
// They are now handled by FileManager.