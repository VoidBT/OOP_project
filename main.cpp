#include <iostream>
#include "CStorage.h"
#include "FStorage.h"
#include "ScheduleList.h"
#include "TUI.h"

int main() {
    CStorage cargoStorage;
    FStorage freightStorage;
    ScheduleList schedule;
    TUI tui;

    tui.run(cargoStorage, freightStorage, schedule);

    return 0;
    // hi
}