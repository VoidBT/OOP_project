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

//Done by Ryan Ang Rui Heng 2400522

using namespace std;

class FileManager {
public:
	// Load and save methods for CStorage
    static void loadCargos(const string& filename, CStorage& cargoStorage);
    static void saveCargos(const string& filename, const vector<Cargo>& cargos);

	// Load and save methods for FStorage
    static void loadFreights(const string& filename, FStorage& freightStorage);
    static void saveFreights(const string& filename, const vector<shared_ptr<FreightExtended>>& freights);

    // Updated to save vector of Match::MatchPair, not Match objects
    static void saveMatches(const string& filename, vector<string> matches);
};
