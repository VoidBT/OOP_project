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

    handleLoadAllData(cargoStorage, freightStorage); // Load basic data

    do {
        showMainMenu();
        choice = getMenuChoice();

        switch (choice) {
        case 1: {
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
        case 2: {
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
    cout << "\n--- Main Menu ---\n";
    cout << "1. Cargo Operations\n";
    cout << "2. Freight Operations\n";
    cout << "3. Cargo Group Operations\n";
    cout << "4. Scheduling Options\n";
    cout << "5. Display Options\n";
    cout << "6. File Operations\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

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

void TUI::showCargoGroupOperationsMenu() const {
    cout << "\n--- Cargo Group Operations ---\n";
    cout << "1. Create New Cargo Group\n";
    cout << "0. Back to Main Menu\n";
    cout << "Enter your choice: ";
}

void TUI::showSchedulingOptionsMenu() const {
    cout << "\n--- Scheduling Options ---\n";
    cout << "1. Run Scheduling by Cargo Arrival Time\n"; 
    cout << "2. Run Scheduling by Freight Capacity Optimization\n"; 
    cout << "3. Run Basic Freight-Cargo Matching (for old report file)\n"; 
    cout << "0. Back to Main Menu\n";
    cout << "Enter your choice: ";
}

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

string TUI::getInput(const string& prompt) const {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

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

// Corrected: getCargoGroupData now includes timeWindow parameter
void TUI::getCargoGroupData(string& groupId, string& dest, int& maxSize, int& timeWindow) const {
    groupId = getInput("Enter Cargo Group ID: ");
    dest = getInput("Enter Destination for Cargo Group: ");
    maxSize = getIntInput("Enter Max Size (number of cargos) for Cargo Group: ");
    if (maxSize <= 0) {
        cout << "Max size must be positive. Setting to default (10).\n";
        maxSize = 10;
    }
    timeWindow = getIntInput("Enter Time Window for Cargo Group (e.g., 15 for 15 minutes): "); // Added
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

void TUI::handleEditCargo(CStorage& cargoStorage) const {
    string id = getInput("Enter ID of Cargo to edit: ");
    int newTime = getIntInput("Enter new Time: ");
    string newDest = getInput("Enter new Destination: ");
    cargoStorage.editCargo(id, newTime, newDest);
    cout << "Cargo edited successfully (if ID found).\n";
}

void TUI::handleDeleteCargo(CStorage& cargoStorage) const {
    string id = getInput("Enter ID of Cargo to delete: ");
    cargoStorage.deleteCargo(id);
    cout << "Cargo deleted successfully (if ID found).\n";
}

void TUI::handleDisplayCargos() const {

}

// Fixed: handleAddFreight now creates and adds shared_ptr<FreightExtended>
void TUI::handleAddFreight(FStorage& freightStorage) const {
    string id = getInput("Enter Freight ID: ");
    int time = getIntInput("Enter Freight Time (e.g., 900 for 9:00): ");
    string dest = getInput("Enter Freight Destination: ");
    FreightType type = getFreightTypeInput(); // Prompt for type for FStorage
    freightStorage.addFreight(make_shared<FreightExtended>(id, time, dest, type)); // Use make_shared
    cout << "Freight (Extended Type) added successfully to FStorage.\n"; // Clarified message
}

void TUI::handleEditFreight(FStorage& freightStorage) const {
    string id = getInput("Enter ID of Freight to edit: ");
    int newTime = getIntInput("Enter new Time: ");
    string newDest = getInput("Enter new Destination: ");
    freightStorage.editFreight(id, newTime, newDest);
    cout << "Freight edited successfully (if ID found).\n";
}

void TUI::handleDeleteFreight(FStorage& freightStorage) const {
    string id = getInput("Enter ID of Freight to delete: ");
    freightStorage.deleteFreight(id);
    cout << "Freight deleted successfully (if ID found).\n";
}

void TUI::handleListAllStoredFreights(FStorage& freightStorage) const {

}

void TUI::handleAddFreightExtended(ScheduleList& schedule) const {
    string id = getInput("Enter Freight ID: ");
    int time = getIntInput("Enter Freight Time (e.g., 900 for 9:00): ");
    string dest = getInput("Enter Freight Destination: ");
    FreightType type = getFreightTypeInput();
    schedule.addFreight(std::make_shared<FreightExtended>(id, time, dest, type));
    cout << "Extended Freight added to schedule list successfully.\n";
}

// NEW: Implementation for displaying extended freights
void TUI::handleDisplayExtendedFreights(ScheduleList& schedule) const {
    schedule.displayByArrivalTime(); // This function already displays freights with types and load
}

// NEW: Implementation for displaying cargo groups
void TUI::handleDisplayCargoGroups(ScheduleList& schedule) const {
    schedule.printAll(); // This function already prints cargo groups
}


void TUI::handleAddCargoGroup(ScheduleList& schedule) const {
    std::string groupId;
    std::string dest;
    int maxSize;
    int timeWindow; // Added
    getCargoGroupData(groupId, dest, maxSize, timeWindow); // Pass timeWindow
    CargoGroup newGroup(groupId, dest, maxSize, timeWindow); // Pass timeWindow to constructor

    std::cout << "Now, add cargos to this group. Enter 'done' for Cargo ID when finished.\n";
    while (true) {
        std::string cargoId = getInput("Enter Cargo ID to add to group (or 'done'): ");
        if (cargoId == "done") {
            break;
        }
        int cargoTime = getIntInput("Enter Cargo Time: ");
        std::string cargoDest = getInput("Enter Cargo Destination: ");
        int cargoSize = getIntInput("Enter Cargo Size: "); // <--- ADD THIS LINE

        if (cargoDest != dest) {
            std::cout << "Warning: Cargo destination does not match group destination. Cargo not added.\n";
            continue;
        }

        Cargo tempCargo(cargoId, cargoTime, cargoDest, cargoSize); // <--- MODIFY THIS LINE
        if (newGroup.addCargo(tempCargo)) {
            std::cout << "Cargo " << cargoId << " added to group.\n";
        }
        else {
            std::cout << "Could not add cargo to group (group might be full or destination mismatch).\n";
        }
    }
    schedule.addCargoGroup(newGroup);
    std::cout << "Cargo Group '" << groupId << "' created and added to schedule list.\n";
}

void TUI::handleEditCargoGroup() {
    cout << "Cargo Group editing functionality not yet implemented.\n";
}

void TUI::handleDeleteCargoGroup() {
    cout << "Cargo Group deletion functionality not yet implemented.\n";
}

void TUI::handleLoadAllData(CStorage& cargoStorage, FStorage& freightStorage) const {
    FileManager::loadCargos(cargoFilename, cargoStorage);
    FileManager::loadFreights(freightFilename, freightStorage);
    cout << "All data loaded.\n";
}

void TUI::handleSaveAllData(CStorage& cargoStorage, FStorage& freightStorage, ScheduleList& schedule) const {
    FileManager::saveCargos(cargoFilename, cargoStorage.getAllCargos());
    FileManager::saveFreights(freightFilename, freightStorage.getAllFreights());
    FileManager::saveMatches(matchesFilename, schedule.getMatches()); // Saves basic matches
    schedule.saveEnhancedSchedule(enhancedScheduleFilename); // Saves enhanced schedule from last run
    cout << "All data saved.\n";
}

// --- File Path Change Functions ---

void TUI::changeCargoFile() {
    cargoFilename = getInput("Enter new path for Cargo File: ");
    cout << "Cargo file path updated to: " << cargoFilename << "\n";
}

void TUI::changeFreightFile() {
    freightFilename = getInput("Enter new path for Freight File: ");
    cout << "Freight file path updated to: " << freightFilename << "\n";
}

void TUI::changeMatchesFile() {
    matchesFilename = getInput("Enter new path for Matches File: ");
    cout << "Matches file path updated to: " << matchesFilename << "\n";
}

void TUI::changeEnhancedScheduleFile() {
    enhancedScheduleFilename = getInput("Enter new path for Enhanced Schedule File: ");
    cout << "Enhanced schedule file path updated to: " << enhancedScheduleFilename << "\n";
}

void TUI::resetToDefaultFiles() {
    cargoFilename = DEFAULT_CARGO_FILE;
    freightFilename = DEFAULT_FREIGHT_FILE;
    matchesFilename = DEFAULT_MATCHES_FILE;
    enhancedScheduleFilename = DEFAULT_ENHANCED_SCHEDULE_FILE;
    cout << "All file paths reset to defaults.\n";
}