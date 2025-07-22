#pragma once
#include <string>
#include <vector>
#include "Cargo.h" // Only include Cargo.h

class CargoGroup {
private:
    std::string groupId;
    std::string destination;
    std::vector<Cargo> cargos;
    int maxSize;
    int timeWindow;

public:
    CargoGroup(const std::string& id, const std::string& dest, int maxSize, int timeWindow);

    bool addCargo(const Cargo& cargo);
    bool removeCargo(const std::string& cargoId);
    bool containsCargo(const std::string& cargoId) const;
    bool isFull() const;
    bool isEmpty() const;
    bool canMergeWith(const CargoGroup& other) const;

    // Getters
    std::string getGroupId() const;
    std::string getDestination() const;
    int getSize() const; // Number of cargos in the group
    int getMaxSize() const; // Max number of cargos
    int getTimeWindow() const;
    const std::vector<Cargo>& getCargos() const;

    // Setters
    void setDestination(const std::string& newDest);
    void setTimeWindow(int newWindow);
};