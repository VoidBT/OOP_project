#pragma once
#include <vector>
#include <string>
#include <memory>
#include "FreightTypes.h"

using namespace std;

class FStorage {
private:
    vector<shared_ptr<FreightExtended>> freights;

public:
    void addFreight(shared_ptr<FreightExtended> freight);
    void editFreight(const string& id, int newTime, const string& newDest);
    void deleteFreight(const string& id);
    const vector<shared_ptr<FreightExtended>>& getAllFreights() const;
    void printAll() const;
    shared_ptr<FreightExtended> findFreight(const string& id);
};
