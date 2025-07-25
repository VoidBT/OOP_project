#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "Freight.h"
#include "Cargo.h"

//Done by Ryan Ang Rui Heng 2400522

using namespace std;

class FreightExtended : public Freight {
private:
    int maxCapacity;       // Max number of cargos
    int currentLoadSize;  // Total count of cargos
    vector<string> assignedCargos;

public:
    FreightExtended(const string& id, int t, const string& d, FreightType type);

    int getMaxCapacity() const;
    int getCurrentLoadSize() const;
    bool isFull() const;

    int canAcceptMore(int cargoSize) const;
    bool canAcceptAnotherCargo() const;
    int assignCargo(const string& cargoId, int cargoSize);
    bool removeCargo(const string& cargoId);
    void clearAssignedCargos();
    const vector<string>& getAssignedCargos() const;

    static string typeToString(FreightType type);
    static FreightType stringToType(const string& typeStr);
};
