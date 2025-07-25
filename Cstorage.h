#pragma once // Include guard
#include <vector>
#include <string>
#include "Cargo.h"
#include "CargoGroup.h"

//Done by Brendan Tjung Yew Wang 2401765

using namespace std;

class CStorage {
private:
	// Store cargos and groups
    vector<Cargo> cargos;
    vector<CargoGroup> groups;

public:
    void addCargo(const Cargo& cargo);
    void editCargo(const string& id, int newTime, const string& newDest);
    void deleteCargo(const string& id);
    const vector<Cargo>& getAllCargos() const;
    void printAll() const;
	void printAllGroups() const; // NEW: Print all cargo groups
    Cargo* findCargo(const string& id); // Added non-const findCargo for editing
	void CreateGroups(); // NEW: Add cargo group operations
	vector<CargoGroup> getCargoGroups() const; // NEW: Get all cargo groups
    void clear();
};