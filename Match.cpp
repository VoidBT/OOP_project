#include "Match.h"
#include <iostream>

using namespace std;

bool Match::isEmpty() const {
    return matches.empty();
}

void Match::matchFreightAndCargo(FStorage& freightStorage, CStorage& cargoStorage) {
    if (freightStorage.getAllFreights().empty() || cargoStorage.getAllCargos().empty()) {
        cout << "Cannot perform basic matching: No freights or cargos available in storage.\n";
		matches.push_back(MatchPair(Freight(), Cargo()));
		return;
    }

    shared_ptr<FreightExtended> firstFreightExtended = freightStorage.getAllFreights()[0];
    Freight firstFreight(firstFreightExtended->getID(),
        firstFreightExtended->getTime().getRawTime(),
        firstFreightExtended->getDest(),
        firstFreightExtended->getType());
    Cargo firstCargo = cargoStorage.getAllCargos()[0];

    if (firstFreight.getDest() == firstCargo.getDest() && firstFreight.getTime().getRawTime() >= firstCargo.getTime().getRawTime()) {
        matches.push_back(MatchPair(firstFreight, firstCargo));
        cout << "Basic match found and added: Freight " << firstFreight.getID()
            << " with Cargo " << firstCargo.getID() << "\n";
    }
    else {
        cout << "No basic match found based on simple criteria.\n";
        matches.push_back(MatchPair(Freight(), Cargo()));
    }
}

void Match::addMatch(const Freight& freight, const Cargo& cargo) {
    matches.emplace_back(freight, cargo);
}

vector<string> Match::getMatches() const {
    vector<string> matchdata;
    for (Match::MatchPair match : matches) {
        string data = "Freight ID: " + match.freight.getID() +
            ", Cargo ID: " + match.cargo.getID() +
            ", Freight Time: " + to_string(match.freight.getTime().getRawTime()) +
            ", Cargo Time: " + to_string(match.cargo.getTime().getRawTime()) +
            ", Destination: " + match.freight.getDest();

        matchdata.push_back(data);
    }
    return matchdata;
}

void Match::printAll() const {
    cout << "\n--- Basic Matches ---\n";
    if (isEmpty()) {
        cout << "No basic matches generated.\n";
    }
    else {
        for (std::string match : getMatches()) {
            cout << match << "\n";
        }
    }
    cout << "===========================\n";
}