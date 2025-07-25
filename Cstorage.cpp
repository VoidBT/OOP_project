#include "CStorage.h"
#include <algorithm> // For std::remove_if
#include <iostream>
#include <iomanip>

//Done by Brendan Tjung Yew Wang 2401765

using namespace std;

// CStorage class implementation
void CStorage::addCargo(const Cargo& cargo) {
    cargos.push_back(cargo);
}

// Edit cargo by ID, updating time and destination
void CStorage::editCargo(const string& id, int newTime, const string& newDest) {
    Cargo* cargo = findCargo(id);
    if (cargo) {
        cargo->setTime(newTime);
        cargo->setDest(newDest);
    }
    else {
        cout << "Cargo with ID " << id << " not found.\n";
    }
}

// Delete cargo by ID
void CStorage::deleteCargo(const string& id) {
    auto it = remove_if(cargos.begin(), cargos.end(),
        [&id](const Cargo& c) { return c.getID() == id; });
    if (it != cargos.end()) {
        cargos.erase(it, cargos.end());
    }
    else {
        cout << "Cargo with ID " << id << " not found.\n";
    }
}

// Get all cargos
const vector<Cargo>& CStorage::getAllCargos() const {
    return cargos;
}

// Print all cargos in storage
void CStorage::printAll() const {
    cout << "\n--- All Cargos ---\n";
    if (cargos.empty()) {
        cout << "No cargos available.\n";
        return;
    }
    for (const auto& cargo : cargos) {
        cout << "ID: " << cargo.getID()
            << ", Time: " << setw(4) << setfill('0') << cargo.getTime().getRawTime()
            << ", Destination: " << cargo.getDest()
            << ", Size: " << cargo.getSize() << "\n";
    }
	cout << setfill(' '); // Reset fill character after setw
}

// Print all cargo groups
void CStorage::printAllGroups() const {
    cout << "\n--- All Cargo Groups ---\n";
    if (groups.empty()) {
        cout << "No cargo groups available.\n";
        return;
    }
    for (const auto& group : groups) {
        cout << "Group ID: " << group.getGroupId()
            << ", Destination: " << group.getDestination()
            << ", Size: " << group.getGrpSize() << "/" << group.getMaxSize()
            << ", Time Window: " << group.getTimeWindow().getRawTime() << "\n";
        cout << "Cargos in Group:\n";
        for (const auto& cargo : group.getCargos()) {
            cout << "  - ID: " << cargo.getID()
                << ", Time: " << setw(4) << setfill('0') << cargo.getTime().getRawTime()
                << ", Destination: " << cargo.getDest()
                << ", Size: " << cargo.getSize() << "\n";
        }
    }
    cout << setfill(' '); // Reset fill character after setw
}

// Find cargo by ID
Cargo* CStorage::findCargo(const string& id) {
    for (auto& cargo : cargos) {
        if (cargo.getID() == id) {
            return &cargo;
        }
    }
    return nullptr;
}

// Create cargo groups based on destination and time window
void CStorage::CreateGroups() {

	vector<Cargo> sortedCargos = cargos;
    sort(sortedCargos.begin(), sortedCargos.end(),
        [](const Cargo& a, const Cargo& b) {
            return a.getTime().getRawTime() < b.getTime().getRawTime();
        });

	groups.clear(); // Clear existing groups before creating new ones

	//Group cargo by destination and time window
	vector<Cargo> uniqueDestinations;
    for (auto i : sortedCargos)
    {
        bool Check = false;
        for (auto u : uniqueDestinations)
        {
            if (i.getDest() == u.getDest() && i.getTime().isWithinLimit(u.getTime()) )
            {
                Check = true;
                break;
            }
		}
        if (!Check)
        {
            uniqueDestinations.push_back(i);
        }
	}

	int groupId = 0;

    for (auto i : uniqueDestinations)
    {
		string dest = i.getDest();
		int time = i.getTime().getRawTime();
        bool notdone = true, keep_adding = false;
        while (notdone) {
            CargoGroup group(groupId++, dest, time);
            for (auto p : cargos)
            {
                if (i.getDest() == p.getDest() && i.getTime().isWithinLimit(p.getTime()) ) {
                    if (!group.addCargo(p) && group.getSize() == group.getMaxSize()) {// If we couldn't add cargo bcs it is full
                        // If group is full, create a new cargoGroup with these params
                        keep_adding = true;
						dest = p.getDest();
						time = p.getTime().getRawTime();
                        break;
                    }
                }
            }
            if(!keep_adding) notdone = false;
                groups.push_back(group);
        }

    }
}

// Get all cargo groups
vector<CargoGroup> CStorage::getCargoGroups() const {
    return groups;
}

// Clear all cargos and groups
void CStorage::clear() {
    for (auto& cargo : groups) {
        cargo.clear();
	}
    cargos.clear();
    groups.clear();
}