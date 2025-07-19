#include "FreightTypes.h"
#include <stdexcept>

FreightExtended::FreightExtended(const string& id, int t, const string& d, FreightType type)
    : Freight(id, t, d), type(type) {
    switch (type) {
    case FreightType::MINI_MOVER: maxCapacity = 2; break;
    case FreightType::CARGO_CRUISER: maxCapacity = 6; break;
    case FreightType::MEGA_CARRIER: maxCapacity = 12; break;
    }
}

FreightType FreightExtended::getType() const { return type; }
int FreightExtended::getMaxCapacity() const { return maxCapacity; }
int FreightExtended::getCurrentLoad() const { return assignedCargos.size(); }
bool FreightExtended::isFull() const { return assignedCargos.size() >= maxCapacity; }
bool FreightExtended::canAcceptMore() const { return !isFull(); }

bool FreightExtended::assignCargo(const string& cargoId) {
    if (isFull()) return false;
    assignedCargos.push_back(cargoId);
    return true;
}

bool FreightExtended::removeCargo(const string& cargoId) {
    auto it = remove(assignedCargos.begin(), assignedCargos.end(), cargoId);
    if (it != assignedCargos.end()) {
        assignedCargos.erase(it, assignedCargos.end());
        return true;
    }
    return false;
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