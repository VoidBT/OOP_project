#pragma once // Include guard
#include <vector>
#include <string>
#include "Cargo.h" // Make sure Cargo.h is included

class CStorage {
private:
    std::vector<Cargo> cargos;

public:
    void addCargo(const Cargo& cargo);
    void editCargo(const std::string& id, int newTime, const std::string& newDest);
    void deleteCargo(const std::string& id);
    const std::vector<Cargo>& getAllCargos() const;
    void printAll() const;
    Cargo* findCargo(const std::string& id); // Added non-const findCargo for editing
};