#include "CargoGroup.h"
#include <algorithm>
using namespace std;

CargoGroup::CargoGroup(const string& id, const string& dest, int maxSize, int timeWindow)
    : groupId(id), destination(dest), maxSize(maxSize), timeWindow(timeWindow) {
}

bool CargoGroup::addCargo(const Cargo& cargo) {
    // Ensure cargo destination matches group destination
    if (cargos.size() >= maxSize || cargo.getDest() != destination) {
        return false;
    }
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
string CargoGroup::getGroupId() const { return groupId; }
string CargoGroup::getDestination() const { return destination; }
int CargoGroup::getSize() const { return cargos.size(); }
int CargoGroup::getMaxSize() const { return maxSize; }
int CargoGroup::getTimeWindow() const { return timeWindow; }
const vector<Cargo>& CargoGroup::getCargos() const { return cargos; } // Returns const reference

void CargoGroup::setDestination(const string& newDest) { destination = newDest; }
void CargoGroup::setTimeWindow(int newWindow) { timeWindow = newWindow; }