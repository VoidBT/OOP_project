#include "TUI.h"
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <memory> 
#include <iostream> 

//Done by Brendan Tjung Yew Wang 2401765

using namespace std;

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

void TUI::run(CStorage& cargoStorage, FStorage& freightStorage, Match& matches, ScheduleList& schedule) {
    int choice;

    handleLoadAllData(cargoStorage, freightStorage, schedule); // Load basic data

    do {
        showMainMenu();
        choice = getMenuChoice();

        switch (choice) {
        case 1: {
            int cargoChoice;
            do {
				//Menu for Cargo Operations
                showCargoOperationsMenu();
                cargoChoice = getMenuChoice();
                switch (cargoChoice) {
                case 1: handleAddCargo(cargoStorage); break;
                case 2: handleEditCargo(cargoStorage); break;
                case 3: handleDeleteCargo(cargoStorage); break;
                case 4: cargoStorage.printAll(); break;
                case 5: handleDisplayCargoGroups(cargoStorage); break;
                case 0: cout << "Returning to main menu.\n"; break;
                default: cout << "Invalid choice. Please try again.\n"; break;
                }
            } while (cargoChoice != 0);
            break;
        }
        case 2: {
            int freightChoice;
            do {
				//Menu for Freight Operations
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
        case 3: {
            int schedulingChoice;
            do {
				//Menu for Scheduling Options
                showSchedulingOptionsMenu();
                schedulingChoice = getMenuChoice();
                switch (schedulingChoice) {
                case 1:
                    schedule.scheduleByArrivalTime();
                    break;
                case 2:
                    schedule.scheduleByFreightCapacity();
                    break;
                case 3:
                    matches.matchFreightAndCargo(freightStorage, cargoStorage);
                    cout << "Basic freight and cargo matching completed. (No assignments made to freights for this option)\n";
                    break;
                case 0: cout << "Returning to main menu.\n"; break;
                default: cout << "Invalid choice. Please try again.\n"; break;
                }
            } while (schedulingChoice != 0);
            break;
        }
        case 4: {
            int displayChoice;
            do {
				//Menu for Display Options
                showDisplayOptionsMenu();
                displayChoice = getMenuChoice();
                switch (displayChoice) {
                case 1: schedule.displayByArrivalTime(); break;
                case 2: schedule.displayByFreightCapacity(); break;
                case 3: schedule.displayUnderutilizedFreights(); break;
                case 4: schedule.displayUnassignedCargos(); break;
                case 5: schedule.printAll(); matches.printAll(); break;
                case 6: FilePrinter::printFileWithHeaders(matchesFilename, { "Freight ID", "Cargo ID", "Freight Time", "Cargo Time", "Destination" }); break;
                case 7: FilePrinter::printFileWithHeaders(cargoFilename, { "ID", "Time", "Destination", "Size" }); break;
                case 8: FilePrinter::printFileWithHeaders(freightFilename, { "ID", "Time", "Destination", "Type" }); break;
                case 9: FilePrinter::printFileWithHeaders(enhancedScheduleFilename, { "Freight ID", "Type", "Load/Cap", "Destination", "Time", "Assigned Cargos" }); break;
                case 0: cout << "Returning to main menu.\n"; break;
                default: cout << "Invalid choice. Please try again.\n"; break;
                }
            } while (displayChoice != 0);
            break;
        }
        case 5: {
            int fileChoice;
            do {
				//Menu for File Operations
                showFileOperationsMenu();
                fileChoice = getMenuChoice();
                switch (fileChoice) {
                case 1: handleLoadAllData(cargoStorage, freightStorage, schedule); break;
                case 2: handleSaveAllData(cargoStorage, freightStorage, matches, schedule); break;
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
    cout << "\n--- Main Menu ---\n";
    cout << "1. Cargo Operations\n";
    cout << "2. Freight Operations\n";
    cout << "3. Scheduling Options\n";
    cout << "4. Display Options\n";
    cout << "5. File Operations\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

//Cargo Operations Menu
void TUI::showCargoOperationsMenu() const {
    cout << "\n--- Cargo Operations ---\n";
    cout << "1. Add Cargo\n";
    cout << "2. Edit Cargo\n";
    cout << "3. Delete Cargo\n";
    cout << "4. List All Cargos\n";
    cout << "5. Display All Cargo Groups\n";
    cout << "0. Back to Main Menu\n";
    cout << "Enter your choice: ";
}

//Freight Operations Menu
void TUI::showFreightOperationsMenu() const {
    cout << "\n--- Freight Operations ---\n";
    cout << "1. Add Freight (to storage, will be extended type)\n";
    cout << "2. Edit Freight\n";
    cout << "3. Delete Freight\n";
    cout << "4. List All Stored Freights\n";
    cout << "5. Add Extended Freight (to Schedule List for scheduling - same as option 1 now)\n";
    cout << "6. Display All Extended Freights (with Types & Capacity)\n";
    cout << "0. Back to Main Menu\n";
    cout << "Enter your choice: ";
}

//Scheduling Options Menu
void TUI::showSchedulingOptionsMenu() const {
    cout << "\n--- Scheduling Options ---\n";
    cout << "1. Run Scheduling by Cargo Arrival Time\n";
    cout << "2. Run Scheduling by Freight Capacity Optimization\n";
    cout << "3. Run Basic Freight-Cargo Matching (for old report file)\n";
    cout << "0. Back to Main Menu\n";
    cout << "Enter your choice: ";
}

//Display Options Menu
void TUI::showDisplayOptionsMenu() const {
    cout << "\n--- Display Options ---\n";
    cout << "1. Display Last Scheduling Plan by Freight Arrival Time\n";
    cout << "2. Display Last Scheduling Plan by Freight Capacity\n";
    cout << "3. Display Underutilized Freights\n";
    cout << "4. Display Unassigned Cargos\n";
    cout << "5. Display Full Schedule State (Freights, Groups, Unassigned, Basic Matches)\n";
    cout << "6. View Basic Matches File (" << matchesFilename << ")\n";
    cout << "7. View Cargos File (" << cargoFilename << ")\n";
    cout << "8. View Freights File (" << freightFilename << ")\n";
    cout << "9. View Enhanced Schedule File (" << enhancedScheduleFilename << ")\n";
    cout << "0. Back to Main Menu\n";
    cout << "Enter your choice: ";
}

//File Operations Menu
void TUI::showFileOperationsMenu() const {
    cout << "\n--- File Operations ---\n";
    cout << "1. Load All Data (Cargos, Freights)\n";
    cout << "2. Save All Data (Cargos, Freights, Basic Matches, Enhanced Schedule)\n";
    cout << "3. Change Cargo File Path\n";
    cout << "4. Change Freight File Path\n";
    cout << "5. Change Matches File Path\n";
    cout << "6. Change Enhanced Schedule File Path\n";
    cout << "7. Reset File Paths to Default\n";
    cout << "8. Show Current File Paths\n";
    cout << "0. Back to Main Menu\n";
    cout << "Enter your choice: ";
}

//Current File Paths Display
void TUI::showCurrentFiles() const {
    cout << "\nCurrent File Paths:\n";
    cout << "Cargo File: " << cargoFilename << "\n";
    cout << "Freight File: " << freightFilename << "\n";
    cout << "Matches File: " << matchesFilename << "\n";
    cout << "Enhanced Schedule File: " << enhancedScheduleFilename << "\n";
}

// --- Input Helper Functions ---
int TUI::getMenuChoice() const {
    int choice;
    cout << ">> ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

//Input helper functions for string and integer inputs
string TUI::getInput(const string& prompt) const {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

//Input helper function for integer inputs with validation
int TUI::getIntInput(const string& prompt) const {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

//Input helper function for FreightType with validation
FreightType TUI::getFreightTypeInput() const {
    while (true) {
        cout << "Enter Freight Type (MiniMover, CargoCruiser, MegaCarrier): ";
        string typeStr = getInput("");
        try {
            return FreightExtended::stringToType(typeStr);
        }
        catch (const invalid_argument& e) {
            cout << "Invalid freight type. Please try again.\n";
        }
    }
}

// getCargoGroupData now includes timeWindow parameter
void TUI::getCargoGroupData(string& groupId, string& dest, int& maxSize, int& timeWindow) const {
    groupId = getInput("Enter Cargo Group ID: ");
    dest = getInput("Enter Destination for Cargo Group: ");
    maxSize = getIntInput("Enter Max Size (number of cargos) for Cargo Group: ");
    if (maxSize <= 0) {
        cout << "Max size must be positive. Setting to default (10).\n";
        maxSize = 10;
    }
    timeWindow = getIntInput("Enter Time Window for Cargo Group (e.g., 15 for 15 minutes): ");
    if (timeWindow <= 0) {
        cout << "Time window must be positive. Setting to default (15).\n";
        timeWindow = 15;
    }
}

// --- Handler Methods for Operations ---

void TUI::handleAddCargo(CStorage& cargoStorage) const {
    std::string id = getInput("Enter Cargo ID: ");
    int time = getIntInput("Enter Cargo Time (e.g., 900 for 9:00): ");
    std::string dest = getInput("Enter Cargo Destination: ");
    int size = getIntInput("Enter Cargo Size: ");
    cargoStorage.addCargo(Cargo(id, time, dest, size));
    std::cout << "Cargo added successfully.\n";
}

// Handle editing a cargo by ID
void TUI::handleEditCargo(CStorage& cargoStorage) const {
    string id = getInput("Enter ID of Cargo to edit: ");
    int newTime = getIntInput("Enter new Time: ");
    string newDest = getInput("Enter new Destination: ");
    cargoStorage.editCargo(id, newTime, newDest);
    cout << "Cargo edited successfully (if ID found).\n";
}

// Handle deleting a cargo by ID
void TUI::handleDeleteCargo(CStorage& cargoStorage) const {
    string id = getInput("Enter ID of Cargo to delete: ");
    cargoStorage.deleteCargo(id);
    cout << "Cargo deleted successfully (if ID found).\n";
}

// Handle adding a freight to the freight storage
void TUI::handleAddFreight(FStorage& freightStorage) const {
    string id = getInput("Enter Freight ID: ");
    int time = getIntInput("Enter Freight Time (e.g., 900 for 9:00): ");
    string dest = getInput("Enter Freight Destination: ");
    FreightType type = getFreightTypeInput();
    freightStorage.addFreight(make_shared<FreightExtended>(id, time, dest, type));
    cout << "Freight (Extended Type) added successfully to FStorage.\n";
}

// Handle editing a freight by ID
void TUI::handleEditFreight(FStorage& freightStorage) const {
    string id = getInput("Enter ID of Freight to edit: ");
    int newTime = getIntInput("Enter new Time: ");
    string newDest = getInput("Enter new Destination: ");
    freightStorage.editFreight(id, newTime, newDest);
    cout << "Freight edited successfully (if ID found).\n";
}

// Handle deleting a freight by ID
void TUI::handleDeleteFreight(FStorage& freightStorage) const {
    string id = getInput("Enter ID of Freight to delete: ");
    freightStorage.deleteFreight(id);
    cout << "Freight deleted successfully (if ID found).\n";
}

// Handle adding an extended freight to the schedule list
void TUI::handleAddFreightExtended(ScheduleList& schedule) const {
    string id = getInput("Enter Freight ID: ");
    int time = getIntInput("Enter Freight Time (e.g., 900 for 9:00): ");
    string dest = getInput("Enter Freight Destination: ");
    FreightType type = getFreightTypeInput();
    schedule.addFreight(make_shared<FreightExtended>(id, time, dest, type));
    cout << "Extended Freight added to schedule list successfully.\n";
}

// Handle displaying extended freights with types and load info
void TUI::handleDisplayExtendedFreights(ScheduleList& schedule) const {
    schedule.displayByArrivalTime(); // Displays extended freights with types & load info
}

// Handle displaying cargo groups
void TUI::handleDisplayCargoGroups(CStorage& storage) const {
    storage.printAllGroups(); // Prints all cargo groups
}

// Load all data from files into storage and schedule
void TUI::handleLoadAllData(CStorage& cargoStorage, FStorage& freightStorage, ScheduleList& schedule) const {
    schedule.reset();

    FileManager::loadCargos(cargoFilename, cargoStorage);
    FileManager::loadFreights(freightFilename, freightStorage);

    for(Cargo i: cargoStorage.getAllCargos()) {
		schedule.addCargo(i);
    }

	cargoStorage.CreateGroups(); // Create cargo groups after loading cargos

    for(std::shared_ptr<FreightExtended> i: freightStorage.getAllFreights()) {
        schedule.addFreight(i);
	}
    cout << "All data loaded.\n";
}

// Save all data to files
void TUI::handleSaveAllData(CStorage& cargoStorage, FStorage& freightStorage, Match& matches, ScheduleList& schedule) const {
    FileManager::saveCargos(cargoFilename, cargoStorage.getAllCargos());
    FileManager::saveFreights(freightFilename, freightStorage.getAllFreights());
    FileManager::saveMatches(matchesFilename, matches.getMatches()); // Now correctly uses vector<string>
    schedule.saveEnhancedSchedule(enhancedScheduleFilename);
    cout << "All data saved.\n";
}

// Change Cargo file path
void TUI::changeCargoFile() {
    string newFile = getInput("Enter new Cargo filename: ");
    if (!newFile.empty()) cargoFilename = newFile;
    cout << "Cargo filename updated.\n";
}

// Change Freight file path
void TUI::changeFreightFile() {
    string newFile = getInput("Enter new Freight filename: ");
    if (!newFile.empty()) freightFilename = newFile;
    cout << "Freight filename updated.\n";
}

// Change Matches file path
void TUI::changeMatchesFile() {
    string newFile = getInput("Enter new Matches filename: ");
    if (!newFile.empty()) matchesFilename = newFile;
    cout << "Matches filename updated.\n";
}

// Change Enhanced Schedule file path
void TUI::changeEnhancedScheduleFile() {
    string newFile = getInput("Enter new Enhanced Schedule filename: ");
    if (!newFile.empty()) enhancedScheduleFilename = newFile;
    cout << "Enhanced Schedule filename updated.\n";
}

// Reset file paths to default values
void TUI::resetToDefaultFiles() {
    cargoFilename = DEFAULT_CARGO_FILE;
    freightFilename = DEFAULT_FREIGHT_FILE;
    matchesFilename = DEFAULT_MATCHES_FILE;
    enhancedScheduleFilename = DEFAULT_ENHANCED_SCHEDULE_FILE;
    cout << "File paths reset to default.\n";
}