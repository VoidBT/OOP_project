#pragma once // Include guard
#include <string>
#include <vector>
#include <memory> // For std::shared_ptr and std::make_shared

// Ensure all necessary headers are included
#include "CStorage.h"
#include "FStorage.h"
#include "ScheduleList.h"
#include "CargoGroup.h"
#include "FreightTypes.h" // For FreightExtended and FreightType
#include "FileManager.h"  // For FileManager and FilePrinter

class TUI {
private:
    std::string cargoFilename;
    std::string freightFilename;
    std::string matchesFilename;
    std::string enhancedScheduleFilename;

    // Static const members for default file names
    static const std::string DEFAULT_CARGO_FILE;
    static const std::string DEFAULT_FREIGHT_FILE;
    static const std::string DEFAULT_MATCHES_FILE;
    static const std::string DEFAULT_ENHANCED_SCHEDULE_FILE;

public:
    TUI();
    void run(CStorage& cargoStorage, FStorage& freightStorage, ScheduleList& schedule);

private:
    // Menu Display Functions
    void showMainMenu() const;
    void showCargoOperationsMenu() const;
    void showFreightOperationsMenu() const;
    void showCargoGroupOperationsMenu() const;
    void showSchedulingOptionsMenu() const;
    void showDisplayOptionsMenu() const;
    void showFileOperationsMenu() const;
    void showCurrentFiles() const;

    // Input Helper Functions
    int getMenuChoice() const;
    std::string getInput(const std::string& prompt) const;
    int getIntInput(const std::string& prompt) const;
    FreightType getFreightTypeInput() const;
    void getCargoGroupData(std::string& groupId, std::string& dest, int& maxSize) const;

    // Handler Methods for Operations
    void handleAddCargo(CStorage& cargoStorage) const;
    void handleEditCargo(CStorage& cargoStorage) const;
    void handleDeleteCargo(CStorage& cargoStorage) const;
    void handleDisplayCargos() const; // Renamed from printAll to match new context

    void handleAddFreight(FStorage& freightStorage) const;
    void handleEditFreight(FStorage& freightStorage) const;
    void handleDeleteFreight(FStorage& freightStorage) const;
    void handleListAllStoredFreights(FStorage& freightStorage) const; // Renamed for clarity
    void handleAddFreightExtended(ScheduleList& schedule) const; // New: Adds FreightExtended to schedule
    void handleDisplayExtendedFreights(ScheduleList& schedule) const; // NEW: Display freights in ScheduleList with types

    void handleAddCargoGroup(ScheduleList& schedule) const;
    void handleDisplayCargoGroups(ScheduleList& schedule) const; // NEW: Display cargo groups in ScheduleList
    void handleEditCargoGroup(); // Placeholder
    void handleDeleteCargoGroup(); // Placeholder

    void handleLoadAllData(CStorage& cargoStorage, FStorage& freightStorage) const;
    void handleSaveAllData(CStorage& cargoStorage, FStorage& freightStorage, ScheduleList& schedule) const;

    // File Path Change Functions
    void changeCargoFile();
    void changeFreightFile();
    void changeMatchesFile();
    void changeEnhancedScheduleFile();
    void resetToDefaultFiles();
};