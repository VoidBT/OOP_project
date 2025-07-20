#pragma once // Include guard
#include <vector>
#include <string>
#include "Freight.h" // Make sure Freight.h is included

class FStorage {
private:
    std::vector<Freight> freights;

public:
    void addFreight(const Freight& freight);
    void editFreight(const std::string& id, int newTime, const std::string& newDest);
    void deleteFreight(const std::string& id);
    const std::vector<Freight>& getAllFreights() const;
    void printAll() const;
    Freight* findFreight(const std::string& id); // Added non-const findFreight for editing
};