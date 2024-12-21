#include "Inputs.h"
#include "../Arduino/Arduino.h"
#include "../TennisConstants/TennisConstants.h"
#include "../RemoteCodeTranslator/RemoteCodeTranslator.h"
#include <string>
#include <iostream>

Inputs::Inputs( Player* player1,
    Player* player2,
    PinInterface* pinInterface,
    GameState* gameState )
    : _player1( player1 ),
    _player2( player2 ),
    _pinInterface( pinInterface ),
    _gameState( gameState ),
    _reset( player1, player2, pinInterface, gameState ) {
    _logger = new Logger( "Inputs" );
    _need_jump_start = true;
}

Inputs::~Inputs() {
    // print("*** Inputs destructor called. ***" );
    delete _logger; }

void Inputs::readReset() {
    if ( _pinInterface->pinDigitalRead( RESET ) == LOW ) {
        if ( SIMULATION == 0 ) { while ( _pinInterface->pinDigitalRead( RESET ) == LOW ) { GameTimer::gameDelay( 25 ); }}  // Wait for the button to be released
        _reset.resetScoreboard(); }}

void Inputs::readUndoButton() {
    return; // DISABLED
    if ( _pinInterface->pinDigitalRead( UNDO ) == LOW ) {
        if ( SIMULATION == 0 ) {
            while ( _pinInterface->pinDigitalRead( UNDO ) == LOW ) {
                GameTimer::gameDelay( 25 );
            }
        } // Wait for the button to be released
        _gameState->setUndo( 1 );
    }
}

int Inputs::readRotary() {  // TODO: make this one read.
    _gameState->setRotaryPosition( 0 );  // int rotaryPosition = 0;
    int rotaryAnalogValue = _pinInterface->pinAnalogRead( ROTARY );
    #if defined _WIN32 || defined _WIN64
        print("rotary analog value: " << rotaryAnalogValue );
    #endif
    if ( rotaryAnalogValue <= 100 ) { _gameState->setRotaryPosition( 1 ); }
    if ( rotaryAnalogValue >= 350 && rotaryAnalogValue <= 450 ) { _gameState->setRotaryPosition( 2 ); }
    if ( rotaryAnalogValue >= 550 && rotaryAnalogValue <= 700 ) { _gameState->setRotaryPosition( 3 ); }
    if ( rotaryAnalogValue >= 750 && rotaryAnalogValue <= 800 ) { _gameState->setRotaryPosition( 4 ); }
    if ( rotaryAnalogValue >= 850 && rotaryAnalogValue <= 1000 ) { _gameState->setRotaryPosition( 5 ); }
    if ( _gameState->getRotaryPosition() != _gameState->getPrevRotaryPosition() ) {
        _gameState->setRotaryChange( 1 );
        _gameState->setPrevRotaryPosition( _gameState->getRotaryPosition() );
        _reset.refresh();  // set rotaryChange to false and reset the scoreboard
    }
    return _gameState->getRotaryPosition(); }

// void Inputs::readPlayerButtons() {
    // return; // DISABLED
    // int anlgPlyrBtnVal = _pinInterface->pinAnalogRead( PLAYER_BUTTONS );
    // print("\n\n\n\n\nplayer button read: " << anlgPlyrBtnVal );
    // if ( anlgPlyrBtnVal <= 1000 ) {  // if one of the player buttons is pressed...
    //     GameTimer::gameDelay( 20 );
    //     if ( anlgPlyrBtnVal <= 50 ) {                                _gameState->setPlayerButton( 1 ); }
    //     else if ( anlgPlyrBtnVal >= 350 && anlgPlyrBtnVal <= 400 ) { _gameState->setPlayerButton( 2 ); }
    //     else if ( anlgPlyrBtnVal >= 550 && anlgPlyrBtnVal <= 650 ) { _gameState->setPlayerButton( 3 ); }
    //     else if ( anlgPlyrBtnVal >= 750 && anlgPlyrBtnVal <= 800 ) { _gameState->setPlayerButton( 4 ); }
    //     #if defined _WIN32 || defined _WIN64
    //         print("player button: " << _gameState->getPlayerButton() );
    //         // _logger->logUpdate( "set player button to [" + std::to_string( _gameState->getPlayerButton()) + "]" , __FUNCTION__ );
    //     #else
    //         while ( _pinInterface->pinAnalogRead( PLAYER_BUTTONS ) <= 1000 ) { GameTimer::gameDelay( 20 ); }
    //     #endif
    // }    
 // }

int Inputs::readRemotePinArray( remoteDataStructure* remoteData ) {
    #if defined _WIN32
    remoteData->pin_1 = _pinInterface->pinAnalogRead( REMOTE_DATA_0 );
    remoteData->pin_2 = _pinInterface->pinAnalogRead( REMOTE_DATA_1 );
    remoteData->pin_3 = _pinInterface->pinAnalogRead( REMOTE_DATA_2 );
    remoteData->pin_4 = _pinInterface->pinAnalogRead( REMOTE_DATA_3 );
    #else 
    remoteData->pin_1 = _pinInterface->pinDigitalRead( REMOTE_DATA_0 );
    remoteData->pin_2 = _pinInterface->pinDigitalRead( REMOTE_DATA_1 );
    remoteData->pin_3 = _pinInterface->pinDigitalRead( REMOTE_DATA_2 );
    remoteData->pin_4 = _pinInterface->pinDigitalRead( REMOTE_DATA_3 );
    #endif
    // return _remoteCodeTranslator->translateRemoteCode( *remoteData );
    return 1; // above causes compile time error.  must move on!
}

int Inputs::readPlayerButtons() {
    remoteDataStructure remoteInitialData = { 0, 0, 0, 0 };
    remoteDataStructure *remoteData = &remoteInitialData;
    int originalRemoteCode = this->readRemotePinArray( remoteData );
    #if defined _WIN32
    // print("got original remote code: " << originalRemoteCode );
    #endif
    GameTimer::gameDelay( STEVE_DELAY ); // that delay steve was talking about...
    int freshRemoteCode = this->readRemotePinArray( remoteData );
    #if defined _WIN32
    if ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON ) {
        print("got fresh remote code: " << std::to_string( freshRemoteCode ) );
    }
    #endif
    if (( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON )) {  // known code and a match?
        while(( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON )) {
            #if defined _WIN32  // got a matching measurement gameDelay( x ) apart.  entering while...
            print( "inside while.  freshRemoteCode [" << std::to_string( freshRemoteCode ) << "]" );
            #endif
            GameTimer::gameDelay( REMOTE_READ_DELAY ); //  wait 250ms, then get a fresh one...
            #if defined _WIN32
            print("after delay within while reading getting fresh remote code again to verify" );
            #endif
            freshRemoteCode = this->readRemotePinArray( remoteData );
            #if defined _WIN32
            _logger->logUpdate( " inputs [" + std::to_string( remoteData->pin_1 ) + "], " +
                                        "[" + std::to_string( remoteData->pin_2 ) + "], " +
                                        "[" + std::to_string( remoteData->pin_3 ) + "], " +
                                        "[" + std::to_string( remoteData->pin_4 ) + "]  ", "translatedRemoteCode()" ); 
            print("after delay within while; freshRemoteCode [" << std::to_string( freshRemoteCode ) << "]" );
            #endif
        }
        #if defined _WIN32
        print("exited while.  *** CODE IS VALID ***.  returning originalRemoteCode [" << std::to_string( originalRemoteCode ) << "]" );
        #endif
        return originalRemoteCode;
    } else { // false alarm.  we DO NOT have a matching measurement gameDelay( x ) apart.
        return UNKNOWN_REMOTE_BUTTON;
    }
 }

 int Inputs::read_mcp23017_value() {
    int originalRemoteCode = _pinInterface->read_mcp23017_value();
    print( "originalRemoteCode [" + std::to_string( originalRemoteCode ) + "]" );
    GameTimer::gameDelay( STEVE_DELAY ); // that delay steve was talking about...
    int freshRemoteCode = _pinInterface->read_mcp23017_value();
    print( "freshRemoteCode after steve delay [" + std::to_string( freshRemoteCode ) + "]" );
    if (( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON )) {  // known code and a match?
        while(( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON )) {
            print( "inside while.  originalRemoteCode [" << std::to_string( originalRemoteCode ) << "]" );
            print( "inside while.  freshRemoteCode [" << std::to_string( freshRemoteCode ) << "]" );
            GameTimer::gameDelay( REMOTE_READ_DELAY ); //  wait 250ms, then get a fresh one...
            print("after delay within while reading getting fresh remote code again to verify" );
            freshRemoteCode = _pinInterface->read_mcp23017_value();
            print("after delay within while; freshRemoteCode [" << std::to_string( freshRemoteCode ) << "]" );
            if ( _need_jump_start ) { _need_jump_start = false; originalRemoteCode = 7; }
        }
        print("exited while.  *** CODE IS VALID ***.  returning originalRemoteCode [" << std::to_string( originalRemoteCode ) << "]" );
        
        return originalRemoteCode;
    } else {                        // false alarm.  we DO NOT have a matching measurement gameDelay( x ) apart.
        return UNKNOWN_REMOTE_BUTTON;
    }
 }