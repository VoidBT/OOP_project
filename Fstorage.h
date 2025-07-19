#pragma once
#include <vector>
#include <string>
#include "Freight.h"

using namespace std;

class FStorage {
private:
    vector<Freight> freight;

public:
    void addFreight(const Freight& f);
    void editFreight(const string& id, int newTime, const string& newDest);
    void deleteFreight(const string& id);
    void clearFreights(); // Added clear method

    const Freight* getByTimeAndDest(int time, const string& dest) const;
    const vector<Freight>& getAllFreights() const;
    void printAll() const;

    // Removed loadFromFile and saveToFile - now handled by FileManager
};