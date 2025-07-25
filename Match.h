#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Cargo.h"
#include "Freight.h"
#include "FStorage.h"
#include "CStorage.h"

//Done by Ryan Ang Rui Heng 2400522

using namespace std;

/*
	Match holds the temporary data for matching freights and cargos.
	Before finally the data goes to ScheduleList.
*/
class Match {
private:
	// Pairing of Freight and Cargo
    struct MatchPair {
        Freight freight;
        Cargo cargo;
        MatchPair(const Freight& f, const Cargo& c) : freight(f), cargo(c) {}
    };

    vector<MatchPair> matches;

public:
    void matchFreightAndCargo(FStorage& fStorage, CStorage& cStorage);

    void addMatch(const Freight& freight, const Cargo& cargo);
    vector<string> getMatches() const;
    void printAll() const;

protected:
    bool isEmpty() const;
};
