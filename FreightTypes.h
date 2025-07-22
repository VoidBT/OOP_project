#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "Freight.h"
#include "Cargo.h" // NEW: Include Cargo.h to access Cargo::getSize()

enum class FreightType { MINI_MOVER, CARGO_CRUISER, MEGA_CARRIER };

class FreightExtended : public Freight {
private:
    FreightType type;
    int maxCapacity; // This now refers to max *size/weight/volume* capacity
    int currentLoadSize; // NEW: Track current load by size
    std::vector<std::string> assignedCargos; // Stores cargo IDs

public:
    FreightExtended(const std::string& id, int t, const std::string& d, FreightType type);

    FreightType getType() const;
    int getMaxCapacity() const; // Returns max size capacity
    int getCurrentLoad() const; // This still returns number of items, maybe rename or remove if size is primary
    int getCurrentLoadSize() const; // NEW: Get current load by size
    bool isFull() const; // Based on size
    bool canAcceptMore(int cargoSize) const; // NEW: Check if it can accept a specific cargo size
    bool assignCargo(const std::string& cargoId, int cargoSize); // NEW: Assign with cargo size
    bool removeCargo(const std::string& cargoId); // This might need cargo size too for accurate load tracking, but for simplicity, let's assume it just removes ID for now.
    void clearAssignedCargos();
    const std::vector<std::string>& getAssignedCargos() const;

    static std::string typeToString(FreightType type);
    static FreightType stringToType(const std::string& typeStr);
};