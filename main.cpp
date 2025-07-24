#include <iostream>
#include "CStorage.h"
#include "FStorage.h"
#include "ScheduleList.h"
#include "TUI.h"

int main() {
    CStorage cargoStorage;
    FStorage freightStorage;
	Match matches;
    ScheduleList schedule;
    TUI tui;

    tui.run(cargoStorage, freightStorage, matches, schedule);

    return 0;
}