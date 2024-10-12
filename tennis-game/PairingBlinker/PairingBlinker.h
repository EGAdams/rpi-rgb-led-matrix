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
    void stop() override;
    void setGreenPlayerPaired(bool paired);  // New method to track Green player status
    void setRedPlayerPaired(bool paired);    // New method to track Red player status
    void enable();

private:
    void blinkLoop();         // Alternates between Green and Red instructions
    void showGreenInstructions();
    void showPlayerPressYourRemoteText();
    void showRedInstructions();
    
    ScoreBoard* _scoreboard;
    bool        _should_stop;
    std::thread blink_thread;

    // New state tracking
    bool green_player_paired;
    bool red_player_paired;
};

#endif
