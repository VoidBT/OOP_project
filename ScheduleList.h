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

using namespace std;

class ScheduleList {
private:
    vector<shared_ptr<FreightExtended>> freights;
    vector<CargoGroup> cargoGroups;
    vector<string> unassignedCargos;
    vector<Match> matches;

public:
    void addFreight(shared_ptr<FreightExtended> freight);
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
    void saveEnhancedSchedule(const string& filename) const;

    // Original ScheduleList methods, adapted
    void matchFreightAndCargo(FStorage& fStorage, CStorage& cStorage);
    const vector<Match>& getMatches() const;
    void printAll() const;

private:
    // Helper functions
    void splitAndAssignGroup(const CargoGroup& group);
    bool canAssignToFreight(const FreightExtended& freight, const Cargo& cargo) const;
};