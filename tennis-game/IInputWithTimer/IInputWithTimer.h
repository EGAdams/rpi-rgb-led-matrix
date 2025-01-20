#ifndef I_INPUT_WITH_TIMER_H
#define I_INPUT_WITH_TIMER_H

#include "../Blinker/Blinker.h"

class IInputWithTimer {
private:
    Blinker* _blinker;
    unsigned long _time_slept;

public:
    virtual ~IInputWithTimer()  = default;
    virtual int getInput()      = 0;
    virtual long getTimeSlept() = 0;
};

#endif // I_INPUT_WITH_TIMER_H
