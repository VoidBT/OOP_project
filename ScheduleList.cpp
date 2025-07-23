#include "ScheduleList.h"
#include <algorithm>
#include <fstream>
#include <iomanip> // For std::setw, std::left
#include <limits>
#include <iostream> // For std::cout, std::cerr

using namespace std;

void ScheduleList::addFreight(shared_ptr<FreightExtended> freight) {
    freights.push_back(freight);
}

void ScheduleList::addCargoGroup(const CargoGroup& group) {
    cargoGroups.push_back(group);
}

void ScheduleList::resetFreightAssignments() {
    for (auto& freight : freights) {
        freight->clearAssignedCargos();
    }
    unassignedCargos.clear();
}

// NEW: canAssignToFreight uses cargo.getSize() and freight.canAcceptMore(cargoSize)
bool ScheduleList::canAssignToFreight(const FreightExtended& freight, const Cargo& cargo) const {
    return freight.canAcceptMore(cargo.getSize()) && // Check if it can accept based on size
        freight.getDest() == cargo.getDest() &&
        freight.getTime() >= cargo.getTime() &&
        (freight.getTime() - cargo.getTime()) <= 15;
}

// NEW: assignCargoToBestFreight uses cargo.getSize() in the assignment
bool ScheduleList::assignCargoToBestFreight(const Cargo& cargo) {
    sort(freights.begin(), freights.end(),
        [](const auto& a, const auto& b) {
            return a->getTime() < b->getTime();
        });

    for (auto& freight : freights) {
        if (canAssignToFreight(*freight, cargo)) {
            freight->assignCargo(cargo.getID(), cargo.getSize()); // Pass cargo size
            return true;
        }
    }
    return false;
}

void ScheduleList::scheduleByArrivalTime() {
    resetFreightAssignments();

    sort(freights.begin(), freights.end(),
        [](const auto& a, const auto& b) {
            return a->getTime() < b->getTime();
        });

    vector<Cargo> allCargosFromGroups;
    for (const auto& group : cargoGroups) {
        const auto& groupCargos = group.getCargos();
        allCargosFromGroups.insert(allCargosFromGroups.end(), groupCargos.begin(), groupCargos.end());
    }

    sort(allCargosFromGroups.begin(), allCargosFromGroups.end(),
        [](const Cargo& a, const Cargo& b) {
            return a.getTime() < b.getTime();
        });

    for (const auto& cargo : allCargosFromGroups) {
        if (!assignCargoToBestFreight(cargo)) {
            unassignedCargos.push_back(cargo.getID());
        }
    }
    cout << "Scheduling by arrival time completed. " << unassignedCargos.size() << " cargos unassigned.\n";
}

void ScheduleList::scheduleByFreightCapacity() {
    resetFreightAssignments();

    // Sort freights by capacity (descending) to prioritize filling larger freights
    sort(freights.begin(), freights.end(),
        [](const auto& a, const auto& b) {
            return a->getMaxCapacity() > b->getMaxCapacity();
        });

    for (auto& group : cargoGroups) {
        vector<Cargo> currentGroupCargos = group.getCargos();

        bool groupAssignedWhole = false;
        // Calculate total size of the group
        int groupTotalSize = 0;
        for (const auto& cargo : currentGroupCargos) {
            groupTotalSize += cargo.getSize();
        }

        for (auto& freight : freights) {
            // Check if this freight can take the entire group's total size
            if (freight->canAcceptMore(groupTotalSize) && // Check with total group size
                freight->getDest() == group.getDestination()) {

                bool allCargosMatchTime = true;
                for (const auto& cargo : currentGroupCargos) {
                    if (!canAssignToFreight(*freight, cargo)) {
                        allCargosMatchTime = false;
                        break;
                    }
                }

                if (allCargosMatchTime) {
                    for (const auto& cargo : currentGroupCargos) {
                        freight->assignCargo(cargo.getID(), cargo.getSize()); // Pass cargo size
                    }
                    groupAssignedWhole = true;
                    break;
                }
            }
        }

        if (!groupAssignedWhole) {
            sort(currentGroupCargos.begin(), currentGroupCargos.end(),
                [](const Cargo& a, const Cargo& b) {
                    return a.getTime() < b.getTime();
                });

            for (const auto& cargo : currentGroupCargos) {
                // When assigning individual cargos in capacity mode, prioritize filling existing freights
                // by remaining capacity.
                sort(freights.begin(), freights.end(),
                    [](const auto& a, const auto& b) {
                        // Prioritize freights with more available space for large cargos,
                        // but also prefer filling up existing freights (less remaining capacity is better)
                        int a_remaining = a->getMaxCapacity() - a->getCurrentLoadSize();
                        int b_remaining = b->getMaxCapacity() - b->getCurrentLoadSize();
                        // Sort by least remaining capacity first (most full)
                        return a_remaining < b_remaining;
                    });

                bool assigned = false;
                for (auto& freight : freights) {
                    if (canAssignToFreight(*freight, cargo)) {
                        freight->assignCargo(cargo.getID(), cargo.getSize()); // Pass cargo size
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
    cout << "Scheduling by freight capacity completed. " << unassignedCargos.size() << " cargos unassigned.\n";
}


void ScheduleList::displayByArrivalTime() const {
    vector<shared_ptr<FreightExtended>> sortedFreights = freights;
    sort(sortedFreights.begin(), sortedFreights.end(),
        [](const auto& a, const auto& b) {
            return a->getTime() < b->getTime();
        });

    cout << "\nScheduling Plan Sorted by Freight Arrival Time:\n";
    cout << "===============================================\n";
    for (const auto& freight : sortedFreights) {
        cout << "Freight " << freight->getID()
            << " (" << FreightExtended::typeToString(freight->getType()) << ")"
            << " - Time: " << freight->getTime()
            << ", Destination: " << freight->getDest()
            << ", Load: " << freight->getCurrentLoadSize() << "/" << freight->getMaxCapacity() << " (size)\n"; // Display size load

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
    // Sort freights by capacity (descending), then by current load size (descending) for better utilization view
    sort(sortedFreights.begin(), sortedFreights.end(),
        [](const auto& a, const auto& b) {
            if (a->getMaxCapacity() != b->getMaxCapacity()) {
                return a->getMaxCapacity() > b->getMaxCapacity();
            }
            return a->getCurrentLoadSize() > b->getCurrentLoadSize(); // More loaded by size first
        });

    cout << "\nScheduling Plan Sorted by Freight Capacity (Most Used First):\n";
    cout << "============================================================\n";
    for (const auto& freight : sortedFreights) {
        cout << "Freight " << freight->getID()
            << " (" << FreightExtended::typeToString(freight->getType()) << ")"
            << " - Capacity: " << freight->getMaxCapacity()
            << ", Current Load: " << freight->getCurrentLoadSize() << " (size)" // Display size load
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
    cout << "\nUnderutilized Freights (Not at Full Capacity by Size):\n"; // Updated description
    cout << "=============================================\n";
    bool found = false;
    for (const auto& freight : freights) {
        if (freight->getCurrentLoadSize() < freight->getMaxCapacity()) { // Check based on size
            cout << "Freight " << freight->getID()
                << " - Type: " << FreightExtended::typeToString(freight->getType())
                << ", Current Load: " << freight->getCurrentLoadSize()
                << ", Max Capacity: " << freight->getMaxCapacity()
                << " (" << (freight->getMaxCapacity() - freight->getCurrentLoadSize()) << " size units available)" // Units available
                << ", Destination: " << freight->getDest()
                << ", Time: " << freight->getTime() << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << "All freights are at full capacity by size or no freights available.\n"; // Updated message
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

Match ScheduleList::matchFreightAndCargo(FStorage& freightStorage, CStorage& cargoStorage) {
    // Ensure freights and cargos exist
    if (freightStorage.getAllFreights().empty() || cargoStorage.getAllCargos().empty()) {
        cout << "Cannot perform basic matching: No freights or cargos available in storage.\n";
        return Match(Freight("", 0, ""), Cargo("", 0, "", 0));
    }

    shared_ptr<FreightExtended> firstFreightExtended = freightStorage.getAllFreights()[0];
    Freight firstFreight(firstFreightExtended->getID(), firstFreightExtended->getTime(), firstFreightExtended->getDest());
    Cargo firstCargo = cargoStorage.getAllCargos()[0]; 
    
    if (firstFreight.getDest() == firstCargo.getDest() && firstFreight.getTime() >= firstCargo.getTime()) {
        Match newMatch(firstFreight, firstCargo);
        matches.push_back(newMatch);
        cout << "Basic match found and added: Freight " << firstFreight.getID()
            << " with Cargo " << firstCargo.getID() << "\n";
        return newMatch;
    }
    else {
        cout << "No basic match found based on simple criteria.\n";
        return Match(Freight("", 0, ""), Cargo("", 0, "", 0)); // Return a dummy match
    }
}

const std::vector<Match>& ScheduleList::getMatches() const {
    return matches;
}


void ScheduleList::saveEnhancedSchedule(const string& filename) const {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file for writing: " << filename << "\n";
        return;
    }

    file << "Freight ID | Type         | Load/Cap (Size) | Destination | Time | Assigned Cargos\n"; // Updated header
    file << "--------------------------------------------------------------------------------\n";

    vector<shared_ptr<FreightExtended>> sortedFreights = freights;
    sort(sortedFreights.begin(), sortedFreights.end(),
        [](const auto& a, const auto& b) {
            return a->getID() < b->getID();
        });


    for (const auto& freight : sortedFreights) {
        file << std::setw(10) << std::left << freight->getID() << " | " // Use std::setw
            << std::setw(12) << std::left << FreightExtended::typeToString(freight->getType()) << " | " // Use std::setw
            << std::setw(15) << std::left << to_string(freight->getCurrentLoadSize()) + "/" + to_string(freight->getMaxCapacity()) << " | " // Display size load
            << std::setw(12) << std::left << freight->getDest() << " | "
            << std::setw(5) << std::left << freight->getTime() << " | ";

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
    cout << "\nSchedule List (All Data):\n";
    cout << "===========================\n";

    cout << "\n--- Freights in Schedule ---\n";
    if (freights.empty()) {
        cout << "No freights added to schedule.\n";
    }
    else {
        vector<shared_ptr<FreightExtended>> sortedFreights = freights;
        sort(sortedFreights.begin(), sortedFreights.end(),
            [](const auto& a, const auto& b) {
                return a->getID() < b->getID();
            });

        for (const auto& freight : sortedFreights) {
            cout << "Freight ID: " << freight->getID()
                << ", Type: " << FreightExtended::typeToString(freight->getType())
                << ", Time: " << freight->getTime()
                << ", Destination: " << freight->getDest()
                << ", Load: " << freight->getCurrentLoadSize() << "/" << freight->getMaxCapacity() << " (size)\n"; // Display size load
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

    cout << "\n--- Cargo Groups in Schedule ---\n";
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
                << ", Time Window: " << group.getTimeWindow() << "\n"; // Added time window
            cout << "  Cargos in Group: ";
            if (group.getCargos().empty()) {
                cout << "None";
            }
            else {
                bool first = true;
                for (const auto& cargo : group.getCargos()) {
                    if (!first) cout << ", ";
                    cout << cargo.getID() << " (Size: " << cargo.getSize() << ")"; // Display cargo size in group
                    first = false;
                }
            }
            cout << "\n";
        }
    }

    cout << "\n--- Unassigned Cargos ---\n";
    if (unassignedCargos.empty()) {
        cout << "All cargos have been assigned or grouped.\n";
    }
    else {
        for (const auto& cargoId : unassignedCargos) {
            cout << "- " << cargoId << "\n";
        }
    }

    cout << "\n--- Basic Matches ---\n";
    if (matches.empty()) {
        cout << "No basic matches generated.\n";
    }
    else {
        for (const auto& match : matches) {
            cout << "Freight: " << match.freight.getID() << ", Cargo: " << match.cargo.getID()
                << ", Time: " << match.freight.getTime() << ", Destination: " << match.freight.getDest() << "\n";
        }
    }
    cout << "===========================\n";
}