#include "FStorage.h"
#include <algorithm>
#include <iostream>

using namespace std;

void FStorage::addFreight(shared_ptr<FreightExtended> freight) {
    freights.push_back(freight);
}

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

const vector<shared_ptr<FreightExtended>>& FStorage::getAllFreights() const {
    return freights;
}

void FStorage::printAll() const {
    cout << "\n--- All Freights ---\n";
    if (freights.empty()) {
        cout << "No freights available.\n";
        return;
    }
    for (const auto& freight : freights) {
        cout << "ID: " << freight->getID()
            << ", Type: " << FreightExtended::typeToString(freight->getType())
            << ", Time: " << freight->getTime()
            << ", Destination: " << freight->getDest()
            << ", Capacity: " << freight->getMaxCapacity() << "\n";
    }
}

shared_ptr<FreightExtended> FStorage::findFreight(const string& id) {
    for (auto& freight : freights) {
        if (freight->getID() == id) {
            return freight;
        }
    }
    return nullptr;
}
