#include "FreightTypes.h"
#include <algorithm>
#include <stdexcept>

//Done by Ryan Ang Rui Heng 2400522

using namespace std;

FreightExtended::FreightExtended(const string& id, int t, const string& d, FreightType type)
    : Freight(id, t, d, type), currentLoadSize(0) {
    switch (type) {
    case FreightType::MINI_MOVER: maxCapacity = 2; break;
    case FreightType::CARGO_CRUISER: maxCapacity = 6; break;
    case FreightType::MEGA_CARRIER: maxCapacity = 12; break;
    default: maxCapacity = 0;
		throw invalid_argument("Invalid freight type for FreightExtended");
        break;
    }
}

int FreightExtended::getMaxCapacity() const { return maxCapacity; }
int FreightExtended::getCurrentLoadSize() const { return currentLoadSize; }

bool FreightExtended::isFull() const {
    return currentLoadSize >= maxCapacity;
}

int FreightExtended::canAcceptMore(int cargoSize) const {
    if (currentLoadSize + cargoSize < maxCapacity) {
		return cargoSize;
    }
	return maxCapacity - currentLoadSize;
}

bool FreightExtended::canAcceptAnotherCargo() const {
    return currentLoadSize < maxCapacity;
}

int FreightExtended::assignCargo(const string& cargoId, int cargoSize) {
	int acceptedSize = canAcceptMore(cargoSize);
    
    assignedCargos.push_back(cargoId+", Qty: "+to_string(acceptedSize));
    currentLoadSize += acceptedSize;

    return cargoSize - acceptedSize;
}

bool FreightExtended::removeCargo(const string& cargoId) {
    auto it = remove(assignedCargos.begin(), assignedCargos.end(), cargoId);
    if (it != assignedCargos.end()) {
        assignedCargos.erase(it, assignedCargos.end());
        currentLoadSize = (currentLoadSize > 0) ? currentLoadSize - 1 : 0;
        return true;
    }
    return false;
}

void FreightExtended::clearAssignedCargos() {
    assignedCargos.clear();
    currentLoadSize = 0;
}

const vector<string>& FreightExtended::getAssignedCargos() const {
    return assignedCargos;
}

string FreightExtended::typeToString(FreightType type) {
    return Freight::typeToString(type);
}

FreightType FreightExtended::stringToType(const string& typeStr) {
    if (typeStr == "MiniMover") return FreightType::MINI_MOVER;
    if (typeStr == "CargoCruiser") return FreightType::CARGO_CRUISER;
    if (typeStr == "MegaCarrier") return FreightType::MEGA_CARRIER;
    throw invalid_argument("Invalid freight type string");
}
