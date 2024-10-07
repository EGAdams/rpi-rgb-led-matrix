#include "PairingBlinker.h"
#include <chrono>
#include <thread>
#include <iostream>

PairingBlinker::PairingBlinker( ScoreBoard* scoreBoard )
    : _scoreboard( scoreBoard ), should_stop( false ), green_player_paired( false ), red_player_paired( false ) {
    std::cout << "PairingBlinker constructing..." << std::endl;
    if ( _scoreboard == nullptr ) { // check for null _scoreboard
        std::cerr << "Error: _scoreboard is null in PairingBlinker constructor." << std::endl;
        return;  // TODO: louder error and exit!
    }
}

PairingBlinker::~PairingBlinker() { stop(); }

void PairingBlinker::blinkLoop() {
    bool show_green = true;  // Start with Green instructions
    print( "starting blink loop..." );
    while ( !should_stop ) {
        print( "in blink loop..." );
        _scoreboard->clearScreen();
        if ( green_player_paired && red_player_paired ) { 
            print( "both players seem to be paired, break..." );
            break;  // If both players are paired, stop blinking
        }
        // If only the Green player is paired, show Red player instructions
        if ( green_player_paired && !red_player_paired ) { 
            print( "showing red instructions inside blink loop..." );
            showRedInstructions();
        }

        // If only the Red player is paired, show Green player instructions
        else if ( !green_player_paired && red_player_paired ) {
            print( "showing green instructions inside blink loop..." );
            showGreenInstructions(); 
        }

        // If neither player is paired, alternate between Green and Red instructions
        else if ( !green_player_paired && !red_player_paired ) {
            if ( show_green ) {
                showGreenInstructions();
                print( "showing green instructions..." );
            }
            else {
                print( "showing red instructions..." );
                showRedInstructions();
            }
            print( "alternating between green and red..." );
            show_green = !show_green;  // Alternate between Green and Red
        }
        std::this_thread::sleep_for( std::chrono::seconds( 1 )); // Delay for 1 second between switching
    }
    print( "should stop flag must be true, exiting blink loop..." );
}

void PairingBlinker::showGreenInstructions() {
    int left_margin = 8;
    print( "clearing screen..." );
    _scoreboard->clearScreen();
    print( "drawing Green text..." );
    _scoreboard->drawText( "Green", left_margin + 1, 17 );
    print( "drawing Player text..." );
    _scoreboard->drawText( "Player", left_margin + 0, 34 );
    _scoreboard->drawText( "Press", left_margin + 1, 51 );
    _scoreboard->drawText( "Your", left_margin + 2, 68 );
    _scoreboard->drawText( "Remote", left_margin + -8, 85 );
    _scoreboard->drawText( "Green", left_margin + 1, 102 );
    _scoreboard->drawText( "Button", left_margin + 0, 119 );
}

void PairingBlinker::showRedInstructions() {
    if ( _scoreboard->onRaspberryPi() ) {
        int left_margin = 8;
        _scoreboard->clearScreen();
        _scoreboard->drawText( "Red", left_margin + 4, 17 );
        _scoreboard->drawText( "Player", left_margin + 0, 34 );
        _scoreboard->drawText( "Press", left_margin + 1, 51 );
        _scoreboard->drawText( "Your", left_margin + 2, 68 );
        _scoreboard->drawText( "Remote", left_margin + -8, 85 );
        _scoreboard->drawText( "Red", left_margin + 4, 102 );
        _scoreboard->drawText( "Button", left_margin + 0, 119 );
    }
}

void PairingBlinker::start() {
    should_stop = false;
    blink_thread = std::thread( &PairingBlinker::blinkLoop, this );
}

void PairingBlinker::stop() {
    should_stop = true;
    if ( blink_thread.joinable() ) {
        blink_thread.join();
    }
}

void PairingBlinker::setGreenPlayerPaired( bool paired ) { green_player_paired = paired; }
void PairingBlinker::setRedPlayerPaired(   bool paired ) { red_player_paired = paired;   }
