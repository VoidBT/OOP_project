#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Match.h"
#include "FStorage.h"
#include "CStorage.h"

using namespace std;

class ScheduleList {
protected:
    vector<Match> matches;
public:
    virtual void matchFreightAndCargo(FStorage& fStorage, CStorage& cStorage);
    const vector<Match>& getMatches() const;
    virtual void printAll() const;
};