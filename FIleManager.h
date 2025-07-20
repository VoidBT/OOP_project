#pragma once // Include guard
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

// Forward declarations if needed, but direct includes are safer for small projects
#include "Cargo.h"
#include "Freight.h"
#include "CStorage.h"
#include "FStorage.h"
#include "Match.h"
#include "FreightTypes.h" // For FreightExtended, if used for saving enhanced schedule

// Utility class to print file contents
class FilePrinter {
public:
    static void printFileWithHeaders(const std::string& filename, const std::vector<std::string>& headers);
};


class FileManager {
public:
    static void loadCargos(const std::string& filename, CStorage& cargoStorage);
    static void saveCargos(const std::string& filename, const std::vector<Cargo>& cargos);

    static void loadFreights(const std::string& filename, FStorage& freightStorage);
    static void saveFreights(const std::string& filename, const std::vector<Freight>& freights);

    static void saveMatches(const std::string& filename, const std::vector<Match>& matches);
    // You might need a method to load FreightExtended objects into ScheduleList if reading a schedule
    // And a method to save the final scheduled state (already in ScheduleList::saveEnhancedSchedule)
};