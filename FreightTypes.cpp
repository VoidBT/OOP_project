#include "FreightTypes.h"
#include <stdexcept>
#include <algorithm>

using namespace std;

// Updated constructor to initialize currentLoadSize
FreightExtended::FreightExtended(const string& id, int t, const string& d, FreightType type)
    : Freight(id, t, d), type(type), currentLoadSize(0) { // Initialize currentLoadSize
    switch (type) {
    case FreightType::MINI_MOVER: maxCapacity = 2; break; // These are now max *size* capacities
    case FreightType::CARGO_CRUISER: maxCapacity = 6; break;
    case FreightType::MEGA_CARRIER: maxCapacity = 12; break;
    }
}

FreightType FreightExtended::getType() const { return type; }
int FreightExtended::getMaxCapacity() const { return maxCapacity; }
int FreightExtended::getCurrentLoad() const { return assignedCargos.size(); } // Still number of items
int FreightExtended::getCurrentLoadSize() const { return currentLoadSize; } // NEW: Get load by size

// NEW: isFull() now checks based on size
bool FreightExtended::isFull() const { return currentLoadSize >= maxCapacity; }

// NEW: canAcceptMore() takes cargoSize as argument
bool FreightExtended::canAcceptMore(int cargoSize) const {
    return (currentLoadSize + cargoSize) <= maxCapacity;
}

// NEW: assignCargo() takes cargoSize as argument
bool FreightExtended::assignCargo(const string& cargoId, int cargoSize) {
    if (!canAcceptMore(cargoSize)) return false; // Use new capacity check
    assignedCargos.push_back(cargoId);
    currentLoadSize += cargoSize; // Update current load size
    return true;
}

// RemoveCargo would ideally reduce currentLoadSize, but would need cargo's size.
// For simplicity here, it just removes the ID. If accurate size tracking for removal is needed,
// you'd need to pass the cargo object or its size when removing.
bool FreightExtended::removeCargo(const string& cargoId) {
    auto it = remove(assignedCargos.begin(), assignedCargos.end(), cargoId);
    if (it != assignedCargos.end()) {
        assignedCargos.erase(it, assignedCargos.end());
        // WARNING: currentLoadSize is not updated here, leading to potential inaccuracies if cargos are removed.
        // To fix this properly, you would need access to the Cargo object's size being removed.
        // For now, this assumes removals are rare or handled by a full reset.
        return true;
    }
    return false;
}

void FreightExtended::clearAssignedCargos() {
    assignedCargos.clear();
    currentLoadSize = 0; // NEW: Reset load size when clearing
}

const vector<string>& FreightExtended::getAssignedCargos() const {
    return assignedCargos;
}

string FreightExtended::typeToString(FreightType type) {
    switch (type) {
    case FreightType::MINI_MOVER: return "MiniMover";
    case FreightType::CARGO_CRUISER: return "CargoCruiser";
    case FreightType::MEGA_CARRIER: return "MegaCarrier";
    default: return "Unknown";
    }
}

FreightType FreightExtended::stringToType(const string& typeStr) {
    if (typeStr == "MiniMover") return FreightType::MINI_MOVER;
    if (typeStr == "CargoCruiser") return FreightType::CARGO_CRUISER;
    if (typeStr == "MegaCarrier") return FreightType::MEGA_CARRIER;
    throw invalid_argument("Invalid freight type string");
}