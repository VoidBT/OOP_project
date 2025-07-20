#include "TUI.h"
#include "CStorage.h"
#include "FStorage.h"
#include "ScheduleList.h" // Keep this, as it's the class with all the functionality now

int main() {
    CStorage cargoStorage;
    FStorage freightStorage;
    ScheduleList schedule; // Correctly using ScheduleList

    TUI ui;
    ui.run(cargoStorage, freightStorage, schedule);

    return 0;
}