#include "FStorage.h"
#include <algorithm> // For std::remove_if
#include <iostream>

using namespace std;

void FStorage::addFreight(const Freight& freight) {
    freights.push_back(freight);
}

void FStorage::editFreight(const string& id, int newTime, const string& newDest) {
    Freight* freight = findFreight(id);
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
        [&id](const Freight& f) { return f.getID() == id; });
    if (it != freights.end()) {
        freights.erase(it, freights.end());
    }
    else {
        cout << "Freight with ID " << id << " not found.\n";
    }
}

const vector<Freight>& FStorage::getAllFreights() const {
    return freights;
}

void FStorage::printAll() const {
    cout << "\n--- All Freights ---\n";
    if (freights.empty()) {
        cout << "No freights available.\n";
        return;
    }
    for (const auto& freight : freights) {
        cout << "ID: " << freight.getID()
            << ", Time: " << freight.getTime()
            << ", Destination: " << freight.getDest() << "\n";
    }
}

Freight* FStorage::findFreight(const string& id) {
    for (auto& freight : freights) {
        if (freight.getID() == id) {
            return &freight;
        }
    }
    return nullptr;
}