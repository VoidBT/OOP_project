#pragma once
#include <vector>
#include <string>
#include <memory>
#include "FreightTypes.h"
#include "FStorage.h"
#include "CStorage.h"
#include "Cargo.h"
#include "Freight.h"

using namespace std;

/*
	Match holds the temporary data for matching freights and cargos.
	Before finally the data goes to ScheduleList.
*/
class Match {
private:
    vector<shared_ptr<FreightExtended>> freights;
    vector<Cargo> cargoGroups;
    vector<string> unassignedCargos;

public:
    void addFreight(shared_ptr<FreightExtended> freight);
    void addCargo(const Cargo& cargo);
    const vector<shared_ptr<FreightExtended>> getFreights() const;
    const vector<Cargo>& getCargos() const;

    void scheduleByArrivalTime();
    void scheduleByFreightCapacity();

    void displayByArrivalTime() const;
    void displayByFreightCapacity() const;
    void displayUnderutilizedFreights() const;
    void displayUnassignedCargos() const;

    void saveEnhancedSchedule(const string& filename) const;
    vector<int> matchFreightAndCargo(FStorage& fStorage, CStorage& cStorage);

private:
    bool canAssignToFreight(const FreightExtended& freight, const Cargo& cargo) const;
    bool assignCargoToBestFreight(const Cargo& cargo);
    bool assignGroupToFreights(const Cargo& group);
    void resetFreightAssignments();
};
