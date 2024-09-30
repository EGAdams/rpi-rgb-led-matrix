#ifndef INPUT_WITH_TIMER_H
#define INPUT_WITH_TIMER_H

#include "../TennisConstants/TennisConstants.h"
#include "../Blinker/Blinker.h"
#include "../GameTimer/GameTimer.h"

class Blinker;  // Forward declaration

class InputWithTimer {
private:
    Blinker* _blinker;
    unsigned long _time_slept;

public:
    InputWithTimer( Blinker* blinker );
    ~InputWithTimer();

    int getInput();
    unsigned long getTimeSlept();
};

#endif // INPUT_WITH_TIMER_H
