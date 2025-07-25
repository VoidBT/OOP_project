#include "FStorage.h"
#include <algorithm>
#include <iostream>

//Done by Ryan Ang Rui Heng 2400522

using namespace std;

// FStorage class implementation
void FStorage::addFreight(shared_ptr<FreightExtended> freight) {
    freights.push_back(freight);
}

// Edit freight details by ID
void FStorage::editFreight(const string& id, int newTime, const string& newDest) {
    shared_ptr<FreightExtended> freight = findFreight(id);
    if (freight) {
        freight->setTime(newTime);
        freight->setDest(newDest);
    }
    else {
        cout << "Freight with ID " << id << " not found.\n";
    }
}

// Delete freight by ID
void FStorage::deleteFreight(const string& id) {
    auto it = remove_if(freights.begin(), freights.end(),
        [&id](const shared_ptr<FreightExtended>& f) { return f->getID() == id; });
    if (it != freights.end()) {
        freights.erase(it, freights.end());
    }
    else {
        cout << "Freight with ID " << id << " not found.\n";
    }
}

// Get all freights
const vector<shared_ptr<FreightExtended>>& FStorage::getAllFreights() const {
    return freights;
}

// Print all freights in storage
void FStorage::printAll() const {
    cout << "\n--- All Freights ---\n";
    if (freights.empty()) {
        cout << "No freights available.\n";
        return;
    }
    for (const auto& freight : freights) {
        cout << "ID: " << freight->getID()
            << ", Type: " << FreightExtended::typeToString(freight->getType())
            << ", Time: " << freight->getTime().getRawTime()
            << ", Destination: " << freight->getDest()
            << ", Capacity: " << freight->getMaxCapacity() << "\n";
    }
}

// Find freight by ID
shared_ptr<FreightExtended> FStorage::findFreight(const string& id) {
    for (auto& freight : freights) {
        if (freight->getID() == id) {
            return freight;
        }
    }
    return nullptr;
}

// Clear all freights in storage
void FStorage::clear() {
    for(auto& ptr : freights) {
		FreightExtended* freightPtr = ptr.get();
		freightPtr->clearAssignedCargos(); // Clear assigned cargos before resetting
        ptr.reset(); // Clear shared_ptr
	}
    freights.clear();
}