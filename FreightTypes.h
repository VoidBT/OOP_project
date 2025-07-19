#ifndef FREIGHTTYPES_H
#define FREIGHTTYPES_H

#include "Freight.h"
#include <vector>
#include <memory>

using namespace std;

enum class FreightType { MINI_MOVER, CARGO_CRUISER, MEGA_CARRIER };

class FreightExtended : public Freight {
private:
    FreightType type;
    int maxCapacity;
    vector<string> assignedCargos;

public:
    FreightExtended(const string& id, int t, const string& d, FreightType type);

    FreightType getType() const;
    int getMaxCapacity() const;
    int getCurrentLoad() const;
    bool isFull() const;
    bool canAcceptMore() const;
    bool assignCargo(const string& cargoId);
    bool removeCargo(const string& cargoId);
    const vector<string>& getAssignedCargos() const;

    static string typeToString(FreightType type);
    static FreightType stringToType(const string& typeStr);
};

#endif