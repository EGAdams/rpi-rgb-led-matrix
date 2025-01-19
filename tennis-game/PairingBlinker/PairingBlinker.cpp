#include "PairingBlinker.h"
#include <chrono>
#include <thread>
#include <iostream>

PairingBlinker::PairingBlinker( ScoreBoard* scoreBoard )
    : _scoreboard( scoreBoard ), _should_stop( false ), _green_player_paired( false ), _red_player_paired( false ) {
    std::cout << "PairingBlinker constructing..." << std::endl;
    if ( _scoreboard == nullptr ) { // check for null _scoreboard
        std::cerr << "Error: _scoreboard is null in PairingBlinker constructor." << std::endl;
        return;  // TODO: louder error and exit!
    }
}

PairingBlinker::~PairingBlinker() { stop(); }

const unsigned long PAIRING_TIMEOUT = 5000;
void PairingBlinker::blinkLoop() {
    bool toggle_on = true;  // Start with Green instructions
    print( "starting blink loop..." );
    unsigned long pairing_start_time = GameTimer::gameMillis();
    while ( !_should_stop ) {
        print( "in blink loop..." );
        unsigned long current_time       = GameTimer::gameMillis();
        print( "current time: " + std::to_string( current_time ));
        unsigned long elapsed_time       = current_time - pairing_start_time;
        print( "elapsed time: " + std::to_string( elapsed_time ));
        // Check if timeout exceeded
        if ( elapsed_time > PAIRING_TIMEOUT ) {
            // Switch to "blinking ball" mode
            print( "Timeout exceeded. Switch to blinking ball mode in the future..." );
            // ScoreboardBlinker blinker( _scoreboard );
            // InputWithTimer inputWithTimer( &blinker );
            // _scoreboardBlinker.start();  // Assuming `_scoreboardBlinker` is an instance of `ScoreboardBlinker`
            // int menu_selection = inputWithTimer.getInput();
            // print( "menu selection: " + std::to_string( menu_selection ));
            // // After blinking mode, reset pairing time and restart pairing instructions
            // pairing_start_time = GameTimer::gameMillis();
            // continue;
            // _should_stop = true;
            // _sleep_mode  = true;
        } else {
            print( "timeout not exceeded, continue blinking..." );
        }

        // break out of here for debugging
        // _green_player_paired = true;
        // _red_player_paired = true;
        // TODO: Put this back in to turn pairing back on.

        _scoreboard->clearScreen();
        if ( _green_player_paired && _red_player_paired ) { 
            print( "both players seem to be paired, break..." );
            break;  // If both players are paired, stop blinking
        }
        // If only the Red player is paired, show Green player instructions
        else if ( !_green_player_paired && _red_player_paired ) {
            if ( toggle_on ) {
                showGreenInstructions();
                print( "showing green instructions..." );
            } else {
                print( "show neutral text only..." );
                showPlayerPressYourRemoteText();
            }
            print( "alternating between green on and green off..." );
            toggle_on = !toggle_on;  // Alternate led on and led off
        }

        // If only the Green player is paired, show Red player instructions
        if ( _green_player_paired && !_red_player_paired ) {
            _show_green = false;
            print( "showing red instructions inside blink loop..." );
            if ( toggle_on ) {
                showRedInstructions();
                print( "showing red instructions..." );
            }
            else {
                print( "show neutral text only..." );
                showPlayerPressYourRemoteText();
            }
            print( "alternating between red on and red off..." );
            toggle_on = !toggle_on;  // Alternate led on and led off
        }


        // If neither player is paired, show Green instructions
        else if ( !_green_player_paired && !_red_player_paired ) {
            if ( toggle_on ) {
                showGreenInstructions();
                print( "showing green instructions..." );
            }
            else {
                print( "show neutral text only..." );
                showPlayerPressYourRemoteText();
            }
            print( "alternating between green on and green off..." );
            toggle_on = !toggle_on;  // Alternate led on and led off
        }
        std::this_thread::sleep_for( std::chrono::seconds( 1 )); // Delay for 1 second between switching
    }
    print( "should stop flag must be true, exiting pairing blinker blink loop..." );
}

void PairingBlinker::showGreenInstructions() {
    Color red_color( 255, 0, 0 );
    Color green_color( 0, 255, 0 );
    Color yellow_color( 255, 255, 0 );
    int left_margin = 9;
    print( "clearing screen... " );
    _scoreboard->clearScreen();
    print( "setting drawer foreground color to green..." );
    _scoreboard->setDrawerForegroundColor( green_color );
    print( "drawing Green text..." );
    _scoreboard->drawNewText( "Green", left_margin + GREEN_OFFSET, 17 );
    print( "drawing Player text..." );
    _scoreboard->drawNewText( "Player", left_margin + 0, PLAYER_TOP );
    _scoreboard->setDrawerForegroundColor( yellow_color );
    _scoreboard->drawNewText( "Press", left_margin + PRESS_OFFSET, PRESS_TOP + MIDDLE_OFFSET );
    _scoreboard->drawNewText( "Your", left_margin + YOUR_OFFSET, YOUR_TOP + MIDDLE_OFFSET);
    _scoreboard->drawNewText( "Remote", left_margin + REMOTE_OFFSET, REMOTE_TOP + MIDDLE_OFFSET );
    _scoreboard->setDrawerForegroundColor( green_color );
    _scoreboard->drawNewText( "Green", left_margin + GREEN_OFFSET, GREEN_TOP );
    _scoreboard->drawNewText( "Button", left_margin + 0, BUTTON_TOP ); 
}

void PairingBlinker::showRedInstructions() {
    Color red_color( 255, 0, 0 );
    Color green_color( 0, 255, 0 );
    Color yellow_color( 255, 255, 0 );
    if ( _scoreboard->onRaspberryPi() ) {
        int left_margin = 9;
        _scoreboard->clearScreen();
        _scoreboard->setDrawerForegroundColor( red_color );
        _scoreboard->drawNewText( "Red", left_margin + RED_OFFSET, 17 );
        _scoreboard->drawNewText( "Player", left_margin + 0, PLAYER_TOP );
        _scoreboard->setDrawerForegroundColor( yellow_color );
        _scoreboard->drawNewText( "Press", left_margin + PRESS_OFFSET, PRESS_TOP + MIDDLE_OFFSET );
        _scoreboard->drawNewText( "Your", left_margin + YOUR_OFFSET, YOUR_TOP + MIDDLE_OFFSET );
        _scoreboard->drawNewText( "Remote", left_margin + REMOTE_OFFSET, REMOTE_TOP + MIDDLE_OFFSET );
        _scoreboard->setDrawerForegroundColor( red_color );
        _scoreboard->drawNewText( "Red", left_margin + RED_OFFSET, RED_TOP );
        _scoreboard->drawNewText( "Button", left_margin + 0, BUTTON_TOP );
    }
}

void PairingBlinker::showPlayerPressYourRemoteText() {
    Color red_color( 255, 0, 0 );
    Color green_color( 0, 255, 0 );
    Color yellow_color( 255, 255, 0 );
    int left_margin = 9;
    _scoreboard->clearScreen();
    if ( _show_green ) {
        _scoreboard->setDrawerForegroundColor( green_color );
        _scoreboard->drawNewText( "Player", left_margin + 0, PLAYER_TOP );
    } else {
        _scoreboard->setDrawerForegroundColor( red_color );
        _scoreboard->drawNewText( "Player", left_margin + 0, PLAYER_TOP );
    }
    _scoreboard->setDrawerForegroundColor( yellow_color );
    _scoreboard->drawNewText( "Press", left_margin + PRESS_OFFSET, PRESS_TOP + MIDDLE_OFFSET );
    _scoreboard->drawNewText( "Your", left_margin + YOUR_OFFSET, YOUR_TOP + MIDDLE_OFFSET );
    _scoreboard->drawNewText( "Remote", left_margin + REMOTE_OFFSET, REMOTE_TOP + MIDDLE_OFFSET );
    if ( _show_green ) {
        _scoreboard->setDrawerForegroundColor( green_color );
        _scoreboard->drawNewText( "Green", left_margin + GREEN_OFFSET, GREEN_TOP );
        _scoreboard->drawNewText( "Button", left_margin + 0, BUTTON_TOP );
    } else {
        _scoreboard->setDrawerForegroundColor( red_color );
        _scoreboard->drawNewText( "Red", left_margin + RED_OFFSET, RED_TOP );
        _scoreboard->drawNewText( "Button", left_margin + 0, BUTTON_TOP );
    }
}

void PairingBlinker::start() {
    print( "starting blink thread..." );
    _should_stop = false;
    blink_thread = std::thread( &PairingBlinker::blinkLoop, this );
    print( "blink thread started..." );
}

void PairingBlinker::stop() {
    _should_stop = true;
    if ( blink_thread.joinable() ) {
        blink_thread.join();
    }
}

void PairingBlinker::enable() {            _should_stop = false; }
void PairingBlinker::sleepModeOn() {       _sleep_mode  = true;  }
void PairingBlinker::sleepModeOff() {      _sleep_mode  = false; }
bool PairingBlinker::awake() {     return !_sleep_mode;          }
void PairingBlinker::setGreenPlayerPaired( bool paired ) { _green_player_paired = paired; }
void PairingBlinker::setRedPlayerPaired(   bool paired ) { _red_player_paired = paired;   }
