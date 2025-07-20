#include "ScheduleList.h"
#include <algorithm> // For std::sort, std::remove, std::copy_if
#include <fstream>   // For file operations
#include <iomanip>   // For output formatting
#include <limits>    // For std::numeric_limits

using namespace std;

void ScheduleList::addFreight(shared_ptr<FreightExtended> freight) {
    freights.push_back(freight);
}

void ScheduleList::addCargoGroup(const CargoGroup& group) {
    cargoGroups.push_back(group);
}

// Helper to reset freight assignments and unassigned cargos before a new scheduling run
void ScheduleList::resetFreightAssignments() {
    for (auto& freight : freights) {
        freight->clearAssignedCargos(); // Assuming you add this method to FreightExtended
    }
    unassignedCargos.clear();
}

// Refined `canAssignToFreight` based on Requirement 4
bool ScheduleList::canAssignToFreight(const FreightExtended& freight, const Cargo& cargo) const {
    // 1. Freight must be able to accept more cargo
    // 2. Destinations must match
    // 3. Arrival timing requirement:
    //    freight.time must be >= cargo.time AND
    //    freight.time - cargo.time <= 15
    return freight.canAcceptMore() &&
        freight.getDest() == cargo.getDest() &&
        freight.getTime() >= cargo.getTime() &&
        (freight.getTime() - cargo.getTime()) <= 15;
}

// Helper to assign a single cargo to the best available freight (earliest time first)
bool ScheduleList::assignCargoToBestFreight(const Cargo& cargo) {
    // Sort freights by their time to find the earliest suitable one (Requirement 5 implication)
    // This ensures that even if you're assigning individually, you're trying to meet time requirements.
    sort(freights.begin(), freights.end(),
        [](const auto& a, const auto& b) {
            return a->getTime() < b->getTime();
        });

    for (auto& freight : freights) {
        if (canAssignToFreight(*freight, cargo)) {
            freight->assignCargo(cargo.getID());
            return true;
        }
    }
    return false; // Could not assign this cargo
}


void ScheduleList::scheduleByArrivalTime() {
    resetFreightAssignments(); // Clear previous assignments before new scheduling

    // Sort freights by their time (relevant for matching cargos to earliest freight)
    sort(freights.begin(), freights.end(),
        [](const auto& a, const auto& b) {
            return a->getTime() < b->getTime();
        });

    // Process cargos from groups, prioritizing by their individual arrival times (Requirement 5)
    // We need to collect all individual cargos from all groups first to sort them globally.
    vector<Cargo> allCargosFromGroups;
    for (const auto& group : cargoGroups) {
        // Copy cargos from the group to the temporary vector
        // This ensures we're working with a modifiable list of individual cargos
        // It also handles Requirement 3's implicit need to treat individual cargos
        // when splitting a group.
        const auto& groupCargos = group.getCargos();
        allCargosFromGroups.insert(allCargosFromGroups.end(), groupCargos.begin(), groupCargos.end());
    }

    // Sort all individual cargos by their time (Requirement 5)
    sort(allCargosFromGroups.begin(), allCargosFromGroups.end(),
        [](const Cargo& a, const Cargo& b) {
            return a.getTime() < b.getTime();
        });

    // Attempt to assign each sorted cargo
    for (const auto& cargo : allCargosFromGroups) {
        if (!assignCargoToBestFreight(cargo)) { // Uses the helper with time-sorted freights
            unassignedCargos.push_back(cargo.getID());
        }
    }
    cout << "Scheduling by arrival time completed. " << unassignedCargos.size() << " cargos unassigned.\n";
}


void ScheduleList::scheduleByFreightCapacity() {
    resetFreightAssignments(); // Clear previous assignments before new scheduling

    // Sort freights by capacity (descending) to prioritize filling larger freights (Requirement 6)
    sort(freights.begin(), freights.end(),
        [](const auto& a, const auto& b) {
            return a->getMaxCapacity() > b->getMaxCapacity();
        });

    // Iterate through cargo groups
    for (auto& group : cargoGroups) {
        // Collect cargos from the current group
        vector<Cargo> currentGroupCargos = group.getCargos(); // Make a copy

        // Try to assign the whole group to the first suitable freight that can take it
        // This attempts to keep groups together if possible, aligning with Req 3's "grouped by destination"
        bool groupAssignedWhole = false;
        for (auto& freight : freights) {
            // Check if this freight can take all cargos in the current group
            if (freight->canAcceptMore() &&
                freight->getCurrentLoad() + currentGroupCargos.size() <= freight->getMaxCapacity() &&
                freight->getDest() == group.getDestination()) { // Destination must match group's destination

                bool allCargosMatchTime = true;
                for (const auto& cargo : currentGroupCargos) {
                    if (!canAssignToFreight(*freight, cargo)) { // Check time for ALL cargos in group
                        allCargosMatchTime = false;
                        break;
                    }
                }

                if (allCargosMatchTime) {
                    for (const auto& cargo : currentGroupCargos) {
                        freight->assignCargo(cargo.getID());
                    }
                    groupAssignedWhole = true;
                    break; // Group assigned, move to next cargo group
                }
            }
        }

        if (!groupAssignedWhole) {
            // If the whole group cannot be assigned, split and assign individual cargos (Requirement 3)
            // Sort cargos within the group by time (optional, but can help with time-based assignment if splitting)
            sort(currentGroupCargos.begin(), currentGroupCargos.end(),
                [](const Cargo& a, const Cargo& b) {
                    return a.getTime() < b.getTime();
                });

            for (const auto& cargo : currentGroupCargos) {
                // Find the best freight for this individual cargo based on capacity preference first, then time.
                // Re-sort freights by available capacity to find the best fit for this individual cargo.
                // This is a subtle point: for *individual* cargos, you still want to fill up freights.
                sort(freights.begin(), freights.end(),
                    [](const auto& a, const auto& b) {
                        // Prioritize freights that can take more, then by current load (more full first)
                        if (a->getMaxCapacity() != b->getMaxCapacity()) {
                            return a->getMaxCapacity() > b->getMaxCapacity();
                        }
                        return a->getCurrentLoad() > b->getCurrentLoad();
                    });

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
    cout << "Scheduling by freight capacity completed. " << unassignedCargos.size() << " cargos unassigned.\n";
}


void ScheduleList::displayByArrivalTime() const {
    vector<shared_ptr<FreightExtended>> sortedFreights = freights;
    // Sort freights by their own time for display
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
            << ", Load: " << freight->getCurrentLoad() << "/" << freight->getMaxCapacity() << "\n";

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
    // Sort freights by capacity (descending), then by current load (descending) for better utilization view
    sort(sortedFreights.begin(), sortedFreights.end(),
        [](const auto& a, const auto& b) {
            if (a->getMaxCapacity() != b->getMaxCapacity()) {
                return a->getMaxCapacity() > b->getMaxCapacity();
            }
            return a->getCurrentLoad() > b->getCurrentLoad(); // More loaded first
        });

    cout << "\nScheduling Plan Sorted by Freight Capacity (Most Used First):\n";
    cout << "============================================================\n";
    for (const auto& freight : sortedFreights) {
        cout << "Freight " << freight->getID()
            << " (" << FreightExtended::typeToString(freight->getType()) << ")"
            << " - Capacity: " << freight->getMaxCapacity()
            << ", Load: " << freight->getCurrentLoad()
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
    cout << "\nUnderutilized Freights (Not at Full Capacity):\n";
    cout << "=============================================\n";
    bool found = false;
    for (const auto& freight : freights) {
        if (freight->getCurrentLoad() < freight->getMaxCapacity()) {
            cout << "Freight " << freight->getID()
                << " - Type: " << FreightExtended::typeToString(freight->getType())
                << ", Current Load: " << freight->getCurrentLoad()
                << ", Max Capacity: " << freight->getMaxCapacity()
                << " (" << (freight->getMaxCapacity() - freight->getCurrentLoad()) << " slots available)"
                << ", Destination: " << freight->getDest()
                << ", Time: " << freight->getTime() << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << "All freights are at full capacity or no freights available.\n";
    }
}

void ScheduleList::displayUnassignedCargos() const {
    cout << "\nUnassigned Cargos:\n";
    cout << "=================\n";
    if (unassignedCargos.empty()) {
        cout << "No unassigned cargos.\n";
    }
    else {
        for (const auto& cargoId : unassignedCargos) {
            cout << "Cargo ID: " << cargoId << "\n";
        }
    }
}

void ScheduleList::saveEnhancedSchedule(const string& filename) const {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file for writing: " << filename << "\n";
        return;
    }

    file << "Freight ID | Type         | Load/Cap | Destination | Time | Assigned Cargos\n";
    file << "--------------------------------------------------------------------------------\n";

    // Sort freights by ID for consistent output
    vector<shared_ptr<FreightExtended>> sortedFreights = freights;
    sort(sortedFreights.begin(), sortedFreights.end(),
        [](const auto& a, const auto& b) {
            return a->getID() < b->getID();
        });


    for (const auto& freight : sortedFreights) {
        file << setw(10) << left << freight->getID() << " | "
            << setw(12) << left << FreightExtended::typeToString(freight->getType()) << " | "
            << setw(8) << left << to_string(freight->getCurrentLoad()) + "/" + to_string(freight->getMaxCapacity()) << " | "
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

// This function remains for its original purpose of finding "basic" matches
// It does NOT assign cargos to freights' internal assignedCargos list.
void ScheduleList::matchFreightAndCargo(FStorage& fStorage, CStorage& cStorage) {
    matches.clear();
    // This function can be simplified if its role is just to find all potential basic matches.
    // It should ideally use Freight objects from FStorage and Cargo objects from CStorage directly,
    // not FreightExtended objects from the internal 'freights' list, unless its purpose is
    // to match what's already in the schedule.

    // Let's assume its purpose is to find matches between all available FStorage freights
    // and CStorage cargos, for an initial report.
    const vector<Freight>& allBasicFreights = fStorage.getAllFreights();
    const vector<Cargo>& allCargos = cStorage.getAllCargos();

    for (const auto& freight : allBasicFreights) { // Use basic Freight objects here
        for (const auto& cargo : allCargos) {
            // Match based on destination and the 15-minute time window
            if (freight.getDest() == cargo.getDest() &&
                freight.getTime() >= cargo.getTime() && // Freight cannot depart before cargo arrives
                (freight.getTime() - cargo.getTime()) <= 15) { // And not too early
                matches.push_back(Match(freight, cargo));
            }
        }
    }
    cout << "Basic freight and cargo matching completed. " << matches.size() << " matches found.\n";
}

const vector<Match>& ScheduleList::getMatches() const {
    return matches;
}

void ScheduleList::printAll() const {
    cout << "\nSchedule List (All Data):\n";
    cout << "===========================\n";

    cout << "\n--- Freights in Schedule ---\n";
    if (freights.empty()) {
        cout << "No freights added to schedule.\n";
    }
    else {
        // Sort freights by ID for consistent output
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
                << ", Load: " << freight->getCurrentLoad() << "/" << freight->getMaxCapacity() << "\n";
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

    cout << "\n--- Cargo Groups ---\n";
    if (cargoGroups.empty()) {
        cout << "No cargo groups added.\n";
    }
    else {
        // Sort cargo groups by ID for consistent output
        vector<CargoGroup> sortedGroups = cargoGroups;
        sort(sortedGroups.begin(), sortedGroups.end(),
            [](const auto& a, const auto& b) {
                return a.getGroupId() < b.getGroupId();
            });

        for (const auto& group : sortedGroups) {
            cout << "Group ID: " << group.getGroupId()
                << ", Destination: " << group.getDestination()
                << ", Size: " << group.getSize() << "/" << group.getMaxSize() << "\n";
            cout << "  Cargos in group: ";
            if (group.getCargos().empty()) {
                cout << "None";
            }
            else {
                bool first = true;
                for (const auto& cargo : group.getCargos()) {
                    if (!first) cout << ", ";
                    cout << cargo.getID();
                    first = false;
                }
            }
            cout << "\n";
        }
    }

    if (!unassignedCargos.empty()) {
        cout << "\n--- Unassigned Cargos ---\n";
        // Sort unassigned cargos for consistent output
        vector<string> sortedUnassigned = unassignedCargos;
        sort(sortedUnassigned.begin(), sortedUnassigned.end());
        for (const auto& cargoId : sortedUnassigned) {
            cout << "Cargo ID: " << cargoId << "\n";
        }
    }
    else {
        cout << "\nNo unassigned cargos.\n";
    }

    if (!matches.empty()) {
        cout << "\n--- Basic Matches (from matchFreightAndCargo) ---\n";
        // Sort matches for consistent output
        vector<Match> sortedMatches = matches;
        sort(sortedMatches.begin(), sortedMatches.end(),
            [](const Match& a, const Match& b) {
                if (a.freight.getID() != b.freight.getID()) {
                    return a.freight.getID() < b.freight.getID();
                }
                return a.cargo.getID() < b.cargo.getID();
            });

        for (const auto& m : sortedMatches) {
            cout << "Freight: " << m.freight.getID() << " | "
                << "Cargo: " << m.cargo.getID() << " | "
                << "F Time: " << m.freight.getTime() << " | "
                << "F Dest: " << m.freight.getDest() << "\n";
        }
    }
    else {
        cout << "\nNo basic matches found.\n";
    }
}