#include "TUI.h"
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <memory> 
#include <iostream> 

using namespace std;

// Define the static const members here
const string TUI::DEFAULT_CARGO_FILE = "cargo.txt";
const string TUI::DEFAULT_FREIGHT_FILE = "freight.txt";
const string TUI::DEFAULT_MATCHES_FILE = "matched_schedule.txt";
const string TUI::DEFAULT_ENHANCED_SCHEDULE_FILE = "enhanced_schedule.txt";

TUI::TUI()
    : cargoFilename(DEFAULT_CARGO_FILE),
    freightFilename(DEFAULT_FREIGHT_FILE),
    matchesFilename(DEFAULT_MATCHES_FILE),
    enhancedScheduleFilename(DEFAULT_ENHANCED_SCHEDULE_FILE) {
}

void TUI::run(CStorage& cargoStorage, FStorage& freightStorage, ScheduleList& schedule) {
    int choice;

    handleLoadAllData(cargoStorage, freightStorage); // Load initial data

    do {
        showMainMenu();
        choice = getMenuChoice();

        switch (choice) {
        case 1: { // Cargo Operations
            int cargoChoice;
            do {
                showCargoOperationsMenu();
                cargoChoice = getMenuChoice();
                switch (cargoChoice) {
                case 1: handleAddCargo(cargoStorage); break;
                case 2: handleEditCargo(cargoStorage); break;
                case 3: handleDeleteCargo(cargoStorage); break;
                case 4: cargoStorage.printAll(); break;
                case 5: handleDisplayCargoGroups(schedule); break;
                case 0: cout << "Returning to main menu.\n"; break;
                default: cout << "Invalid choice. Please try again.\n"; break;
                }
            } while (cargoChoice != 0);
            break;
        }
        case 2: { // Freight Operations
            int freightChoice;
            do {
                showFreightOperationsMenu();
                freightChoice = getMenuChoice();
                switch (freightChoice) {
                case 1: handleAddFreight(freightStorage); break;
                case 2: handleEditFreight(freightStorage); break;
                case 3: handleDeleteFreight(freightStorage); break;
                case 4: freightStorage.printAll(); break;
                case 5: handleAddFreightExtended(schedule); break;
                case 6: handleDisplayExtendedFreights(schedule); break;
                case 0: cout << "Returning to main menu.\n"; break;
                default: cout << "Invalid choice. Please try again.\n"; break;
                }
            } while (freightChoice != 0);
            break;
        }
        case 3: { // Cargo Group Operations
            int groupChoice;
            do {
                showCargoGroupOperationsMenu();
                groupChoice = getMenuChoice();
                switch (groupChoice) {
                case 1: handleAddCargoGroup(schedule); break;
                case 0: cout << "Returning to main menu.\n"; break;
                default: cout << "Invalid choice. Please try again.\n"; break;
                }
            } while (groupChoice != 0);
            break;
        }
        case 4: { // Scheduling Options
            int schedulingChoice;
            do {
                showSchedulingOptionsMenu();
                schedulingChoice = getMenuChoice();
                switch (schedulingChoice) {
                case 1:
                    schedule.scheduleByArrivalTime();
                    cout << "Scheduling by arrival time completed and cargos assigned to freights.\n";
                    break;
                case 2:
                    schedule.scheduleByFreightCapacity();
                    cout << "Scheduling by freight capacity completed and cargos assigned to freights.\n";
                    break;
                case 3:
                    schedule.matchFreightAndCargo(freightStorage, cargoStorage);
                    cout << "Basic freight and cargo matching completed. (No assignments made to freights for this option)\n";
                    break;
                case 0: cout << "Returning to main menu.\n"; break;
                default: cout << "Invalid choice. Please try again.\n"; break;
                }
            } while (schedulingChoice != 0);
            break;
        }
        case 5: { // Display Options
            int displayChoice;
            do {
                showDisplayOptionsMenu();
                displayChoice = getMenuChoice();
                switch (displayChoice) {
                case 1: schedule.displayByArrivalTime(); break;
                case 2: schedule.displayByFreightCapacity(); break;
                case 3: schedule.displayUnderutilizedFreights(); break;
                case 4: schedule.displayUnassignedCargos(); break;
                case 5: schedule.printAll(); break;
                case 6: FilePrinter::printFileWithHeaders(matchesFilename, { "Freight ID", "Cargo ID", "Time", "Destination" }); break;
                case 7: FilePrinter::printFileWithHeaders(cargoFilename, { "ID", "Time", "Destination", "Size" }); break;
                case 8: FilePrinter::printFileWithHeaders(freightFilename, { "ID", "Time", "Destination", "Type" }); break;
                case 9: FilePrinter::printFileWithHeaders(enhancedScheduleFilename, { "Freight ID", "Type", "Load/Cap", "Destination", "Time", "Assigned Cargos" }); break;
                case 0: cout << "Returning to main menu.\n"; break;
                default: cout << "Invalid choice. Please try again.\n"; break;
                }
            } while (displayChoice != 0);
            break;
        }
        case 6: { // File Operations
            int fileChoice;
            do {
                showFileOperationsMenu();
                fileChoice = getMenuChoice();
                switch (fileChoice) {
                case 1: handleLoadAllData(cargoStorage, freightStorage); break;
                case 2: handleSaveAllData(cargoStorage, freightStorage, schedule); break;
                case 3: changeCargoFile(); break;
                case 4: changeFreightFile(); break;
                case 5: changeMatchesFile(); break;
                case 6: changeEnhancedScheduleFile(); break;
                case 7: resetToDefaultFiles(); break;
                case 8: showCurrentFiles(); break;
                case 0: cout << "Returning to main menu.\n"; break;
                default: cout << "Invalid choice. Please try again.\n"; break;
                }
            } while (fileChoice != 0);
            break;
        }
        case 0:
            cout << "Exiting program. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 0);
}

// --- Menu Display Functions ---

void TUI::showMainMenu() const {
    cout << "\n--- Main Menu ---\n"
        << "1. Cargo Operations\n"
        << "2. Freight Operations\n"
        << "3. Cargo Group Operations\n"
        << "4. Scheduling Options\n"
        << "5. Display Options\n"
        << "6. File Operations\n"
        << "0. Exit\n"
        << "Enter your choice: ";
}

// ... (other show*Menu() functions unchanged, just use similar style for cleaner output)

// --- Input Helper Functions ---

int TUI::getMenuChoice() const {
    int choice;
    cout << ">> ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

// Other input helpers unchanged...

// --- Handler Methods ---

void TUI::handleAddCargo(CStorage& cargoStorage) const {
    string id = getInput("Enter Cargo ID: ");
    int time = getIntInput("Enter Cargo Time (e.g., 900 for 9:00): ");
    string dest = getInput("Enter Cargo Destination: ");
    int size = getIntInput("Enter Cargo Size: ");
    cargoStorage.addCargo(Cargo(id, time, dest, size));
    cout << "Cargo added successfully.\n";
}

// Similar pattern for other handlers...

// --- File Operations ---

void TUI::handleLoadAllData(CStorage& cargoStorage, FStorage& freightStorage) const {
    FileManager::loadCargos(cargoFilename, cargoStorage);
    FileManager::loadFreights(freightFilename, freightStorage);
    cout << "All data loaded.\n";
}

void TUI::handleSaveAllData(CStorage& cargoStorage, FStorage& freightStorage, ScheduleList& schedule) const {
    FileManager::saveCargos(cargoFilename, cargoStorage.getAllCargos());
    FileManager::saveFreights(freightFilename, freightStorage.getAllFreights());
    FileManager::saveMatches(matchesFilename, schedule.getMatches()); // Saves basic matches
    schedule.saveEnhancedSchedule(enhancedScheduleFilename);           // Saves enhanced schedule from last run
    cout << "All data saved.\n";
}

// File path change functions unchanged...

