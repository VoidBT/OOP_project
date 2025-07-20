#include "ScheduleList.h"
#include <algorithm> // For std::sort, std::remove
#include <fstream>   // For file operations
#include <iomanip>   // For output formatting

// Add back 'using namespace std;' here
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
        // Need a modifiable copy of cargos for sorting if the original vector is const
        // If getCargos() returns a const reference, you can't sort it directly unless you make a copy.
        // Or change getCargos() to return by value or non-const reference if this sort is intended to modify the group's internal order.
        // For now, assuming cargos is a local copy for sorting purposes.
        vector<Cargo> cargos = group.getCargos(); // Make a copy to sort
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
                    // Break once assigned to the first suitable freight
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
                freight->getCurrentLoad() + group.getSize() <= freight->getMaxCapacity() &&
                freight->getDest() == group.getDestination()) { // Add destination check
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

        // Display assigned cargos for this freight as well, for completeness
        for (const auto& cargoId : freight->getAssignedCargos()) {
            cout << "  - Cargo: " << cargoId << "\n";
        }
    }
}

void ScheduleList::displayUnderutilizedFreights() const {
    cout << "\nUnderutilized Freights:\n";
    cout << "======================\n";
    for (const auto& freight : freights) {
        if (freight->getCurrentLoad() < freight->getMaxCapacity()) {
            cout << "Freight " << freight->getID()
                << " - Capacity: " << freight->getCurrentLoad() << "/" << freight->getMaxCapacity()
                << " (" << (freight->getMaxCapacity() - freight->getCurrentLoad()) << " slots available)"
                << ", Destination: " << freight->getDest()
                << ", Time: " << freight->getTime() << "\n";
        }
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

    file << "Freight ID | Type | Capacity | Destination | Time | Assigned Cargos\n";
    file << "------------------------------------------------------------------\n";

    for (const auto& freight : freights) {
        file << setw(10) << left << freight->getID() << " | "
            << setw(12) << left << FreightExtended::typeToString(freight->getType()) << " | "
            << setw(8) << left << to_string(freight->getCurrentLoad()) + "/" + to_string(freight->getMaxCapacity()) << " | "
            << setw(12) << left << freight->getDest() << " | "
            << setw(5) << left << freight->getTime() << " | ";

        bool firstCargo = true;
        for (const auto& cargoId : freight->getAssignedCargos()) {
            if (!firstCargo) {
                file << ", "; // Separate cargo IDs with a comma
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

void ScheduleList::matchFreightAndCargo(FStorage& fStorage, CStorage& cStorage) {
    matches.clear();
    // This part of the logic needs to be revisited.
    // The previous implementation was matching based on direct Freight object in fStorage
    // and Cargo in cStorage. However, schedule.freights now holds FreightExtended.
    // This function will primarily match FreightExtended in `this->freights`
    // with Cargos in `cStorage` based on time and destination.
    // It's a "basic" match, not leveraging the advanced scheduling.

    for (const auto& freightExt : freights) {
        // Iterate through all cargos in cStorage to find a match
        const vector<Cargo>& allCargos = cStorage.getAllCargos();
        for (const auto& cargo : allCargos) {
            if (freightExt->getDest() == cargo.getDest() &&
                abs(freightExt->getTime() - cargo.getTime()) <= 15) { // Within 15-minute window
                // Found a match, create a Match object
                // Note: The Match constructor takes Freight and Cargo.
                // freightExt is a shared_ptr<FreightExtended>, which is a Freight.
                matches.push_back(Match(*freightExt, cargo));
                // Assuming one cargo can be matched per freight in this basic matching
                // or you might want to find the "best" match or all possible matches.
                // For simplicity, we add the first valid match.
                break; // Move to the next freight
            }
        }
    }
    cout << "Basic freight and cargo matching completed. " << matches.size() << " matches found.\n";
}

const vector<Match>& ScheduleList::getMatches() const {
    return matches;
}

void ScheduleList::printAll() const {
    cout << "\nSchedule List (All):\n";
    cout << "=======================\n";
    if (freights.empty() && cargoGroups.empty() && unassignedCargos.empty() && matches.empty()) {
        cout << "No schedule data to display.\n";
        return;
    }

    cout << "\n--- Scheduled Freights ---\n";
    if (freights.empty()) {
        cout << "No freights added to schedule.\n";
    }
    else {
        for (const auto& freight : freights) {
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
                for (const auto& cargoId : freight->getAssignedCargos()) {
                    cout << cargoId << " ";
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
        for (const auto& group : cargoGroups) {
            cout << "Group ID: " << group.getGroupId()
                << ", Destination: " << group.getDestination()
                << ", Size: " << group.getSize() << "/" << group.getMaxSize() << "\n";
            cout << "  Cargos in group: ";
            if (group.getCargos().empty()) {
                cout << "None";
            }
            else {
                for (const auto& cargo : group.getCargos()) {
                    cout << cargo.getID() << " ";
                }
            }
            cout << "\n";
        }
    }

    if (!unassignedCargos.empty()) {
        cout << "\n--- Unassigned Cargos ---\n";
        for (const auto& cargoId : unassignedCargos) {
            cout << "Cargo ID: " << cargoId << "\n";
        }
    }
    else {
        cout << "\nNo unassigned cargos.\n";
    }


    if (!matches.empty()) {
        cout << "\n--- Original Matches (from matchFreightAndCargo) ---\n";
        for (const auto& m : matches) {
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
    // Check if freight can accept more, has same destination, and time is within a 15-minute window
    return freight.canAcceptMore() &&
        freight.getDest() == cargo.getDest() &&
        abs(freight.getTime() - cargo.getTime()) <= 15; // Within 15-minute window
}