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

// FilePrinter class (remains unchanged)
/*class FilePrinter {
public:
    static void printFile(const std::string& filename);
    static void printFileFormatted(const std::string& filename,
        const std::string& delimiter = " | ",
        int columnWidth = 15);
    static void printFileWithHeaders(const std::string& filename,
        const std::vector<std::string>& headers,
        const std::string& delimiter = " | ",
        int columnWidth = 15);

private:
    static std::vector<std::string> splitLine(const std::string& line, char delimiter = ',');
};*/