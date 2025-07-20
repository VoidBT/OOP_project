#pragma once // Add this include guard
#include <iostream>
#include <iomanip>
#include <string>
#include <vector> // Required for std::vector<std::string> headers
#include <limits> // Required for std::numeric_limits
#include <memory> // For std::make_shared (used in TUI.cpp for FreightExtended)

#include "FilePrinter.h"
#include "FileManager.h"
#include "CStorage.h"
#include "FStorage.h"
#include "ScheduleList.h" // Now includes all scheduling logic
#include "CargoGroup.h"
#include "FreightTypes.h" // Required for FreightType enum and FreightExtended class definition

// It's generally better to avoid 'using namespace std;' in headers
// to prevent potential name collisions in other files that include this header.
// Instead, qualify names with 'std::' or include 'using namespace std;' in the .cpp file.

class TUI {
private:
    std::string cargoFilename;
    std::string freightFilename;
    std::string matchesFilename;
    std::string enhancedScheduleFilename; // New: for saving enhanced schedules

    // Static constants for default files
    static const std::string DEFAULT_CARGO_FILE;
    static const std::string DEFAULT_FREIGHT_FILE;
    static const std::string DEFAULT_MATCHES_FILE;
    static const std::string DEFAULT_ENHANCED_SCHEDULE_FILE; // New default filename

    // Menus
    void showSchedulingOptionsMenu() const;
    void showCargoOperationsMenu() const;
    void showFreightOperationsMenu() const;
    void showDisplayOptionsMenu() const;
    void showFileOperationsMenu() const;
    void showCargoGroupOperationsMenu() const; // New: Menu for CargoGroup

    // Input helpers (made private as they are primarily internal to TUI)
    FreightType getFreightTypeInput() const;
    void getCargoGroupData(std::string& groupId, std::string& dest, int& maxSize) const;
    int getIntInput(const std::string& prompt) const;
    std::string getInput(const std::string& prompt) const;

    // Private helper methods for operations
    void handleAddCargo(CStorage& cargoStorage) const;
    void handleEditCargo(CStorage& cargoStorage) const;
    void handleDeleteCargo(CStorage& cargoStorage) const;

    void handleAddFreight(FStorage& freightStorage) const;
    void handleEditFreight(FStorage& freightStorage) const;
    void handleDeleteFreight(FStorage& freightStorage) const;
    void handleAddFreightExtended(ScheduleList& schedule) const; // New: for adding FreightExtended to ScheduleList

    void handleAddCargoGroup(ScheduleList& schedule) const; // New: for adding CargoGroup to ScheduleList
    void handleEditCargoGroup(); // Placeholder, requires more logic
    void handleDeleteCargoGroup(); // Placeholder, requires more logic

    void handleLoadAllData(CStorage& cargoStorage, FStorage& freightStorage) const;
    void handleSaveAllData(CStorage& cargoStorage, FStorage& freightStorage, ScheduleList& schedule) const;

public:
    TUI();

    // Main UI loop
    void run(CStorage& cargoStorage, FStorage& freightStorage, ScheduleList& schedule);

    // Menus (kept public as they are entry points)
    void showMainMenu() const;
    int getMenuChoice() const;
    void showCurrentFiles() const;

    // File operations (kept public for direct call from main menu handler)
    void changeCargoFile();
    void changeFreightFile();
    void changeMatchesFile();
    void changeEnhancedScheduleFile(); // New: for enhanced schedule file
    void resetToDefaultFiles();
};