#include "TUI.h"
// #include "EnhancedScheduleList.h" // REMOVE THIS LINE
#include <iostream>
#include <iomanip>
#include <limits> // Required for numeric_limits

// Define static constants
const string TUI::DEFAULT_CARGO_FILE = "cargo.txt";
const string TUI::DEFAULT_FREIGHT_FILE = "freight.txt";
const string TUI::DEFAULT_MATCHES_FILE = "matches.txt";

TUI::TUI() : cargoFilename(DEFAULT_CARGO_FILE),
freightFilename(DEFAULT_FREIGHT_FILE),
matchesFilename(DEFAULT_MATCHES_FILE) {
}

void TUI::showMainMenu() const {
    cout << "\nLogistics Management System\n"
        << "==========================\n";
    showCurrentFiles();
    cout << "--------------------------\n"
        << "1. Cargo Operations\n"
        << "2. Freight Operations\n"
        << "3. Scheduling Options\n"
        << "4. Display Options\n"
        << "5. File Operations\n"
        << "6. Exit\n"
        << "Enter your choice: ";
}

void TUI::showSchedulingOptionsMenu() const {
    cout << "\nScheduling Options\n"
        << "==================\n"
        << "1. Match Freight and Cargo\n"
        << "2. Schedule by Arrival Time\n" // New option
        << "3. Schedule by Freight Capacity\n" // New option
        << "4. Display Matches\n"
        << "5. Display Unassigned Cargos\n" // New option
        << "6. Display Underutilized Freights\n" // New option
        << "7. Back to Main Menu\n"
        << "Enter your choice: ";
}

void TUI::showCargoOperationsMenu() const {
    cout << "\nCargo Operations\n"
        << "================\n"
        << "1. Add Cargo\n"
        << "2. Edit Cargo\n"
        << "3. Delete Cargo\n"
        << "4. Back to Main Menu\n"
        << "Enter your choice: ";
}

void TUI::showFreightOperationsMenu() const {
    cout << "\nFreight Operations\n"
        << "==================\n"
        << "1. Add Freight\n"
        << "2. Edit Freight\n"
        << "3. Delete Freight\n"
        << "4. Back to Main Menu\n"
        << "Enter your choice: ";
}

void TUI::showDisplayOptionsMenu() const {
    cout << "\nDisplay Options\n"
        << "===============\n"
        << "1. Display All Cargos\n"
        << "2. Display All Freights\n"
        << "3. Display Matches (Original)\n" // Clarified to distinguish
        << "4. Display Schedule by Arrival Time\n" // New option
        << "5. Display Schedule by Freight Capacity\n" // New option
        << "6. Display Underutilized Freights\n" // New option
        << "7. Display Unassigned Cargos\n" // New option
        << "8. Back to Main Menu\n"
        << "Enter your choice: ";
}

void TUI::showFileOperationsMenu() const {
    cout << "\nFile Operations\n"
        << "===============\n"
        << "1. Change Cargo File\n"
        << "2. Change Freight File\n"
        << "3. Change Matches File\n"
        << "4. Reset to Default Files\n"
        << "5. Save Current Data\n"
        << "6. Back to Main Menu\n"
        << "Enter your choice: ";
}

FreightType TUI::getFreightTypeInput() const {
    cout << "Select Freight Type:\n"
        << "1. Mini Mover\n"
        << "2. Cargo Cruiser\n"
        << "3. Mega Carrier\n"
        << "Enter your choice: ";
    int choice = getIntInput("");
    switch (choice) {
    case 1: return FreightType::MINI_MOVER;
    case 2: return FreightType::CARGO_CRUISER;
    case 3: return FreightType::MEGA_CARRIER;
    default:
        cout << "Invalid choice. Defaulting to Mega Carrier.\n";
        return FreightType::MEGA_CARRIER;
    }
}

void TUI::getCargoGroupData(string& groupId, string& dest, int& maxSize) const {
    groupId = getInput("Enter Cargo Group ID: ");
    dest = getInput("Enter Destination: ");
    maxSize = getIntInput("Enter Maximum Size: ");
}

void TUI::run(CStorage& cargoStorage, FStorage& freightStorage, ScheduleList& schedule) {
    // Load initial data
    FileManager::loadCargos(cargoFilename, cargoStorage);
    FileManager::loadFreights(freightFilename, freightStorage);

    bool running = true;
    while (running) {
        showMainMenu();
        int choice = getMenuChoice();

        switch (choice) {
        case 1: { // Cargo Operations
            int sub;
            do {
                showCargoOperationsMenu();
                sub = getMenuChoice();
                string id, dest;
                int time;

                switch (sub) {
                case 1: // Add
                    getCargoData(id, time, dest);
                    cargoStorage.addCargo(Cargo(id, time, dest));
                    break;
                case 2: { // Edit
                    id = getInput("Enter cargo ID to edit: ");
                    time = getIntInput("Enter new time: ");
                    dest = getInput("Enter new destination: ");
                    cargoStorage.editCargo(id, time, dest);
                    break;
                }
                case 3: { // Delete
                    id = getInput("Enter cargo ID to delete: ");
                    cargoStorage.deleteCargo(id);
                    break;
                }
                case 4: break;
                default: cout << "Invalid choice.\n";
                }
            } while (sub != 4);
            break;
        }

        case 2: { // Freight Operations
            int sub;
            do {
                showFreightOperationsMenu();
                sub = getMenuChoice();
                string id, dest;
                int time;
                FreightType type;

                switch (sub) {
                case 1: // Add
                    getFreightData(id, time, dest, type);
                    freightStorage.addFreight(Freight(id, time, dest));
                    break;
                case 2: { // Edit
                    id = getInput("Enter freight ID to edit: ");
                    time = getIntInput("Enter new time: ");
                    dest = getInput("Enter new destination: ");
                    freightStorage.editFreight(id, time, dest);
                    break;
                }
                case 3: { // Delete
                    id = getInput("Enter freight ID to delete: ");
                    freightStorage.deleteFreight(id);
                    break;
                }
                case 4: break;
                default: cout << "Invalid choice.\n";
                }
            } while (sub != 4);
            break;
        }

        case 3: // Scheduling Options
            handleSchedulingOptions(schedule, cargoStorage, freightStorage);
            break;

        case 4: // Display Options
            handleDisplayOptions(schedule, cargoStorage, freightStorage);
            break;

        case 5: { // File Operations
            int sub;
            do {
                showFileOperationsMenu();
                sub = getMenuChoice();
                switch (sub) {
                case 1: changeCargoFile(); break;
                case 2: changeFreightFile(); break;
                case 3: changeMatchesFile(); break;
                case 4: resetToDefaultFiles(); break;
                case 5: {
                    FileManager::saveCargos(cargoFilename, cargoStorage.getAllCargos());
                    FileManager::saveFreights(freightFilename, freightStorage.getAllFreights());
                    schedule.saveEnhancedSchedule(matchesFilename);
                    cout << "Data saved.\n";
                    break;
                }
                case 6: break;
                default: cout << "Invalid choice.\n";
                }
            } while (sub != 6);
            break;
        }

        case 6: // Exit
            running = false;
            break;

        default:
            cout << "Invalid option. Try again.\n";
        }
    }

    // Save on exit
    FileManager::saveCargos(cargoFilename, cargoStorage.getAllCargos());
    FileManager::saveFreights(freightFilename, freightStorage.getAllFreights());
    schedule.saveEnhancedSchedule(matchesFilename);
}

int TUI::getMenuChoice() const {
    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
    return choice;
}

void TUI::showCurrentFiles() const {
    cout << "Current Files:\n"
        << "Cargo File: " << cargoFilename << "\n"
        << "Freight File: " << freightFilename << "\n"
        << "Matches File: " << matchesFilename << "\n";
}

void TUI::changeCargoFile() {
    string newFile = getInput("Enter new cargo file name: ");
    cargoFilename = newFile;
    cout << "Cargo file changed to: " << cargoFilename << "\n";
}

void TUI::changeFreightFile() {
    string newFile = getInput("Enter new freight file name: ");
    freightFilename = newFile;
    cout << "Freight file changed to: " << freightFilename << "\n";
}

void TUI::changeMatchesFile() {
    string newFile = getInput("Enter new matches file name: ");
    matchesFilename = newFile;
    cout << "Matches file changed to: " << matchesFilename << "\n";
}

void TUI::resetToDefaultFiles() {
    cargoFilename = DEFAULT_CARGO_FILE;
    freightFilename = DEFAULT_FREIGHT_FILE;
    matchesFilename = DEFAULT_MATCHES_FILE;
    cout << "Files reset to default.\n";
}

string TUI::getInput(const string& prompt) const {
    cout << prompt;
    string input;
    getline(cin, input);
    return input;
}

int TUI::getIntInput(const string& prompt) const {
    cout << prompt;
    int input;
    while (!(cin >> input)) {
        cin.clear(); // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        cout << "Invalid input. " << prompt;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
    return input;
}

void TUI::getCargoData(string& id, int& time, string& dest) const {
    id = getInput("Enter Cargo ID: ");
    time = getIntInput("Enter Arrival Time (in minutes): ");
    dest = getInput("Enter Destination: ");
}

void TUI::getFreightData(string& id, int& time, string& dest, FreightType& type) const {
    id = getInput("Enter Freight ID: ");
    time = getIntInput("Enter Arrival Time (in minutes): ");
    dest = getInput("Enter Destination: ");
    type = getFreightTypeInput();
}

void TUI::displayCargo(const CStorage& storage) const {
    cout << "\nCargo List:\n";
    cout << "===========\n";
    for (const auto& cargo : storage.getAllCargos()) {
        cout << "ID: " << cargo.getID()
            << ", Arrival Time: " << cargo.getTime()
            << ", Destination: " << cargo.getDest() << "\n";
    }
}

void TUI::displayFreight(const FStorage& storage) const {
    cout << "\nFreight List:\n";
    cout << "=============\n";
    for (const auto& freight : storage.getAllFreights()) {
        cout << "ID: " << freight.getID()
            << ", Arrival Time: " << freight.getTime()
            << ", Destination: " << freight.getDest() << "\n";
    }
}

void TUI::displayMatches(const ScheduleList& schedule) const {
    // This now calls the original printAll on ScheduleList
    // If you need specific "match" output, you'll need to iterate schedule.getMatches()
    schedule.printAll();
}

void TUI::handleSchedulingOptions(ScheduleList& schedule, CStorage& cargoStorage, FStorage& freightStorage) {
    int sub;
    do {
        showSchedulingOptionsMenu();
        sub = getMenuChoice();
        switch (sub) {
        case 1: // Match Freight and Cargo (original logic)
            schedule.matchFreightAndCargo(freightStorage, cargoStorage);
            cout << "Freight and Cargo matched successfully.\n";
            break;
        case 2: // Schedule by Arrival Time
            schedule.scheduleByArrivalTime();
            cout << "Scheduling by arrival time completed.\n";
            break;
        case 3: // Schedule by Freight Capacity
            schedule.scheduleByFreightCapacity();
            cout << "Scheduling by freight capacity completed.\n";
            break;
        case 4: // Display Matches (now calls printAll)
            schedule.printAll();
            break;
        case 5: // Display Unassigned Cargos
            schedule.displayUnassignedCargos();
            break;
        case 6: // Display Underutilized Freights
            schedule.displayUnderutilizedFreights();
            break;
        case 7: break; // Back to Main Menu
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (sub != 7);
}

void TUI::handleDisplayOptions(const ScheduleList& schedule, CStorage& cargoStorage, FStorage& freightStorage) const {
    int sub;
    do {
        showDisplayOptionsMenu();
        sub = getMenuChoice();
        switch (sub) {
        case 1: // Display All Cargos
            displayCargo(cargoStorage);
            break;
        case 2: // Display All Freights
            displayFreight(freightStorage);
            break;
        case 3: // Display Matches (Original)
            displayMatches(schedule);
            break;
        case 4: // Display Schedule by Arrival Time
            schedule.displayByArrivalTime();
            break;
        case 5: // Display Schedule by Freight Capacity
            schedule.displayByFreightCapacity();
            break;
        case 6: // Display Underutilized Freights
            schedule.displayUnderutilizedFreights();
            break;
        case 7: // Display Unassigned Cargos
            schedule.displayUnassignedCargos();
            break;
        case 8: break; // Back to Main Menu
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (sub != 8);
}