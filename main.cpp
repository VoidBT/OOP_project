#include "TUI.h"
#include "CStorage.h"
#include "FStorage.h"
#include "ScheduleList.h" // Keep this, as it's the class with all the functionality now

int main() {
    CStorage cargoStorage;
    FStorage freightStorage;
    ScheduleList schedule; // Correctly using ScheduleList

    TUI ui;
    // The error C2664 here implies that ScheduleList is not fully defined due to earlier redefinition errors.
    // Once CargoGroup.h and FreightTypes.h are properly guarded, this should resolve.
    ui.run(cargoStorage, freightStorage, schedule);

    return 0;
}