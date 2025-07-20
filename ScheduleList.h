#pragma once // Add this include guard
#include <vector>
#include <string>
#include <iostream>
#include <memory>    // For shared_ptr

// Assuming these headers provide necessary definitions for FreightExtended, Cargo, CargoGroup, FStorage, CStorage, and Match
#include "FreightTypes.h"
#include "CargoGroup.h"
#include "Match.h"
#include "FStorage.h"
#include "CStorage.h"

// Removed: using namespace std; // Avoid in header files

class ScheduleList {
private:
    std::vector<std::shared_ptr<FreightExtended>> freights; // Qualify std::
    std::vector<CargoGroup> cargoGroups; // Qualify std::
    std::vector<std::string> unassignedCargos; // Qualify std::
    std::vector<Match> matches; // Qualify std::

public:
    void addFreight(std::shared_ptr<FreightExtended> freight); // Qualify std::
    void addCargoGroup(const CargoGroup& group);

    // Scheduling options
    void scheduleByArrivalTime();
    void scheduleByFreightCapacity();

    // Display functions
    void displayByArrivalTime() const;
    void displayByFreightCapacity() const;
    void displayUnderutilizedFreights() const;
    void displayUnassignedCargos() const;

    // File operations
    void saveEnhancedSchedule(const std::string& filename) const; // Qualify std::

    // Original ScheduleList methods, adapted
    void matchFreightAndCargo(FStorage& fStorage, CStorage& cStorage);
    const std::vector<Match>& getMatches() const; // Qualify std::
    void printAll() const;

private:
    // Helper functions
    void splitAndAssignGroup(const CargoGroup& group);
    bool canAssignToFreight(const FreightExtended& freight, const Cargo& cargo) const;
};