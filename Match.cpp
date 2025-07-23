#include "Match.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

void Match::addFreight(shared_ptr<FreightExtended> freight) {
    freights.push_back(freight);
}

void Match::addCargoGroup(const CargoGroup& group) {
    cargoGroups.push_back(group);
}

void Match::resetFreightAssignments() {
    for (auto& freight : freights) {
        freight->clearAssignedCargos();
    }
    unassignedCargos.clear();
}

bool Match::canAssignToFreight(const FreightExtended& freight, const Cargo& cargo) const {
    return freight.canAcceptAnotherCargo() &&
        freight.getDest() == cargo.getDest() &&
        freight.getTime() >= cargo.getTime() &&
        (freight.getTime() - cargo.getTime()) <= 15;
}

bool Match::assignCargoToBestFreight(const Cargo& cargo) {
    sort(freights.begin(), freights.end(),
        [](const auto& a, const auto& b) { return a->getTime() < b->getTime(); });

    for (auto& freight : freights) {
        if (canAssignToFreight(*freight, cargo)) {
            freight->assignCargo(cargo.getID(), cargo.getSize());
            return true;
        }
    }
    return false;
}

bool Match::assignGroupToFreights(const CargoGroup& group) {
    auto cargos = group.getCargos();
    bool allAssigned = true;

    for (const auto& cargo : cargos) {
        if (!assignCargoToBestFreight(cargo)) {
            unassignedCargos.push_back(cargo.getID());
            allAssigned = false;
        }
    }
    return allAssigned;
}

void Match::scheduleByFreightCapacity() {
    resetFreightAssignments();

    sort(freights.begin(), freights.end(),
        [](const auto& a, const auto& b) { return a->getMaxCapacity() > b->getMaxCapacity(); });

    for (const auto& group : cargoGroups) {
        assignGroupToFreights(group);
    }

    cout << "Scheduling by freight capacity completed. "
        << unassignedCargos.size() << " cargos unassigned.\n";
}

void Match::scheduleByArrivalTime() {
    resetFreightAssignments();

    sort(freights.begin(), freights.end(),
        [](const auto& a, const auto& b) { return a->getTime() < b->getTime(); });

    vector<Cargo> allCargosFromGroups;
    for (const auto& group : cargoGroups) {
        const auto& groupCargos = group.getCargos();
        allCargosFromGroups.insert(allCargosFromGroups.end(), groupCargos.begin(), groupCargos.end());
    }

    sort(allCargosFromGroups.begin(), allCargosFromGroups.end(),
        [](const Cargo& a, const Cargo& b) { return a.getTime() < b.getTime(); });

    for (const auto& cargo : allCargosFromGroups) {
        if (!assignCargoToBestFreight(cargo)) {
            unassignedCargos.push_back(cargo.getID());
        }
    }
    cout << "Scheduling by arrival time completed. " << unassignedCargos.size() << " cargos unassigned.\n";
}

void Match::displayByArrivalTime() const {
    vector<shared_ptr<FreightExtended>> sortedFreights = freights;
    sort(sortedFreights.begin(), sortedFreights.end(),
        [](const auto& a, const auto& b) { return a->getTime() < b->getTime(); });

    cout << "\nScheduling Plan Sorted by Freight Arrival Time:\n";
    cout << "===============================================\n";
    for (const auto& freight : sortedFreights) {
        cout << "Freight " << freight->getID()
            << " (" << FreightExtended::typeToString(freight->getType()) << ")"
            << " - Time: " << freight->getTime()
            << ", Destination: " << freight->getDest()
            << ", Load: " << freight->getCurrentLoadSize() << "/" << freight->getMaxCapacity() << " (size)\n";

        if (!freight->getAssignedCargos().empty()) {
            cout << "  Assigned Cargos (ID): \n";
            for (const auto& cargoId : freight->getAssignedCargos()) {
                cout << "    - " << cargoId << "\n";
            }
        }
        else {
            cout << "  No cargos assigned.\n";
        }
    }
}

void Match::displayByFreightCapacity() const {
    vector<shared_ptr<FreightExtended>> sortedFreights = freights;
    sort(sortedFreights.begin(), sortedFreights.end(),
        [](const auto& a, const auto& b) {
            if (a->getMaxCapacity() != b->getMaxCapacity())
                return a->getMaxCapacity() > b->getMaxCapacity();
            return a->getCurrentLoadSize() > b->getCurrentLoadSize();
        });

    cout << "\nScheduling Plan Sorted by Freight Capacity (Most Used First):\n";
    cout << "============================================================\n";
    for (const auto& freight : sortedFreights) {
        cout << "Freight " << freight->getID()
            << " (" << FreightExtended::typeToString(freight->getType()) << ")"
            << " - Capacity: " << freight->getMaxCapacity()
            << ", Current Load: " << freight->getCurrentLoadSize() << " (size)"
            << ", Destination: " << freight->getDest()
            << ", Time: " << freight->getTime() << "\n";

        if (!freight->getAssignedCargos().empty()) {
            cout << "  Assigned Cargos (ID): \n";
            for (const auto& cargoId : freight->getAssignedCargos()) {
                cout << "    - " << cargoId << "\n";
            }
        }
        else {
            cout << "  No cargos assigned.\n";
        }
    }
}

void Match::displayUnderutilizedFreights() const {
    cout << "\nUnderutilized Freights (Not at Full Capacity by Size):\n";
    cout << "=============================================\n";
    bool found = false;
    for (const auto& freight : freights) {
        if (freight->getCurrentLoadSize() < freight->getMaxCapacity()) {
            cout << "Freight " << freight->getID()
                << " - Type: " << FreightExtended::typeToString(freight->getType())
                << ", Current Load: " << freight->getCurrentLoadSize()
                << ", Max Capacity: " << freight->getMaxCapacity()
                << " (" << (freight->getMaxCapacity() - freight->getCurrentLoadSize()) << " size units available)"
                << ", Destination: " << freight->getDest()
                << ", Time: " << freight->getTime() << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << "All freights are at full capacity by size or no freights available.\n";
    }
}

void Match::displayUnassignedCargos() const {
    cout << "\n--- Unassigned Cargos ---\n";
    if (unassignedCargos.empty()) {
        cout << "All cargos have been assigned.\n";
    }
    else {
        for (const auto& cargoId : unassignedCargos) {
            cout << "- " << cargoId << "\n";
        }
    }
}

Match::MatchPair Match::matchFreightAndCargo(FStorage& freightStorage, CStorage& cargoStorage) {
    if (freightStorage.getAllFreights().empty() || cargoStorage.getAllCargos().empty()) {
        cout << "Cannot perform basic matching: No freights or cargos available in storage.\n";
        return MatchPair(Freight("", 0, "", FreightType::MINI_MOVER), Cargo("", 0, "", 0));
    }

    shared_ptr<FreightExtended> firstFreightExtended = freightStorage.getAllFreights()[0];
    Freight firstFreight(firstFreightExtended->getID(),
        firstFreightExtended->getTime(),
        firstFreightExtended->getDest(),
        firstFreightExtended->getType());
    Cargo firstCargo = cargoStorage.getAllCargos()[0];

    if (firstFreight.getDest() == firstCargo.getDest() && firstFreight.getTime() >= firstCargo.getTime()) {
        MatchPair newMatch(firstFreight, firstCargo);
        matches.push_back(newMatch);
        cout << "Basic match found and added: Freight " << firstFreight.getID()
            << " with Cargo " << firstCargo.getID() << "\n";
        return newMatch;
    }
    else {
        cout << "No basic match found based on simple criteria.\n";
        return MatchPair(Freight("", 0, "", FreightType::MINI_MOVER), Cargo("", 0, "", 0));
    }
}

vector<string> Match::getMatches() {
    vector<string> matchdata;
    for (auto match : matches) {
        string data = "Freight ID: " + match.freight.getID() +
            ", Cargo ID: " + match.cargo.getID() +
            ", Freight Time: " + to_string(match.freight.getTime()) +
            ", Cargo Time: " + to_string(match.cargo.getTime()) +
			", Destination: " + match.freight.getDest();

        matchdata.push_back(data);
    }
    return matchdata;
}

void Match::saveEnhancedSchedule(const string& filename) const {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file for writing: " << filename << "\n";
        return;
    }

    file << "Freight ID | Type         | Load/Cap (Size) | Destination | Time | Assigned Cargos\n";
    file << "--------------------------------------------------------------------------------\n";

    vector<shared_ptr<FreightExtended>> sortedFreights = freights;
    sort(sortedFreights.begin(), sortedFreights.end(),
        [](const auto& a, const auto& b) { return a->getID() < b->getID(); });

    for (const auto& freight : sortedFreights) {
        file << setw(10) << left << freight->getID() << " | "
            << setw(12) << left << FreightExtended::typeToString(freight->getType()) << " | "
            << setw(15) << left << to_string(freight->getCurrentLoadSize()) + "/" + to_string(freight->getMaxCapacity()) << " | "
            << setw(12) << left << freight->getDest() << " | "
            << setw(5) << left << freight->getTime() << " | ";

        bool firstCargo = true;
        for (const auto& cargoId : freight->getAssignedCargos()) {
            if (!firstCargo) {
                file << ", ";
            }
            file << cargoId;
            firstCargo = false;
        }
        file << "\n";
    }

    if (!unassignedCargos.empty()) {
        file << "\nUnassigned Cargos:\n";
        for (const auto& cargoId : unassignedCargos) {
            file << cargoId << "\n";
        }
    }
    cout << "Enhanced schedule saved to " << filename << endl;
}

void Match::printAll() const {
    cout << "\nMatch (All Data):\n";
    cout << "===========================\n";

    cout << "\n--- Freights in Match ---\n";
    if (freights.empty()) {
        cout << "No freights added to match.\n";
    }
    else {
        vector<shared_ptr<FreightExtended>> sortedFreights = freights;
        sort(sortedFreights.begin(), sortedFreights.end(),
            [](const auto& a, const auto& b) { return a->getID() < b->getID(); });

        for (const auto& freight : sortedFreights) {
            cout << "Freight ID: " << freight->getID()
                << ", Type: " << FreightExtended::typeToString(freight->getType())
                << ", Time: " << freight->getTime()
                << ", Destination: " << freight->getDest()
                << ", Load: " << freight->getCurrentLoadSize() << "/" << freight->getMaxCapacity() << " (size)\n";
            cout << "  Assigned Cargos: ";
            if (freight->getAssignedCargos().empty()) {
                cout << "None";
            }
            else {
                bool first = true;
                for (const auto& cargoId : freight->getAssignedCargos()) {
                    if (!first) cout << ", ";
                    cout << cargoId;
                    first = false;
                }
            }
            cout << "\n";
        }
    }

    cout << "\n--- Cargo Groups in Match ---\n";
    if (cargoGroups.empty()) {
        cout << "No cargo groups added.\n";
    }
    else {
        vector<CargoGroup> sortedGroups = cargoGroups;
        sort(sortedGroups.begin(), sortedGroups.end(),
            [](const CargoGroup& a, const CargoGroup& b) {
                return a.getGroupId() < b.getGroupId();
            });

        for (const auto& group : sortedGroups) {
            cout << "Group ID: " << group.getGroupId()
                << ", Destination: " << group.getDestination()
                << ", Size: " << group.getSize() << "/" << group.getMaxSize()
                << ", Time Window: " << group.getTimeWindow() << "\n";
            cout << "  Cargos in Group: ";
            if (group.getCargos().empty()) {
                cout << "None";
            }
            else {
                bool first = true;
                for (const auto& cargo : group.getCargos()) {
                    if (!first) cout << ", ";
                    cout << cargo.getID() << " (Size: " << cargo.getSize() << ")";
                    first = false;
                }
            }
            cout << "\n";
        }
    }

    displayUnassignedCargos();

    cout << "\n--- Basic Matches ---\n";
    if (matches.empty()) {
        cout << "No basic matches generated.\n";
    }
    else {
        for (const auto& match : matches) {
            cout << "Freight: " << match.freight.getID()
                << ", Cargo: " << match.cargo.getID()
                << ", Time: " << match.freight.getTime()
                << ", Destination: " << match.freight.getDest() << "\n";
        }
    }
    cout << "===========================\n";
}
