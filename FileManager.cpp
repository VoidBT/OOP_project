#include "FileManager.h"
#include <fstream>
#include <sstream> // For stringstream
#include <iostream> // For cerr/cout
#include <limits> // For numeric_limits

// Define the static const members here
const string FileManager::DEFAULT_CARGO_FILE_PATH = "cargo.txt";
const string FileManager::DEFAULT_FREIGHT_FILE_PATH = "freight.txt";
const string FileManager::DEFAULT_MATCHES_FILE_PATH = "matched_schedule.txt";

using namespace std;

void FileManager::saveMatches(const string& filename, const vector<Match>& matches) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << " for saving matches.\n";
        return;
    }
    for (const auto& m : matches) {
        // Save in CSV format
        file << m.freight.getID() << ","
            << m.cargo.getID() << ","
            << m.freight.getTime() << ","
            << m.freight.getDest() << "\n";
    }
    file.close();
    cout << "Matches saved to " << filename << endl;
}

// Implementations for loadCargos - now handles parsing directly
void FileManager::loadCargos(const string& filename, CStorage& cargoStorage) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening cargo file: " << filename << "\n";
        return;
    }

    cargoStorage.clearCargos(); // Clear existing data before loading

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id_str, time_str, dest;

        // Read ID, then time, then destination, using ',' as delimiter
        if (getline(ss, id_str, ',') && getline(ss, time_str, ',') && getline(ss, dest)) {
            try {
                int time = stoi(time_str); // Convert time string to int
                cargoStorage.addCargo(Cargo(id_str, time, dest)); // Add directly to storage
            }
            catch (const invalid_argument& e) {
                cerr << "Warning: Could not parse time in " << filename << ": " << time_str << " in line: " << line << "\n";
            }
            catch (const out_of_range& e) {
                cerr << "Warning: Time value out of range in " << filename << ": " << time_str << " in line: " << line << "\n";
            }
        }
        else {
            // Only warn if the line is not empty (e.g., not a blank line)
            if (!line.empty()) {
                cerr << "Warning: Could not parse line in " << filename << ": " << line << "\n";
            }
        }
    }
    file.close();
    cout << "Cargos loaded from " << filename << endl;
}

// Implementations for loadFreights - now handles parsing directly
void FileManager::loadFreights(const string& filename, FStorage& freightStorage) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening freight file: " << filename << "\n";
        return;
    }

    freightStorage.clearFreights(); // Clear existing data before loading

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id_str, time_str, dest;

        // Read ID, then time, then destination, using ',' as delimiter
        if (getline(ss, id_str, ',') && getline(ss, time_str, ',') && getline(ss, dest)) {
            try {
                int time = stoi(time_str); // Convert time string to int
                freightStorage.addFreight(Freight(id_str, time, dest)); // Add directly to storage
            }
            catch (const invalid_argument& e) {
                cerr << "Warning: Could not parse time in " << filename << ": " << time_str << " in line: " << line << "\n";
            }
            catch (const out_of_range& e) {
                cerr << "Warning: Time value out of range in " << filename << ": " << time_str << " in line: " << line << "\n";
            }
        }
        else {
            // Only warn if the line is not empty (e.g., not a blank line)
            if (!line.empty()) {
                cerr << "Warning: Could not parse line in " << filename << ": " << line << "\n";
            }
        }
    }
    file.close();
    cout << "Freights loaded from " << filename << endl;
}

// New saveCargos method
void FileManager::saveCargos(const string& filename, const vector<Cargo>& cargos) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening: " << filename << " for saving cargos.\n";
        return;
    }
    for (const auto& c : cargos) {
        file << c.getID() << "," << c.getTime() << "," << c.getDest() << "\n";
    }
    file.close();
    cout << "Cargos saved to " << filename << endl;
}

// New saveFreights method
void FileManager::saveFreights(const string& filename, const vector<Freight>& freights) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening: " << filename << " for saving freights.\n";
        return;
    }
    for (const auto& f : freights) {
        file << f.getID() << "," << f.getTime() << "," << f.getDest() << "\n";
    }
    file.close();
    cout << "Freights saved to " << filename << endl;
}