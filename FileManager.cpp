#include "FileManager.h"
#include <sstream>
#include <algorithm> // For std::sort

using namespace std;

// In FileManager.cpp (only relevant parts shown)

void FileManager::loadCargos(const std::string& filename, CStorage& cargoStorage) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open cargo file " << filename << "\n";
        return;
    }

    cargoStorage = CStorage(); // Clear existing cargos
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, dest, timeStr, sizeStr; // sizeStr is now properly used
        int time, size;

        // Ensure all four fields are read
        if (std::getline(ss, id, ',') &&
            std::getline(ss, timeStr, ',') &&
            std::getline(ss, dest, ',') &&
            std::getline(ss, sizeStr)) {
            try {
                time = std::stoi(timeStr);
                size = std::stoi(sizeStr); // Parse size
                // NEW: Pass size to Cargo constructor
                cargoStorage.addCargo(Cargo(id, time, dest, size));
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Skipping malformed cargo line: " << line << " (" << e.what() << ")\n";
            }
            catch (const std::out_of_range& e) {
                std::cerr << "Skipping cargo line with out-of-range number: " << line << " (" << e.what() << ")\n";
            }
        }
        else {
            std::cerr << "Skipping malformed cargo line (missing fields): " << line << "\n"; // More specific error
        }
    }
    file.close();
    std::cout << "Cargos loaded from " << filename << "\n";
}

void FileManager::saveCargos(const std::string& filename, const std::vector<Cargo>& cargos) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open cargo file " << filename << " for writing\n";
        return;
    }
    for (const auto& cargo : cargos) {
        // NEW: Save the size along with other cargo data
        file << cargo.getID() << "," << cargo.getTime() << "," << cargo.getDest() << "," << cargo.getSize() << "\n";
    }
    file.close();
    std::cout << "Cargos saved to " << filename << "\n";
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