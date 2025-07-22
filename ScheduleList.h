#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <memory>

#include "FreightTypes.h"
#include "CargoGroup.h"
#include "Match.h"
#include "FStorage.h"
#include "CStorage.h"
#include "Cargo.h" // Ensure Cargo.h is included for Cargo object

class ScheduleList {
private:
    std::vector<std::shared_ptr<FreightExtended>> freights;
    std::vector<CargoGroup> cargoGroups;
    std::vector<std::string> unassignedCargos;
    std::vector<Match> matches;

public:
    void addFreight(std::shared_ptr<FreightExtended> freight);
    void addCargoGroup(const CargoGroup& group);

    void scheduleByArrivalTime();
    void scheduleByFreightCapacity();

    void displayByArrivalTime() const;
    void displayByFreightCapacity() const;
    void displayUnderutilizedFreights() const;
    void displayUnassignedCargos() const;

    void saveEnhancedSchedule(const std::string& filename) const;

    void matchFreightAndCargo(FStorage& fStorage, CStorage& cStorage);
    const std::vector<Match>& getMatches() const;
    void printAll() const;

private:
    // NEW: canAssignToFreight now takes const Cargo&
    bool canAssignToFreight(const FreightExtended& freight, const Cargo& cargo) const;
    // NEW: assignCargoToBestFreight now takes const Cargo&
    bool assignCargoToBestFreight(const Cargo& cargo);
    void resetFreightAssignments();
};