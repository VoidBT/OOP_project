#include "ScheduleList.h"
#include <algorithm>

#include <iostream>

using namespace std;

void ScheduleList::addScheduledEntry(const Freight& freight, const Cargo& cargo) {
	scheduled.emplace_back(freight, cargo);
}

vector<string> ScheduleList::getScheduled() {
    vector<string> schdata;
    for (auto match : scheduled) {
        string data = "Freight ID: " + match.freight.getID() +
            ", Cargo ID: " + match.cargo.getID() +
            ", Freight Time: " + to_string(match.freight.getTime()) +
            ", Cargo Time: " + to_string(match.cargo.getTime()) +
            ", Destination: " + match.freight.getDest();

        schdata.push_back(data);
    }
    return schdata;
}

void ScheduleList::printAll() const {
    cout << "\nMatch (All Data):\n";
    cout << "===========================\n";

    cout << "\n--- Freights in Match ---\n";
    if (getFreights().empty()) {
        cout << "No freights added to match.\n";
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

    cout << "\n--- Cargo Groups in Match ---\n";
    if (getCargos().empty()) {
        cout << "No cargo groups added.\n";
    }
    else {
        vector<Cargo> sortedGroups = getCargos();
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

    cout << "\n--- Basic Matches ---\n";
    if (scheduled.empty()) {
        cout << "No basic matches generated.\n";
    }
    else {
        for (const auto& match : scheduled) {
            cout << "Freight: " << match.freight.getID()
                << ", Cargo: " << match.cargo.getID()
                << ", Time: " << match.freight.getTime()
                << ", Destination: " << match.freight.getDest() << "\n";
        }
    }
    cout << "===========================\n";
}
