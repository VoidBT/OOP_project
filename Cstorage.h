#pragma once // Include guard
#include <vector>
#include <string>
#include "Cargo.h" // Make sure Cargo.h is included

using namespace std;

class CStorage {
private:
    vector<Cargo> cargos;

public:
    void addCargo(const Cargo& cargo);
    void editCargo(const string& id, int newTime, const string& newDest);
    void deleteCargo(const string& id);
    const vector<Cargo>& getAllCargos() const;
    void printAll() const;
    Cargo* findCargo(const string& id); // Added non-const findCargo for editing
};