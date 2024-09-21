#include "InputWithTimer.h"
#include <iostream>


InputWithTimer::InputWithTimer( Blinker* blinker) : _blinker( blinker ) {}
InputWithTimer::~InputWithTimer() {}

int InputWithTimer::getInput() {
    // Mark start time with game timer
    unsigned long sleep_start = GameTimer::gameMillis();
    int menu_selection;
    _blinker->start();
    std::cin >> menu_selection;
    _blinker->stop();
    // Mark end time with game timer
    unsigned long sleep_end = GameTimer::gameMillis();
    _time_slept = sleep_end - sleep_start;
    return menu_selection;
}

unsigned long InputWithTimer::getTimeSlept() {
    return _time_slept;
}

