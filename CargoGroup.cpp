#include "CargoGroup.h"
#include <algorithm>
using namespace std;

CargoGroup::CargoGroup(int id, const string& dest, int timeWindow)
    : groupId(id), destination(dest), timeWindow(timeWindow), GrpSize(0) {
}

bool CargoGroup::addCargo(const Cargo& cargo) {
    // Ensure cargo destination matches group destination
    if (cargos.size() >= maxSize || cargo.getDest() != destination) {
        return false;
    }

	GrpSize += cargo.getSize();

    cargos.push_back(cargo);
    return true;
}

bool CargoGroup::removeCargo(const string& cargoId) {
    auto it = remove_if(cargos.begin(), cargos.end(),
        [&cargoId](const Cargo& c) { return c.getID() == cargoId; });

    if (it != cargos.end()) {
        cargos.erase(it, cargos.end());
        return true;
    }
    return false;
}

bool CargoGroup::containsCargo(const string& cargoId) const {
    return any_of(cargos.begin(), cargos.end(),
        [&cargoId](const Cargo& c) { return c.getID() == cargoId; });
}

bool CargoGroup::isFull() const {
    return cargos.size() >= maxSize;
}

bool CargoGroup::isEmpty() const {
    return cargos.empty();
}

bool CargoGroup::canMergeWith(const CargoGroup& other) const {
    return destination == other.destination &&
        (cargos.size() + other.cargos.size()) <= maxSize;
}

// Getters and setters implementation
int CargoGroup::getGroupId() const { return groupId; }
string CargoGroup::getDestination() const { return destination; }
size_t CargoGroup::getSize() const { return cargos.size(); }
int CargoGroup::getMaxSize() const { return maxSize; }
Time CargoGroup::getTimeWindow() const { return timeWindow; }
const vector<Cargo>& CargoGroup::getCargos() const { return cargos; } // Returns const reference
int CargoGroup::getGrpSize() const { return GrpSize; }

void CargoGroup::setDestination(const string& newDest) { destination = newDest; }
void CargoGroup::setTimeWindow(int newWindow) { timeWindow = newWindow; }