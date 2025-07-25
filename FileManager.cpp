#include "FileManager.h"
#include <sstream>
#include <algorithm> // For std::sort
#include <iomanip>   // For setw, left
#include <iostream>

using namespace std;

// Load cargos from file into CStorage
void FileManager::loadCargos(const string& filename, CStorage& cargoStorage) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open cargo file " << filename << "\n";
        return;
    }

    cargoStorage.clear(); // Clear existing cargos
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, dest, timeStr, sizeStr;
        int time, size;

        if (getline(ss, id, ',') &&
            getline(ss, timeStr, ',') &&
            getline(ss, dest, ',') &&
            getline(ss, sizeStr)) {
            try {
                time = stoi(timeStr);
                size = stoi(sizeStr);
                cargoStorage.addCargo(Cargo(id, time, dest, size));
            }
            catch (const invalid_argument& e) {
                cerr << "Skipping malformed cargo line: " << line << " (" << e.what() << ")\n";
            }
            catch (const out_of_range& e) {
                cerr << "Skipping cargo line with out-of-range number: " << line << " (" << e.what() << ")\n";
            }
        }
        else {
            cerr << "Skipping malformed cargo line (missing fields): " << line << "\n";
        }
    }
    file.close();
    cout << "Cargos loaded from " << filename << "\n";
}

// Save cargos vector to file
void FileManager::saveCargos(const string& filename, const vector<Cargo>& cargos) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open cargo file " << filename << " for writing\n";
        return;
    }
    for (const auto& cargo : cargos) {
        file << cargo.getID() << "," << cargo.getTime().getRawTime() << "," << cargo.getDest() << "," << cargo.getSize() << "\n";
    }
    file.close();
    cout << "Cargos saved to " << filename << "\n";
}

// Load freights from file into FStorage
void FileManager::loadFreights(const string& filename, FStorage& freightStorage) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open freight file " << filename << "\n";
        return;
    }

    freightStorage.clear(); // Clear existing freights
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, dest, timeStr, typeStr;
        int time;
        FreightType type;

        if (getline(ss, id, ',') &&
            getline(ss, timeStr, ',') &&
            getline(ss, dest, ',') &&
            getline(ss, typeStr)) {
            try {
                time = stoi(timeStr);
                type = FreightExtended::stringToType(typeStr);

                freightStorage.addFreight(make_shared<FreightExtended>(id, time, dest, type));
            }
            catch (const invalid_argument& e) {
                cerr << "Skipping malformed freight line: " << line << " (" << e.what() << ")\n";
            }
            catch (const out_of_range& e) {
                cerr << "Skipping freight line with out-of-range number: " << line << " (" << e.what() << ")\n";
            }
            catch (const runtime_error& e) { // For invalid FreightType string
                cerr << "Skipping freight line with invalid type: " << line << " (" << e.what() << ")\n";
            }
        }
        else {
            cerr << "Skipping malformed freight line (missing fields): " << line << "\n";
        }
    }
    file.close();
    cout << "Freights loaded from " << filename << "\n";
}

// Save freights vector (shared_ptr) to file
void FileManager::saveFreights(const string& filename, const vector<shared_ptr<FreightExtended>>& freights) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open freight file " << filename << " for writing\n";
        return;
    }
    for (const auto& freight : freights) {
        file << freight->getID() << "," << freight->getTime().getRawTime() << "," << freight->getDest() << ","
            << FreightExtended::typeToString(freight->getType()) << "\n";
    }
    file.close();
    cout << "Freights saved to " << filename << "\n";
}

// Save matches (vector of Match::MatchPair) to file
void FileManager::saveMatches(const string& filename, vector<string> matches) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open matches file " << filename << " for writing\n";
        return;
    }
    for (const std::string match : matches) {
        file << match << "\n";
    }
    file.close();
    cout << "Basic matches saved to " << filename << "\n";
}
