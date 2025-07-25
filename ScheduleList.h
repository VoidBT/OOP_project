#pragma once
#include <vector>
#include <memory>
#include "FreightTypes.h"

//Done by Ryan Ang Rui Heng 2400522

using namespace std;

class ScheduleList {
private:
	// List of freights and cargo groups
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

    void reset();

private:
    void resetFreightAssignments();
    bool canAssignToFreight(const FreightExtended& freight, const Cargo& cargo) const;
    bool assignCargoToBestFreight(Cargo& cargo);
    bool assignGroupToFreights(Cargo& group);
};
