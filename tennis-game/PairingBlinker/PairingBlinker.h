#ifndef PAIRING_BLINKER_H
#define PAIRING_BLINKER_H

#include "../ScoreBoard/ScoreBoard.h"
#include "../Blinker/Blinker.h"
#include <thread>

class PairingBlinker : public Blinker {
public:
    PairingBlinker(ScoreBoard* scoreBoard);
    ~PairingBlinker();
    
    void start() override;
    void stop()  override;

private:
    void blinkLoop();         // Alternates between Green and Red instructions
    void showGreenInstructions();
    void showRedInstructions();
    
    ScoreBoard* _scoreboard;
    bool should_stop;
    std::thread blink_thread;
};

#endif
