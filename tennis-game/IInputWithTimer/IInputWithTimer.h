#ifndef I_INPUT_WITH_TIMER_H
#define I_INPUT_WITH_TIMER_H

#include "../Blinker/Blinker.h"

class IInputWithTimer {
protected:
    Blinker* _blinker;
    unsigned long _timeout_ms;  // Timeout duration in milliseconds
    unsigned long _time_slept;  // Time slept in milliseconds

    IInputWithTimer(Blinker* blinker, unsigned long timeout_ms)
        : _blinker(blinker), _timeout_ms(timeout_ms), _time_slept(0) {}

public:
    virtual ~IInputWithTimer() = default;
    // Pure virtual methods to enforce implementation in derived classes
    virtual int getInput() = 0;
    unsigned long getTimeSlept() const;
    void setTimeout(unsigned long timeout);
};

#endif // I_INPUT_WITH_TIMER_H