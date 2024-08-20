#include "InputWithTimer.h"
#include <iostream>


InputWithTimer::InputWithTimer( ScoreboardBlinker* blinker) : _blinker( blinker ) {}
InputWithTimer::~InputWithTimer() { delete _blinker; }

int InputWithTimer::getInput() {
    int menu_selection;
    _blinker->start();
    std::cin >> menu_selection;
    _blinker->stop();
    return menu_selection;
}

