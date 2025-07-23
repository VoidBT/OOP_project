#include "ScheduleList.h"

using namespace std;

void ScheduleList::addFreight(shared_ptr<FreightExtended> freight) {
    freights.push_back(freight);
}

void ScheduleList::addCargoGroup(const CargoGroup& group) {
    cargoGroups.push_back(group);
}

const vector<shared_ptr<FreightExtended>>& ScheduleList::getFreights() const {
    return freights;
}

const vector<CargoGroup>& ScheduleList::getCargoGroups() const {
    return cargoGroups;
}
