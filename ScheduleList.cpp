#include "ScheduleList.h"
#include <algorithm> // For std::sort
#include <fstream>   // For file operations
#include <iomanip> // For output formatting

using namespace std;

void ScheduleList::addFreight(shared_ptr<FreightExtended> freight) {
    freights.push_back(freight);
}

void ScheduleList::addCargoGroup(const CargoGroup& group) {
    cargoGroups.push_back(group);
}

void ScheduleList::scheduleByArrivalTime() {
    // Sort freights and cargos by time
    sort(freights.begin(), freights.end(),
        [](const auto& a, const auto& b) {
            return a->getTime() < b->getTime();
        });

    for (auto& group : cargoGroups) {
        auto cargos = group.getCargos();
        sort(cargos.begin(), cargos.end(),
            [](const Cargo& a, const Cargo& b) {
                return a.getTime() < b.getTime();
            });

        // Try to assign each cargo to the earliest possible freight
        for (const auto& cargo : cargos) {
            bool assigned = false;
            for (auto& freight : freights) {
                if (canAssignToFreight(*freight, cargo)) {
                    freight->assignCargo(cargo.getID());
                    assigned = true;
                    break;
                }
            }
            if (!assigned) {
                unassignedCargos.push_back(cargo.getID());
            }
        }
    }
}

void ScheduleList::scheduleByFreightCapacity() {
    // Sort freights by capacity (descending)
    sort(freights.begin(), freights.end(),
        [](const auto& a, const auto& b) {
            return a->getMaxCapacity() > b->getMaxCapacity();
        });

    for (auto& group : cargoGroups) {
        // Try to assign entire group first
        bool groupAssigned = false;
        for (auto& freight : freights) {
            if (freight->canAcceptMore() &&
                freight->getCurrentLoad() + group.getSize() <= freight->getMaxCapacity()) {
                for (const auto& cargo : group.getCargos()) {
                    freight->assignCargo(cargo.getID());
                }
                groupAssigned = true;
                break;
            }
        }

        if (!groupAssigned) {
            splitAndAssignGroup(group);
        }
    }
}

void ScheduleList::displayByArrivalTime() const {
    vector<shared_ptr<FreightExtended>> sortedFreights = freights;
    sort(sortedFreights.begin(), sortedFreights.end(),
        [](const auto& a, const auto& b) {
            return a->getTime() < b->getTime();
        });

    cout << "\nScheduling Plan by Arrival Time:\n";
    cout << "================================\n";
    for (const auto& freight : sortedFreights) {
        cout << "Freight " << freight->getID()
            << " (" << FreightExtended::typeToString(freight->getType()) << ")"
            << " - Time: " << freight->getTime()
            << ", Destination: " << freight->getDest()
            << ", Load: " << freight->getCurrentLoad() << "/" << freight->getMaxCapacity() << "\n";

        for (const auto& cargoId : freight->getAssignedCargos()) {
            cout << "  - Cargo: " << cargoId << "\n";
        }
    }
}

void ScheduleList::displayByFreightCapacity() const {
    vector<shared_ptr<FreightExtended>> sortedFreights = freights;
    sort(sortedFreights.begin(), sortedFreights.end(),
        [](const auto& a, const auto& b) {
            return a->getMaxCapacity() > b->getMaxCapacity();
        });

    cout << "\nScheduling Plan by Freight Capacity:\n";
    cout << "===================================\n";
    for (const auto& freight : sortedFreights) {
        cout << "Freight " << freight->getID()
            << " (" << FreightExtended::typeToString(freight->getType()) << ")"
            << " - Capacity: " << freight->getMaxCapacity()
            << ", Load: " << freight->getCurrentLoad()
            << ", Destination: " << freight->getDest() << "\n";
    }
}

void ScheduleList::displayUnderutilizedFreights() const {
    cout << "\nUnderutilized Freights:\n";
    cout << "======================\n";
    for (const auto& freight : freights) {
        if (freight->getCurrentLoad() < freight->getMaxCapacity()) {
            cout << "Freight " << freight->getID()
                << " - Capacity: " << freight->getCurrentLoad() << "/" << freight->getMaxCapacity()
                << " (" << (freight->getMaxCapacity() - freight->getCurrentLoad()) << " slots available)\n";
        }
    }
}

void ScheduleList::displayUnassignedCargos() const {
    cout << "\nUnassigned Cargos:\n";
    cout << "=================\n";
    for (const auto& cargoId : unassignedCargos) {
        cout << "Cargo ID: " << cargoId << "\n";
    }
}

void ScheduleList::saveEnhancedSchedule(const string& filename) const {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file for writing: " << filename << "\n";
        return;
    }

    file << "Freight ID | Type | Capacity | Destination | Time | Assigned Cargos\n";
    file << "------------------------------------------------------------------\n";

    for (const auto& freight : freights) {
        file << freight->getID() << " | "
            << FreightExtended::typeToString(freight->getType()) << " | "
            << freight->getCurrentLoad() << "/" << freight->getMaxCapacity() << " | "
            << freight->getDest() << " | "
            << freight->getTime() << " | ";

        for (const auto& cargoId : freight->getAssignedCargos()) {
            file << cargoId << " ";
        }
        file << "\n";
    }

    if (!unassignedCargos.empty()) {
        file << "\nUnassigned Cargos:\n";
        for (const auto& cargoId : unassignedCargos) {
            file << cargoId << "\n";
        }
    }
}

void ScheduleList::matchFreightAndCargo(FStorage& fStorage, CStorage& cStorage) {
    matches.clear();
    for (const auto& freight : freights) {
        if (const Cargo* cargo = cStorage.getByTimeAndDest(freight->getTime(), freight->getDest())) {
            matches.push_back(Match(*freight, *cargo));
        }
    }
}

const vector<Match>& ScheduleList::getMatches() const {
    return matches;
}

void ScheduleList::printAll() const {
    cout << "Schedule List (All):\n";
    cout << "=======================\n";
    for (const auto& freight : freights) {
        cout << "Freight ID: " << freight->getID()
            << ", Type: " << FreightExtended::typeToString(freight->getType())
            << ", Time: " << freight->getTime()
            << ", Destination: " << freight->getDest()
            << ", Load: " << freight->getCurrentLoad() << "/" << freight->getMaxCapacity() << "\n";
        cout << "Assigned Cargos: ";
        for (const auto& cargoId : freight->getAssignedCargos()) {
            cout << cargoId << " ";
        }
        cout << "\n";
    }
    if (!unassignedCargos.empty()) {
        cout << "\nUnassigned Cargos:\n";
        for (const auto& cargoId : unassignedCargos) {
            cout << cargoId << "\n";
        }
    }
    if (!matches.empty()) {
        cout << "\nOriginal Matches (from matchFreightAndCargo):\n";
        for (const auto& m : matches) {
            cout << m.freight.getID() << " | "
                << m.cargo.getID() << " | "
                << m.freight.getTime() << " | "
                << m.freight.getDest() << "\n";
        }
    }
}

void ScheduleList::splitAndAssignGroup(const CargoGroup& group) {
    for (const auto& cargo : group.getCargos()) {
        bool assigned = false;
        for (auto& freight : freights) {
            if (canAssignToFreight(*freight, cargo)) {
                freight->assignCargo(cargo.getID());
                assigned = true;
                break;
            }
        }
        if (!assigned) {
            unassignedCargos.push_back(cargo.getID());
        }
    }
}

bool ScheduleList::canAssignToFreight(const FreightExtended& freight, const Cargo& cargo) const {
    return freight.canAcceptMore() &&
        freight.getDest() == cargo.getDest() &&
        abs(freight.getTime() - cargo.getTime()) <= 15; // Within 15-minute window
}