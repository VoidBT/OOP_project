#pragma once // Include guard
#include <vector>
#include <string>
#include <iostream>
#include <memory>    // For shared_ptr

// Ensure all necessary headers are included
#include "FreightTypes.h" // For FreightExtended and FreightType
#include "CargoGroup.h"   // For CargoGroup
#include "Match.h"        // For Match struct
#include "FStorage.h"     // For FStorage (used by matchFreightAndCargo)
#include "CStorage.h"     // For CStorage (used by matchFreightAndCargo)
#include "Cargo.h"        // For Cargo (used internally in scheduling logic)


class ScheduleList {
private:
    std::vector<std::shared_ptr<FreightExtended>> freights;
    std::vector<CargoGroup> cargoGroups;
    std::vector<std::string> unassignedCargos;
    std::vector<Match> matches; // For basic matches, not the primary scheduling assignment

public:
    void addFreight(std::shared_ptr<FreightExtended> freight);
    void addCargoGroup(const CargoGroup& group);

    // Scheduling options (these will do the actual cargo assignments to freights)
    void scheduleByArrivalTime();
    void scheduleByFreightCapacity();

    // Display functions
    void displayByArrivalTime() const; // Displays results of arrival time scheduling
    void displayByFreightCapacity() const; // Displays results of capacity scheduling
    void displayUnderutilizedFreights() const;
    void displayUnassignedCargos() const;

    // File operations
    void saveEnhancedSchedule(const std::string& filename) const;

    // Original ScheduleList methods, adapted
    void matchFreightAndCargo(FStorage& fStorage, CStorage& cStorage); // Basic matching
    const std::vector<Match>& getMatches() const;
    void printAll() const; // Comprehensive display of schedule state

private:
    // Helper functions
    bool assignCargoToBestFreight(const Cargo& cargo); // Assumes Cargo object is passed
    bool canAssignToFreight(const FreightExtended& freight, const Cargo& cargo) const;
    void resetFreightAssignments();
};