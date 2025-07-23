#pragma once
#include <vector>
#include <memory>
#include "FreightTypes.h"
#include "CargoGroup.h"
#include "Match.h"

using namespace std;

class ScheduleList : public Match {
private:
    struct ScheduledEntry {
        Freight freight;
        Cargo cargo;
        ScheduledEntry(const Freight& f, const Cargo& c) : freight(f), cargo(c) {}
    };

    vector<ScheduledEntry> scheduled;

public:
    void addScheduledEntry(const Freight& freight, const Cargo& cargo);
    vector<string> getScheduled();
    void printAll() const;
};
