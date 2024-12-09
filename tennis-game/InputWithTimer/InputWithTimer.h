#ifndef INPUT_WITH_TIMER_H
#define INPUT_WITH_TIMER_H

#include "../Blinker/Blinker.h"
#include "../GameTimer/GameTimer.h"
#include "../Inputs/Inputs.h"

class Blinker;  // Forward declaration

class InputWithTimer {
private:
    Blinker* _blinker;
    unsigned long _time_slept;
    Inputs* _inputs;

public:
    InputWithTimer( Blinker* blinker, Inputs* inputs );
    ~InputWithTimer();

    int getInput();
    unsigned long getTimeSlept();
};

#endif // INPUT_WITH_TIMER_H
