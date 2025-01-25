#ifndef BLANK_BLINKER_H
#define BLANK_BLINKER_H

#include "../Blinker/Blinker.h"

class BlankBlinker : public Blinker {
public:
    void start() override {
        // No operation
    }

    void stop() override {
        // No operation
    }
};

#endif
