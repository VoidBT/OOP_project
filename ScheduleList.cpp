#include "ScheduleList.h"

using namespace std;

void ScheduleList::matchFreightAndCargo(FStorage& fStorage, CStorage& cStorage) {
    matches.clear();
    for (const auto& f : fStorage.getAllFreights()) {
        if (const Cargo* c = cStorage.getByTimeAndDest(f.getTime(), f.getDest())) {
            matches.push_back(Match(f, *c));
        }
    }
}

const vector<Match>& ScheduleList::getMatches() const {
    return matches;
}

void ScheduleList::printAll() const {
    for (const auto& m : matches) {
        cout << m.freight.getID() << " | "
            << m.cargo.getID() << " | "
            << m.freight.getTime() << " | "
            << m.freight.getDest() << "\n";
    }
}