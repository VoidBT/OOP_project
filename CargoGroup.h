#pragma once // Include guard
#include <vector>
#include <string>
#include "Cargo.h" // Make sure Cargo.h is included

class CargoGroup {
private:
    std::string groupId;
    std::string destination;
    int maxSize;
    std::vector<Cargo> cargos;
    int timeWindow; // in minutes

public:
    CargoGroup(const std::string& id, const std::string& dest, int maxSize = 10, int timeWindow = 15);

    bool addCargo(const Cargo& cargo);
    bool removeCargo(const std::string& cargoId);
    bool containsCargo(const std::string& cargoId) const;
    bool isFull() const;
    bool isEmpty() const;
    bool canMergeWith(const CargoGroup& other) const;

    std::string getGroupId() const;
    std::string getDestination() const;
    int getSize() const;
    int getMaxSize() const;
    int getTimeWindow() const;
    const std::vector<Cargo>& getCargos() const;

    void setDestination(const std::string& newDest);
    void setTimeWindow(int newWindow);
};