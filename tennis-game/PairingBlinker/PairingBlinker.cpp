#include "PairingBlinker.h"
#include <chrono>
#include <thread>
#include <iostream>

PairingBlinker::PairingBlinker( ScoreBoard* scoreBoard )
    : _scoreboard( scoreBoard ), _should_stop( false ), green_player_paired( false ), red_player_paired( false ) {
    std::cout << "PairingBlinker constructing..." << std::endl;
    if ( _scoreboard == nullptr ) { // check for null _scoreboard
        std::cerr << "Error: _scoreboard is null in PairingBlinker constructor." << std::endl;
        return;  // TODO: louder error and exit!
    }
}

PairingBlinker::~PairingBlinker() { stop(); }

void PairingBlinker::blinkLoop() {
    bool toggle_on = true;  // Start with Green instructions
    print( "starting blink loop..." );
    while ( !_should_stop ) {
        print( "in blink loop..." );
        _scoreboard->clearScreen();
        if ( green_player_paired && red_player_paired ) { 
            print( "both players seem to be paired, break..." );
            break;  // If both players are paired, stop blinking
        }
        // If only the Red player is paired, show Green player instructions
        else if ( !green_player_paired && red_player_paired ) {
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

        // If only the Green player is paired, show Red player instructions
        if ( green_player_paired && !red_player_paired ) {
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
        else if ( !green_player_paired && !red_player_paired ) {
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
    print( "should stop flag must be true, exiting blink loop..." );
}

void PairingBlinker::showGreenInstructions() {
    Color red_color( 255, 0, 0 );
    Color green_color( 0, 255, 0 );
    Color yellow_color( 255, 255, 0 );
    int left_margin = 9;
    print( "clearing screen... " );
    _scoreboard->clearScreen();
    print( "drawing Green text..." );
    _scoreboard->setDrawerForegroundColor( green_color );
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
}

void PairingBlinker::stop() {
    _should_stop = true;
    if ( blink_thread.joinable() ) {
        blink_thread.join();
    }
}

void PairingBlinker::enable() {
    _should_stop = false;
}

void PairingBlinker::setGreenPlayerPaired( bool paired ) { green_player_paired = paired; }
void PairingBlinker::setRedPlayerPaired(   bool paired ) { red_player_paired = paired;   }
