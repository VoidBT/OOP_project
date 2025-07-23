#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory> // For shared_ptr

#include "CStorage.h"
#include "FStorage.h"
#include "Match.h"
#include "FreightTypes.h" // For FreightExtended

using namespace std;

class FileManager {
public:
    static void loadCargos(const string& filename, CStorage& cargoStorage);
    static void saveCargos(const string& filename, const vector<Cargo>& cargos);

    static void loadFreights(const string& filename, FStorage& freightStorage);
    static void saveFreights(const string& filename, const vector<shared_ptr<FreightExtended>>& freights);

    // Updated to save vector of Match::MatchPair, not Match objects
    static void saveMatches(const string& filename, vector<string> matches);
};
