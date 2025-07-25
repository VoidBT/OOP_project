#include "ScheduleList.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>

//Done by Ryan Ang Rui Heng 2400522

using namespace std;

// Adds a FreightExtended object to the schedule list
void ScheduleList::addFreight(shared_ptr<FreightExtended> freight) {
    freights.push_back(freight);
}

// Adds a Cargo object to the cargo groups in the schedule list
void ScheduleList::addCargo(const Cargo& cargo) {
    cargoGroups.push_back(cargo);
}

// Returns a constant reference to the list of FreightExtended objects
const vector<shared_ptr<FreightExtended>> ScheduleList::getFreights() const {
    return freights;
}

// Returns a constant reference to the list of Cargo objects
const vector<Cargo>& ScheduleList::getCargos() const {
    return cargoGroups;
}

// Resets the schedule list by clearing all freights, cargo groups, and unassigned cargos
void ScheduleList::reset() {
    for (auto& freight : freights) {
        freight->clearAssignedCargos();
    }
    freights.clear();
    cargoGroups.clear();
    unassignedCargos.clear();
}

// Resets the freight assignments by clearing assigned cargos for each freight
void ScheduleList::resetFreightAssignments() {
    for (auto& freight : freights) {
        freight->clearAssignedCargos();
    }
    unassignedCargos.clear();
}

// Checks if a cargo can be assigned to a freight based on destination, time, and capacity
bool ScheduleList::canAssignToFreight(const FreightExtended& freight, const Cargo& cargo) const {
    return freight.canAcceptAnotherCargo() &&
        freight.getDest() == cargo.getDest() &&
        freight.getTime().getRawTime() <= cargo.getTime().getRawTime() &&
        cargo.getTime().isWithinLimit(freight.getTime());
}

// Assigns a cargo to the best available freight based on arrival time
bool ScheduleList::assignCargoToBestFreight(Cargo& cargo) {
    sort(freights.begin(), freights.end(),
        [](const auto& a, const auto& b) { return a->getTime().getRawTime() < b->getTime().getRawTime(); });

    int cargoleft = 0;

    for (auto& freight : freights) {
        if (canAssignToFreight(*freight, cargo)) {
            cargoleft = freight->assignCargo(cargo.getID(), cargo.getSize());
            cargo.setSize(cargoleft);
            if (cargoleft == 0) {
				return true;
            }
        }
    }
    return false;
}

// Assigns a cargo group to the best available freight, returning true if all cargos are assigned
bool ScheduleList::assignGroupToFreights(Cargo& group) {
    bool allAssigned = true;

    if (!assignCargoToBestFreight(group)) {
        unassignedCargos.push_back(group.getID());
        allAssigned = false;
    }

    return allAssigned;
}

// Displays the scheduling plan sorted by freight arrival time
void ScheduleList::displayByArrivalTime() const {
    vector<shared_ptr<FreightExtended>> sortedFreights = freights;
    sort(sortedFreights.begin(), sortedFreights.end(),
        [](const auto& a, const auto& b) { return a->getTime().getRawTime() < b->getTime().getRawTime(); });

    cout << "\nScheduling Plan Sorted by Freight Arrival Time:\n";
    cout << "===============================================\n";
    for (const auto& freight : sortedFreights) {
        cout << "Freight " << freight->getID()
            << " (" << FreightExtended::typeToString(freight->getType()) << ")"
            << " - Time: " << freight->getTime().getRawTime()
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

// Displays the scheduling plan sorted by freight capacity (most used first)
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
            << ", Time: " << freight->getTime().getRawTime() << "\n";

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

// Displays underutilized freights that are not at full capacity by size
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
                << ", Time: " << freight->getTime().getRawTime() << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << "All freights are at full capacity by size or no freights available.\n";
    }
}

// Displays unassigned cargos that could not be scheduled
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

// Schedules freights by their capacity, assigning cargo groups to the most suitable freights
void ScheduleList::scheduleByFreightCapacity() {
    resetFreightAssignments();

    sort(freights.begin(), freights.end(),
        [](const auto& a, const auto& b) {
            return a->getMaxCapacity() > b->getMaxCapacity();
        });

    for (auto& group : cargoGroups) {
        assignGroupToFreights(group);
    }

    cout << "Scheduling by freight capacity completed. "
        << unassignedCargos.size() << " cargoGroups unassigned.\n";
}

// Schedules freights by their arrival time, assigning cargo groups to the best available freights
void ScheduleList::scheduleByArrivalTime() {
    resetFreightAssignments();

    sort(cargoGroups.begin(), cargoGroups.end(),
        [](const Cargo& a, const Cargo& b) {
            return a.getTime().getRawTime() < b.getTime().getRawTime();
        });

    for (auto& cargo : cargoGroups) {
        if (!assignCargoToBestFreight(cargo)) {
            unassignedCargos.push_back(cargo.getID());
        }
    }
    cout << "Scheduling by arrival time completed. " << unassignedCargos.size() << " cargos unassigned.\n";
}

// Saves the enhanced schedule to a file with detailed information about each freight and its assigned cargos
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
            << setw(5) << left << freight->getTime().getRawTime() << " | ";

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

// Prints all data in the schedule list, including freights, cargo groups, and unassigned cargos
void ScheduleList::printAll() const {
    cout << "\n(All Data):\n";
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
                << ", Time: " << freight->getTime().getRawTime()
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
                << ", Size: " << group.getSize()
                << ", Time Window: " << group.getTime().getRawTime() << "\n";
            cout << "\n";
        }
    }

    displayUnassignedCargos();
}