#pragma once
#include <vector>
#include <memory>
#include "FreightTypes.h"
#include "CargoGroup.h"

using namespace std;

class ScheduleList {
private:
    vector<shared_ptr<FreightExtended>> freights;
    vector<CargoGroup> cargoGroups;

public:
    void addFreight(shared_ptr<FreightExtended> freight);
    void addCargoGroup(const CargoGroup& group);

    const vector<shared_ptr<FreightExtended>>& getFreights() const;
    const vector<CargoGroup>& getCargoGroups() const;
};
