#include "TUI.h"
#include "CStorage.h"
#include "FStorage.h"
#include "EnhancedScheduleList.h"

int main() {
    CStorage cargoStorage;
    FStorage freightStorage;
    EnhancedScheduleList schedule;

    TUI ui;
    ui.run(cargoStorage, freightStorage, schedule);

    return 0;
}