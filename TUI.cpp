#include "TUI.h"
#include <algorithm> // For std::transform if needed
#include <limits>    // For std::numeric_limits
#include <stdexcept> // For std::invalid_argument, std::out_of_range
#include <memory>    // Needed for std::make_shared

// Define the static const members here
const std::string TUI::DEFAULT_CARGO_FILE = "cargo.txt";
const std::string TUI::DEFAULT_FREIGHT_FILE = "freight.txt";
const std::string TUI::DEFAULT_MATCHES_FILE = "matched_schedule.txt";
const std::string TUI::DEFAULT_ENHANCED_SCHEDULE_FILE = "enhanced_schedule.txt"; // Initialize new default

// Use std namespace in .cpp file
using namespace std;

TUI::TUI()
    : cargoFilename(DEFAULT_CARGO_FILE),
    freightFilename(DEFAULT_FREIGHT_FILE),
    matchesFilename(DEFAULT_MATCHES_FILE),
    enhancedScheduleFilename(DEFAULT_ENHANCED_SCHEDULE_FILE) {
}

void TUI::run(CStorage& cargoStorage, FStorage& freightStorage, ScheduleList& schedule) {
    int choice;

    // Load initial data
    handleLoadAllData(cargoStorage, freightStorage);

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
                case 5: handleAddFreightExtended(schedule); break; // Add to ScheduleList
                case 0: cout << "Returning to main menu.\n"; break;
                default: cout << "Invalid choice. Please try again.\n"; break;
                }
            } while (freightChoice != 0);
            break;
        }
        case 3: { // Cargo Group Operations (New)
            int groupChoice;
            do {
                showCargoGroupOperationsMenu();
                groupChoice = getMenuChoice();
                switch (groupChoice) {
                case 1: handleAddCargoGroup(schedule); break;
                    // Add more operations here as needed, like edit, remove, display specific group
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
                    cout << "Scheduling by arrival time completed.\n";
                    break;
                case 2:
                    schedule.scheduleByFreightCapacity();
                    cout << "Scheduling by freight capacity completed.\n";
                    break;
                case 3:
                    schedule.matchFreightAndCargo(freightStorage, cargoStorage);
                    cout << "Basic freight and cargo matching completed.\n";
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
                case 5: schedule.printAll(); break; // Prints all schedule data
                case 6: FilePrinter::printFileWithHeaders(matchesFilename, { "Freight ID", "Cargo ID", "Time", "Destination" }); break;
                case 7: FilePrinter::printFileWithHeaders(cargoFilename, { "ID", "Time", "Destination" }); break;
                case 8: FilePrinter::printFileWithHeaders(freightFilename, { "ID", "Time", "Destination" }); break;
                case 9: FilePrinter::printFileWithHeaders(enhancedScheduleFilename, { "Freight ID", "Type", "Capacity", "Destination", "Time", "Assigned Cargos" }); break; // New: View Enhanced Schedule
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
                case 6: changeEnhancedScheduleFile(); break; // New: Change enhanced schedule file
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
    cout << "3. Cargo Group Operations\n"; // New option
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
    cout << "0. Back to Main Menu\n";
    cout << "Enter your choice: ";
}

void TUI::showFreightOperationsMenu() const {
    cout << "\n--- Freight Operations ---\n";
    cout << "1. Add Basic Freight\n";
    cout << "2. Edit Freight\n";
    cout << "3. Delete Freight\n";
    cout << "4. List All Freights\n";
    cout << "5. Add Extended Freight (for scheduling)\n"; // New option
    cout << "0. Back to Main Menu\n";
    cout << "Enter your choice: ";
}

void TUI::showCargoGroupOperationsMenu() const {
    cout << "\n--- Cargo Group Operations ---\n";
    cout << "1. Create New Cargo Group\n";
    // Add more options like 'Add Cargo to Group', 'Remove Cargo from Group', 'Display Group'
    cout << "0. Back to Main Menu\n";
    cout << "Enter your choice: ";
}

void TUI::showSchedulingOptionsMenu() const {
    cout << "\n--- Scheduling Options ---\n";
    cout << "1. Schedule by Arrival Time\n";
    cout << "2. Schedule by Freight Capacity\n";
    cout << "3. Run Basic Freight-Cargo Matching\n";
    cout << "0. Back to Main Menu\n";
    cout << "Enter your choice: ";
}

void TUI::showDisplayOptionsMenu() const {
    cout << "\n--- Display Options ---\n";
    cout << "1. Display Schedule by Arrival Time\n";
    cout << "2. Display Schedule by Freight Capacity\n";
    cout << "3. Display Underutilized Freights\n";
    cout << "4. Display Unassigned Cargos\n";
    cout << "5. Display Full Schedule (including initial matches)\n";
    cout << "6. View Basic Matches File (" << matchesFilename << ")\n";
    cout << "7. View Cargos File (" << cargoFilename << ")\n";
    cout << "8. View Freights File (" << freightFilename << ")\n";
    cout << "9. View Enhanced Schedule File (" << enhancedScheduleFilename << ")\n"; // New: View Enhanced Schedule File
    cout << "0. Back to Main Menu\n";
    cout << "Enter your choice: ";
}

void TUI::showFileOperationsMenu() const {
    cout << "\n--- File Operations ---\n";
    cout << "1. Load All Data (Cargos, Freights)\n";
    cout << "2. Save All Data (Cargos, Freights, Matches, Enhanced Schedule)\n";
    cout << "3. Change Cargo File Path\n";
    cout << "4. Change Freight File Path\n";
    cout << "5. Change Matches File Path\n";
    cout << "6. Change Enhanced Schedule File Path\n"; // New option
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
    // Clear the input buffer
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
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
            return value;
        }
    }
}

FreightType TUI::getFreightTypeInput() const {
    while (true) {
        cout << "Enter Freight Type (MiniMover, CargoCruiser, MegaCarrier): ";
        string typeStr = getInput("");
        try {
            // Corrected: Call static method with class name
            return FreightExtended::stringToType(typeStr);
        }
        catch (const invalid_argument& e) {
            cout << "Invalid freight type. Please try again.\n";
        }
    }
}

void TUI::getCargoGroupData(string& groupId, string& dest, int& maxSize) const {
    groupId = getInput("Enter Cargo Group ID: ");
    dest = getInput("Enter Destination for Cargo Group: ");
    maxSize = getIntInput("Enter Max Size for Cargo Group (default 10): ");
    if (maxSize <= 0) {
        cout << "Max size must be positive. Setting to default (10).\n";
        maxSize = 10;
    }
}


// --- Handler Methods for Operations ---

void TUI::handleAddCargo(CStorage& cargoStorage) const {
    string id = getInput("Enter Cargo ID: ");
    int time = getIntInput("Enter Cargo Time (e.g., 900 for 9:00): ");
    string dest = getInput("Enter Cargo Destination: ");
    cargoStorage.addCargo(Cargo(id, time, dest));
    cout << "Cargo added successfully.\n";
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

void TUI::handleAddFreight(FStorage& freightStorage) const {
    string id = getInput("Enter Freight ID: ");
    int time = getIntInput("Enter Freight Time (e.g., 900 for 9:00): ");
    string dest = getInput("Enter Freight Destination: ");
    freightStorage.addFreight(Freight(id, time, dest));
    cout << "Basic Freight added successfully.\n";
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

void TUI::handleAddFreightExtended(ScheduleList& schedule) const {
    string id = getInput("Enter Freight ID: ");
    int time = getIntInput("Enter Freight Time (e.g., 900 for 9:00): ");
    string dest = getInput("Enter Freight Destination: ");
    FreightType type = getFreightTypeInput();
    // Corrected: use std::make_shared
    schedule.addFreight(std::make_shared<FreightExtended>(id, time, dest, type));
    cout << "Extended Freight added to schedule list successfully.\n";
}

void TUI::handleAddCargoGroup(ScheduleList& schedule) const {
    string groupId;
    string dest;
    int maxSize;
    getCargoGroupData(groupId, dest, maxSize);
    CargoGroup newGroup(groupId, dest, maxSize);

    // Prompt for cargos to add to the group
    cout << "Now, add cargos to this group. Enter 'done' for Cargo ID when finished.\n";
    while (true) {
        string cargoId = getInput("Enter Cargo ID to add to group (or 'done'): ");
        if (cargoId == "done") {
            break;
        }
        int cargoTime = getIntInput("Enter Cargo Time: ");
        string cargoDest = getInput("Enter Cargo Destination: ");

        if (cargoDest != dest) {
            cout << "Warning: Cargo destination does not match group destination. Cargo not added.\n";
            continue;
        }

        Cargo tempCargo(cargoId, cargoTime, cargoDest);
        if (newGroup.addCargo(tempCargo)) {
            cout << "Cargo " << cargoId << " added to group.\n";
        }
        else {
            cout << "Could not add cargo to group (group might be full or destination mismatch).\n";
        }
    }
    schedule.addCargoGroup(newGroup);
    cout << "Cargo Group '" << groupId << "' created and added to schedule list.\n";
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
    // Note: Matches are generated, not loaded directly.
    cout << "All data loaded.\n";
}

void TUI::handleSaveAllData(CStorage& cargoStorage, FStorage& freightStorage, ScheduleList& schedule) const {
    FileManager::saveCargos(cargoFilename, cargoStorage.getAllCargos());
    FileManager::saveFreights(freightFilename, freightStorage.getAllFreights());
    FileManager::saveMatches(matchesFilename, schedule.getMatches());
    schedule.saveEnhancedSchedule(enhancedScheduleFilename); // Save the enhanced schedule
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