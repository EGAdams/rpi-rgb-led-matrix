#ifndef I_INPUT_WITH_TIMER_H
#define I_INPUT_WITH_TIMER_H

#include "../Blinker/Blinker.h"
#include "../GameTimer/GameTimer.h"
#include "../Inputs/Inputs.h"

class Blinker;  // Forward declaration

class IInputWithTimer {
private:
    Blinker* _blinker;
    unsigned long _time_slept;
    Inputs* _inputs;

public:
    virtual ~IInputWithTimer() = default;
    virtual int getInput() = 0;
    virtual IInputWithTimer( Blinker* blinker, Inputs* inputs ) = 0;
    ~IInputWithTimer();

    int getInput();
    unsigned long getTimeSlept();
};

#endif // I_INPUT_WITH_TIMER_H
