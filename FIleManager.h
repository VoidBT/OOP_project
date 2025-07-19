#pragma once
#include <string>
#include <vector>
#include "Match.h"
#include "CStorage.h"
#include "FStorage.h"
#include "Cargo.h"
#include "Freight.h"

using namespace std;

class FileManager {
public:
    static const string DEFAULT_CARGO_FILE_PATH;
    static const string DEFAULT_FREIGHT_FILE_PATH;
    static const string DEFAULT_MATCHES_FILE_PATH;

    static void saveMatches(const string& filename, const vector<Match>& matches);

    // Static methods for loading data into storage classes
    static void loadCargos(const string& filename, CStorage& cargoStorage);
    static void loadFreights(const string& filename, FStorage& freightStorage);

    // Static methods for saving data from vectors
    static void saveCargos(const string& filename, const vector<Cargo>& cargos);
    static void saveFreights(const string& filename, const vector<Freight>& freights);
};