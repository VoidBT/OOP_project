#include "FStorage.h"
#include <fstream> // Not strictly needed anymore if file I/O is moved
#include <algorithm>
#include <iostream>
#include <sstream> // Not strictly needed anymore if parsing is moved

using namespace std;

void FStorage::addFreight(const Freight& f) {
    freight.push_back(f);
}

void FStorage::editFreight(const string& id, int newTime, const string& newDest) {
    for (auto& f : freight) {
        if (f.getID() == id) {
            f.setTime(newTime);
            f.setDest(newDest);
            break;
        }
    }
}

void FStorage::deleteFreight(const string& id) {
    freight.erase(
        remove_if(freight.begin(), freight.end(),
            [&id](const Freight& f) { return f.getID() == id; }),
        freight.end()
    );
}

void FStorage::clearFreights() {
    freight.clear();
}

const Freight* FStorage::getByTimeAndDest(int time, const string& dest) const {
    for (const auto& f : freight) {
        if (f.getTime() == time && f.getDest() == dest) {
            return &f;
        }
    }
    return nullptr;
}

const vector<Freight>& FStorage::getAllFreights() const {
    return freight;
}

void FStorage::printAll() const {
    for (const auto& f : freight) {
        cout << f.getID() << " | "
            << f.getTime() << " | "
            << f.getDest() << "\n";
    }
}

// Removed loadFromFile and saveToFile implementations from here.
// They are now handled by FileManager.