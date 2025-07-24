#pragma once
#include <vector>
#include <memory>
#include "FreightTypes.h"

using namespace std;

class ScheduleList {
private:
    vector<shared_ptr<FreightExtended>> freights;
    vector<Cargo> cargoGroups;
    vector<string> unassignedCargos;

public:
    void addFreight(shared_ptr<FreightExtended> freight);
    void addCargo(const Cargo& cargo);
    const vector<shared_ptr<FreightExtended>> getFreights() const;
    const vector<Cargo>& getCargos() const;

    void displayUnassignedCargos() const;
    void displayByArrivalTime() const;
    void displayByFreightCapacity() const;
    void displayUnderutilizedFreights() const;

    void scheduleByArrivalTime();
    void scheduleByFreightCapacity();

    void saveEnhancedSchedule(const string& filename) const;

    void printAll() const;

private:
    void resetFreightAssignments();
    bool canAssignToFreight(const FreightExtended& freight, const Cargo& cargo) const;
    bool assignCargoToBestFreight(const Cargo& cargo);
    bool assignGroupToFreights(const Cargo& group);
};
