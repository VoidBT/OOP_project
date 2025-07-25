#pragma once
#include <string>
#include <vector>
#include "Time.h"
#include "Cargo.h" // Only include Cargo.h

class CargoGroup {
private:
    int groupId;
    std::string destination;
    std::vector<Cargo> cargos;
    int maxSize = 10;
    Time timeWindow;
    int GrpSize;

public:
    CargoGroup(int id, const std::string& dest, int timeWindow);

    bool addCargo(const Cargo& cargo);
    bool removeCargo(const std::string& cargoId);
    bool containsCargo(const std::string& cargoId) const;
    bool isFull() const;
    bool isEmpty() const;
    bool canMergeWith(const CargoGroup& other) const;

    // Getters
    int getGroupId() const;
    std::string getDestination() const;
    size_t getSize() const; // Number of cargos in the group
    int getMaxSize() const; // Max number of cargos
    Time getTimeWindow() const;
    const std::vector<Cargo>& getCargos() const;
    int getGrpSize() const;

    // Setters
    void setDestination(const std::string& newDest);
    void setTimeWindow(int newWindow);

    void clear();
};