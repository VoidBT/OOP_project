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

class FileManager {
public:
    static void loadCargos(const std::string& filename, CStorage& cargoStorage);
    static void saveCargos(const std::string& filename, const std::vector<Cargo>& cargos);

    static void loadFreights(const std::string& filename, FStorage& freightStorage);
    static void saveFreights(const std::string& filename, const std::vector<std::shared_ptr<FreightExtended>>& freights);

    static void saveMatches(const std::string& filename, const std::vector<Match>& matches);
};