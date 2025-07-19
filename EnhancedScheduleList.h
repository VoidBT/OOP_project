#ifndef ENHANCEDSCHEDULELIST_H
#define ENHANCEDSCHEDULELIST_H

#include "ScheduleList.h"
#include "FreightTypes.h"
#include "CargoGroup.h"
#include <vector>
#include <memory>

using namespace std;

class EnhancedScheduleList : public ScheduleList {
private:
    vector<shared_ptr<FreightExtended>> freights;
    vector<CargoGroup> cargoGroups;
    vector<string> unassignedCargos;

public:
    void addFreight(shared_ptr<FreightExtended> freight);
    void addCargoGroup(const CargoGroup& group);

    // New scheduling options
    void scheduleByArrivalTime();
    void scheduleByFreightCapacity();

    // Display functions
    void displayByArrivalTime() const;
    void displayByFreightCapacity() const;
    void displayUnderutilizedFreights() const;
    void displayUnassignedCargos() const;

    // File operations
    void saveEnhancedSchedule(const string& filename) const;

    // Override base class methods
    void matchFreightAndCargo(FStorage& fStorage, CStorage& cStorage) override;
    void printAll() const override;

private:
    void splitAndAssignGroup(const CargoGroup& group);
    bool canAssignToFreight(const FreightExtended& freight, const Cargo& cargo) const;
};
#endif