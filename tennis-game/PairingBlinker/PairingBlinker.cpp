#include "PairingBlinker.h"
#include <chrono>
#include <thread>
#include <iostream>

PairingBlinker::PairingBlinker(ScoreBoard* scoreBoard)
    : _scoreboard(scoreBoard), should_stop(false), green_player_paired(false), red_player_paired(false) {}

PairingBlinker::~PairingBlinker() {
    stop();
}

void PairingBlinker::blinkLoop() {
    bool show_green = true;  // Start with Green instructions
    
    while (!should_stop) {
        _scoreboard->clearScreen();

        // If both players are paired, stop blinking
        if (green_player_paired && red_player_paired) {
            break;
        }

        // If only the Green player is paired, show Red player instructions
        if (green_player_paired && !red_player_paired) {
            showRedInstructions();
        }
        // If only the Red player is paired, show Green player instructions
        else if (!green_player_paired && red_player_paired) {
            showGreenInstructions();
        }
        // If neither player is paired, alternate between Green and Red instructions
        else if (!green_player_paired && !red_player_paired) {
            if (show_green) {
                showGreenInstructions();
            } else {
                showRedInstructions();
            }
            show_green = !show_green;  // Alternate between Green and Red
        }

        // Delay for 1 second between switching
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void PairingBlinker::showGreenInstructions() {
    int left_margin = 8;
    _scoreboard->clearScreen();
    _scoreboard->drawText( "Green",   GREEN, left_margin + 1, 17  );
    _scoreboard->drawText( "Player",  GREEN, left_margin + 0, 34  );
    _scoreboard->drawText( "Press",   GREEN, left_margin + 1, 51  );
    _scoreboard->drawText( "Your",    GREEN, left_margin + 2, 68  );
    _scoreboard->drawText( "Remote",  GREEN, left_margin + -8, 85 );
    _scoreboard->drawText( "Green",   GREEN, left_margin + 1, 102 );
    _scoreboard->drawText( "Button",  GREEN, left_margin + 0, 119 );
}

void PairingBlinker::showRedInstructions() {
    if( _scoreboard->onRaspberryPi()) {
         int left_margin = 8;
        _scoreboard->clearScreen();
        _scoreboard->drawText( "Red",   RED, left_margin + 4, 17  );
        _scoreboard->drawText( "Player",  RED, left_margin + 0, 34  );
        _scoreboard->drawText( "Press",   RED, left_margin + 1, 51  );
        _scoreboard->drawText( "Your",    RED, left_margin + 2, 68  );
        _scoreboard->drawText( "Remote",  RED, left_margin + -8, 85 );
        _scoreboard->drawText( "Red",   RED, left_margin + 4, 102 );
        _scoreboard->drawText( "Button",  RED, left_margin + 0, 119 );
    }
}

void PairingBlinker::start() {
    should_stop = false;
    blink_thread = std::thread(&PairingBlinker::blinkLoop, this);
}

void PairingBlinker::stop() {
    should_stop = true;
    if (blink_thread.joinable()) {
        blink_thread.join();
    }
}

// Methods to set the pairing status of players
void PairingBlinker::setGreenPlayerPaired(bool paired) {
    green_player_paired = paired;
}

void PairingBlinker::setRedPlayerPaired(bool paired) {
    red_player_paired = paired;
}
