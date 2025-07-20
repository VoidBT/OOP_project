#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include "FilePrinter.h"
#include "FileManager.h"
#include "CStorage.h"
#include "FStorage.h"
#include "ScheduleList.h" // Now includes all scheduling logic
#include "CargoGroup.h"

using namespace std;

class TUI {
private:
    string cargoFilename;
    string freightFilename;
    string matchesFilename;

    // Static constants for default files
    static const string DEFAULT_CARGO_FILE;
    static const string DEFAULT_FREIGHT_FILE;
    static const string DEFAULT_MATCHES_FILE;

    // Menus
    void showSchedulingOptionsMenu() const;
    void showCargoOperationsMenu() const;
    void showFreightOperationsMenu() const;
    void showDisplayOptionsMenu() const;
    void showFileOperationsMenu() const;

    // Input helpers
    FreightType getFreightTypeInput() const;
    void getCargoGroupData(string& groupId, string& dest, int& maxSize) const;

public:
    TUI();

    // Main UI loop
    void run(CStorage& cargoStorage, FStorage& freightStorage, ScheduleList& schedule);

    // Menus
    void showMainMenu() const;
    int getMenuChoice() const;
    void showCurrentFiles() const;

    // File operations
    void changeCargoFile();
    void changeFreightFile();
    void changeMatchesFile();
    void resetToDefaultFiles();

    // Input methods
    string getInput(const string& prompt) const;
    int getIntInput(const string& prompt) const;
    void getCargoData(string& id, int& time, string& dest) const;
    void getFreightData(string& id, int& time, string& dest, FreightType& type) const;

    // Display methods
    void displayCargo(const CStorage& storage) const;
    void displayFreight(const FStorage& storage) const;
    void displayMatches(const ScheduleList& schedule) const;

    // Scheduling and display handlers
    void handleSchedulingOptions(ScheduleList& schedule, CStorage& cargoStorage, FStorage& freightStorage);
    void handleDisplayOptions(const ScheduleList& schedule, CStorage& cargoStorage, FStorage& freightStorage) const;

    // Filename getters
    string getCargoFilename() const { return cargoFilename; }
    string getFreightFilename() const { return freightFilename; }
    string getMatchesFilename() const { return matchesFilename; }
};