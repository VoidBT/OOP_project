#include "FileManager.h"
#include <sstream>
#include <algorithm> // For std::sort

using namespace std;

void FileManager::loadCargos(const string& filename, CStorage& cargoStorage) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open cargo file " << filename << "\n";
        return;
    }

    cargoStorage = CStorage(); // Clear existing cargos
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines
        stringstream ss(line);
        string id, dest, timeStr, sizeStr;
        int time, size;

        if (getline(ss, id, ',') &&
            getline(ss, timeStr, ',') &&
            getline(ss, dest, ',') &&
            getline(ss, sizeStr)) { // Read size as well for Cargo.txt format
            try {
                time = stoi(timeStr);
                size = stoi(sizeStr); // Read size, though Cargo class doesn't store it internally for now
                cargoStorage.addCargo(Cargo(id, time, dest));
            }
            catch (const invalid_argument& e) {
                cerr << "Skipping malformed cargo line: " << line << " (" << e.what() << ")\n";
            }
            catch (const out_of_range& e) {
                cerr << "Skipping cargo line with out-of-range number: " << line << " (" << e.what() << ")\n";
            }
        }
        else {
            cerr << "Skipping malformed cargo line: " << line << "\n";
        }
    }
    file.close();
    cout << "Cargos loaded from " << filename << "\n";
}

void FileManager::saveCargos(const string& filename, const vector<Cargo>& cargos) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open cargo file " << filename << " for writing\n";
        return;
    }
    for (const auto& cargo : cargos) {
        file << cargo.getID() << "," << cargo.getTime() << "," << cargo.getDest() << ",1\n"; // Added ',1' to match original format
    }
    file.close();
    cout << "Cargos saved to " << filename << "\n";
}

void FileManager::loadFreights(const string& filename, FStorage& freightStorage) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open freight file " << filename << "\n";
        return;
    }

    freightStorage = FStorage(); // Clear existing freights
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines
        stringstream ss(line);
        string id, dest, timeStr, typeStr;
        int time;

        if (getline(ss, id, ',') &&
            getline(ss, timeStr, ',') &&
            getline(ss, dest, ',') &&
            getline(ss, typeStr)) {
            try {
                time = stoi(timeStr);
                // Note: The basic Freight class doesn't store type, only FreightExtended does.
                // We're loading a basic Freight object here.
                freightStorage.addFreight(Freight(id, time, dest));
            }
            catch (const invalid_argument& e) {
                cerr << "Skipping malformed freight line: " << line << " (" << e.what() << ")\n";
            }
            catch (const out_of_range& e) {
                cerr << "Skipping freight line with out-of-range number: " << line << " (" << e.what() << ")\n";
            }
        }
        else {
            cerr << "Skipping malformed freight line: " << line << "\n";
        }
    }
    file.close();
    cout << "Freights loaded from " << filename << "\n";
}

void FileManager::saveFreights(const string& filename, const vector<Freight>& freights) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open freight file " << filename << " for writing\n";
        return;
    }
    for (const auto& freight : freights) {
        // Note: The basic Freight doesn't store type, so we can't save it here.
        // The type (MiniMover etc.) is part of FreightExtended.
        file << freight.getID() << "," << freight.getTime() << "," << freight.getDest() << ",Unknown\n"; // "Unknown" for basic Freight
    }
    file.close();
    cout << "Freights saved to " << filename << "\n";
}

void FileManager::saveMatches(const string& filename, const vector<Match>& matches) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open matches file " << filename << " for writing\n";
        return;
    }
    for (const auto& match : matches) {
        file << "Freight ID: " << match.freight.getID()
            << ", Cargo ID: " << match.cargo.getID()
            << ", Freight Time: " << match.freight.getTime()
            << ", Cargo Time: " << match.cargo.getTime()
            << ", Destination: " << match.freight.getDest() << "\n";
    }
    file.close();
    cout << "Basic matches saved to " << filename << "\n";
}

// FilePrinter implementation
void FilePrinter::printFileWithHeaders(const string& filename, const vector<string>& headers) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    cout << "\n--- Contents of " << filename << " ---\n";
    for (const string& header : headers) {
        cout << header << "\t";
    }
    cout << "\n";
    for (size_t i = 0; i < headers.size(); ++i) {
        cout << "--------\t"; // Adjust for typical header length
    }
    cout << "\n";

    string line;
    while (getline(file, line)) {
        cout << line << "\n";
    }
    file.close();
    cout << "--- End of " << filename << " ---\n";
}