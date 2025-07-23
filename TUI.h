#pragma once
#include <string>
#include <vector>
#include <memory>
#include "CStorage.h"
#include "FStorage.h"
#include "ScheduleList.h"
#include "CargoGroup.h"
#include "FreightTypes.h"
#include "FileManager.h"
#include "FilePrinter.h"

using namespace std;

class TUI {
private:
    string cargoFilename;
    string freightFilename;
    string matchesFilename;
    string enhancedScheduleFilename;

    static const string DEFAULT_CARGO_FILE;
    static const string DEFAULT_FREIGHT_FILE;
    static const string DEFAULT_MATCHES_FILE;
    static const string DEFAULT_ENHANCED_SCHEDULE_FILE;

public:
    TUI();
    void run(CStorage& cargoStorage, FStorage& freightStorage, ScheduleList& schedule);

private:
    void showMainMenu() const;
    void showCargoOperationsMenu() const;
    void showFreightOperationsMenu() const;
    void showCargoGroupOperationsMenu() const;
    void showSchedulingOptionsMenu() const;
    void showDisplayOptionsMenu() const;
    void showFileOperationsMenu() const;
    void showCurrentFiles() const;

    int getMenuChoice() const;
    string getInput(const string& prompt) const;
    int getIntInput(const string& prompt) const;
    FreightType getFreightTypeInput() const;
    void getCargoGroupData(string& groupId, string& dest, int& maxSize, int& timeWindow) const;

    void handleAddCargo(CStorage& cargoStorage) const;
    void handleEditCargo(CStorage& cargoStorage) const;
    void handleDeleteCargo(CStorage& cargoStorage) const;
    void handleDisplayCargos() const;

    void handleAddFreight(FStorage& freightStorage) const;
    void handleEditFreight(FStorage& freightStorage) const;
    void handleDeleteFreight(FStorage& freightStorage) const;
    void handleListAllStoredFreights(FStorage& freightStorage) const;
    void handleAddFreightExtended(ScheduleList& schedule) const;
    void handleDisplayExtendedFreights(ScheduleList& schedule) const;

    void handleAddCargoGroup(ScheduleList& schedule) const;
    void handleDisplayCargoGroups(ScheduleList& schedule) const;
    void handleEditCargoGroup();
    void handleDeleteCargoGroup();

    void handleLoadAllData(CStorage& cargoStorage, FStorage& freightStorage) const;
    void handleSaveAllData(CStorage& cargoStorage, FStorage& freightStorage, ScheduleList& schedule) const;

    void changeCargoFile();
    void changeFreightFile();
    void changeMatchesFile();
    void changeEnhancedScheduleFile();
    void resetToDefaultFiles();
};
