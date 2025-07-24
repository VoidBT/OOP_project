#include "ScheduleList.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

void ScheduleList::addFreight(shared_ptr<FreightExtended> freight) {
    freights.push_back(freight);
}

void ScheduleList::addCargo(const Cargo& cargo) {
    cargoGroups.push_back(cargo);
}

const vector<shared_ptr<FreightExtended>> ScheduleList::getFreights() const {
    return freights;
}

const vector<Cargo>& ScheduleList::getCargos() const {
    return cargoGroups;
}

void ScheduleList::resetFreightAssignments() {
    for (auto& freight : freights) {
        freight->clearAssignedCargos();
    }
    unassignedCargos.clear();
}

bool ScheduleList::canAssignToFreight(const FreightExtended& freight, const Cargo& cargo) const {
    return freight.canAcceptAnotherCargo() &&
        freight.getDest() == cargo.getDest() &&
        freight.getTime() <= cargo.getTime();
}

bool ScheduleList::assignCargoToBestFreight(const Cargo& cargo) {
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

bool ScheduleList::assignGroupToFreights(const Cargo& group) {
    bool allAssigned = true;

    if (!assignCargoToBestFreight(group)) {
        unassignedCargos.push_back(group.getID());
        allAssigned = false;
    }

    return allAssigned;
}

void ScheduleList::displayByArrivalTime() const {
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

void ScheduleList::displayByFreightCapacity() const {
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

void ScheduleList::displayUnderutilizedFreights() const {
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

void ScheduleList::displayUnassignedCargos() const {
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

void ScheduleList::scheduleByFreightCapacity() {
    resetFreightAssignments();

    sort(freights.begin(), freights.end(),
        [](const auto& a, const auto& b) {
            return a->getMaxCapacity() > b->getMaxCapacity();
        });

    for (const auto& group : getCargos()) {
        assignGroupToFreights(group);
    }

    cout << "Scheduling by freight capacity completed. "
        << unassignedCargos.size() << " cargoGroups unassigned.\n";
}

void ScheduleList::scheduleByArrivalTime() {
    resetFreightAssignments();

    vector<Cargo> sortedGroups = cargoGroups;
    sort(sortedGroups.begin(), sortedGroups.end(),
        [](const Cargo& a, const Cargo& b) {
            return a.getTime() < b.getTime();
        });

    for (const auto& cargo : sortedGroups) {
        if (!assignCargoToBestFreight(cargo)) {
            unassignedCargos.push_back(cargo.getID());
        }
    }
    cout << "Scheduling by arrival time completed. " << unassignedCargos.size() << " cargos unassigned.\n";
}

void ScheduleList::saveEnhancedSchedule(const string& filename) const {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file for writing: " << filename << "\n";
        return;
    }

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

void ScheduleList::printAll() const {
    cout << "\nMatch (All Data):\n";
    cout << "===========================\n";

    cout << "\n--- Freights in ScheduleList ---\n";
    if (getFreights().empty()) {
        cout << "No freights added.\n";
    }
    else {
        vector<shared_ptr<FreightExtended>> sortedFreights = getFreights();
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

    cout << "\n--- Cargo Groups in ScheduleList ---\n";
    if (cargoGroups.empty()) {
        cout << "No cargo groups added.\n";
    }
    else {
        vector<Cargo> sortedGroups = cargoGroups;
        sort(sortedGroups.begin(), sortedGroups.end(),
            [](const Cargo& a, const Cargo& b) {
                return a.getID() < b.getID();
            });

        for (const auto& group : sortedGroups) {
            cout << "Group ID: " << group.getID()
                << ", Destination: " << group.getDest()
                << ", Size: " << group.getSize() << "/" << group.getSize()
                << ", Time Window: " << group.getTime() << "\n";
            cout << "\n";
        }
    }

    displayUnassignedCargos();
}