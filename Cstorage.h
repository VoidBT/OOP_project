#pragma once
#include <vector>
#include <string>
#include "Cargo.h"

using namespace std;

class CStorage {
private:
    vector<Cargo> cargos;

public:
    void addCargo(const Cargo& c);
    void editCargo(const string& id, int newTime, const string& newDest);
    void deleteCargo(const string& id);
    void clearCargos(); // Added clear method

    const Cargo* getByTimeAndDest(int time, const string& dest) const;
    const Cargo* getCargoByTimeAndDest(int time, const string& dest) const { return getByTimeAndDest(time, dest); }
    void printAll() const;

    // Added this method to allow FileManager to get all cargos for saving
    const vector<Cargo>& getAllCargos() const;

    // Removed loadFromFile and saveToFile - now handled by FileManager
};