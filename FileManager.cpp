#include "FileManager.h"
#include <sstream>
#include <algorithm> // For std::sort
#include <iomanip>   // Added for std::setw and std::left

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
        std::string id, dest, timeStr, sizeStr;
        int time, size;

        if (std::getline(ss, id, ',') &&
            std::getline(ss, timeStr, ',') &&
            std::getline(ss, dest, ',') &&
            std::getline(ss, sizeStr)) {
            try {
                time = std::stoi(timeStr);
                size = std::stoi(sizeStr);
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
            std::cerr << "Skipping malformed cargo line (missing fields): " << line << "\n";
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
        if (line.empty()) continue;
        stringstream ss(line);
        string id, dest, timeStr, typeStr;
        int time;
        FreightType type;

        // Expect ID, Time, Dest, Type
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
            catch (const std::runtime_error& e) { // Catch invalid_argument from stringToType
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

void FileManager::saveFreights(const string& filename, const vector<shared_ptr<FreightExtended>>& freights) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open freight file " << filename << " for writing\n";
        return;
    }
    for (const auto& freight : freights) {
        file << freight->getID() << "," << freight->getTime() << "," << freight->getDest() << ","
            << FreightExtended::typeToString(freight->getType()) << "\n";
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

// FilePrinter implementation (remains unchanged)
/*vector<string> FilePrinter::splitLine(const string& line, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(line);

    while (getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

void FilePrinter::printFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

void FilePrinter::printFileFormatted(const string& filename,
    const string& delimiter,
    int columnWidth) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<string> tokens = splitLine(line, ',');
        for (const auto& token : tokens) {
            cout << left << setw(columnWidth) << token << delimiter;
        }
        cout << endl;
    }
    file.close();
}

void FilePrinter::printFileWithHeaders(const string& filename,
    const vector<string>& headers,
    const string& delimiter,
    int columnWidth) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    // Print headers
    for (const auto& header : headers) {
        cout << left << setw(columnWidth) << header << delimiter;
    }
    cout << endl;

    // Print separator line
    cout << string(headers.size() * (columnWidth + delimiter.length()), '-') << endl;

    // Print file contents
    string line;
    while (getline(file, line)) {
        vector<string> tokens = splitLine(line, ',');
        for (const auto& token : tokens) {
            cout << left << setw(columnWidth) << token << delimiter;
        }
        cout << endl;
    }
    file.close();
}*/