#include "InputWithTimer.h"
#include <iostream>

int InputWithTimer::getInput() {
    int menu_selection;
    blinker.start();

    std::cin >> menu_selection;

    blinker.stop();
    return menu_selection;
}