#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "Freight.h"
#include "Cargo.h"

using namespace std;

class FreightExtended : public Freight {
private:
    int maxCapacity;       // Max number of cargos
    int currentLoadSize;   // Total size of cargos
    int currentLoadCount;  // Total count of cargos
    vector<string> assignedCargos;

public:
    FreightExtended(const string& id, int t, const string& d, FreightType type);

    int getMaxCapacity() const;
    int getCurrentLoadCount() const;
    int getCurrentLoadSize() const;
    bool isFull() const;

    bool canAcceptMore(int cargoSize) const;
    bool canAcceptAnotherCargo() const;
    bool assignCargo(const string& cargoId, int cargoSize);
    bool removeCargo(const string& cargoId);
    void clearAssignedCargos();
    const vector<string>& getAssignedCargos() const;

    static string typeToString(FreightType type);
    static FreightType stringToType(const string& typeStr);
};
