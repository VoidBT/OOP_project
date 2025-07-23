#pragma once
#include <vector>
#include <string>
#include <memory>
#include "FreightTypes.h"
#include "CargoGroup.h"
#include "FStorage.h"
#include "CStorage.h"
#include "Cargo.h"
#include "Freight.h"

using namespace std;

class Match {
private:
    vector<shared_ptr<FreightExtended>> freights;
    vector<CargoGroup> cargoGroups;
    vector<string> unassignedCargos;

    struct MatchPair {
        Freight freight;
        Cargo cargo;
        MatchPair(const Freight& f, const Cargo& c) : freight(f), cargo(c) {}
    };

    vector<MatchPair> matches;


public:
    void addFreight(shared_ptr<FreightExtended> freight);
    void addCargoGroup(const CargoGroup& group);

    void scheduleByArrivalTime();
    void scheduleByFreightCapacity();

    void displayByArrivalTime() const;
    void displayByFreightCapacity() const;
    void displayUnderutilizedFreights() const;
    void displayUnassignedCargos() const;

    void saveEnhancedSchedule(const string& filename) const;
    void printAll() const;

    MatchPair matchFreightAndCargo(FStorage& fStorage, CStorage& cStorage);
    vector<string> getMatches();

    
private:
    bool canAssignToFreight(const FreightExtended& freight, const Cargo& cargo) const;
    bool assignCargoToBestFreight(const Cargo& cargo);
    bool assignGroupToFreights(const CargoGroup& group);
    void resetFreightAssignments();
};
