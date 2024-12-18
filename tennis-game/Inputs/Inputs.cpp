#include "Inputs.h"
#include "../Arduino/Arduino.h"
#include "../TennisConstants/TennisConstants.h"
#include "../RemoteCodeTranslator/RemoteCodeTranslator.h"
#include "../GameObject/GameObject.h"
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
    print( "checking private pin interface argument value inside Inputs constructor..." );
    if( pinInterface == nullptr ) {
        print( "pinInterface is null" );
        exit( 1 );
    } else {
        print( "pinInterface is not null" );
    }
    print( "checking private pin interface value inside Inputs constructor..." );
    if( _pinInterface == nullptr ) {
        print( "_pinInterface is null" );
        exit( 1 );
    } else {
        print( "_pinInterface is not null" );
    }
}

Inputs::~Inputs() { /* std::cout << "*** Inputs destructor called. ***" << std::endl; */ delete _logger; }

PinInterface* Inputs::get_pin_interface() {
    print( "checking _pinInterface value inside get_pin_interface()..." );
    if( !_pinInterface ) {
        print( "!_pinInterface" );
        exit( 1 );
    } else {
        print( "_pinInterface is seems ok.." );
    }
    print( "returning private pin interface value inside Inputs..." );
    return _pinInterface;
}

void Inputs::set_pin_interface( PinInterface* pinInterface ) { 
    print( "checking private pin interface value inside Inputs before setting..." );
    if( _pinInterface == nullptr ) {
        print( "_pinInterface is null" );
        exit( 1 );
    } else {
        print( "_pinInterface is not null" );
    }
    _pinInterface = pinInterface;
}

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
    std::cout << "rotary analog value: " << rotaryAnalogValue << std::endl;
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
    // std::cout << "\n\n\n\n\nplayer button read: " << anlgPlyrBtnVal << std::endl;
    // if ( anlgPlyrBtnVal <= 1000 ) {  // if one of the player buttons is pressed...
    //     GameTimer::gameDelay( 20 );
    //     if ( anlgPlyrBtnVal <= 50 ) {                                _gameState->setPlayerButton( 1 ); }
    //     else if ( anlgPlyrBtnVal >= 350 && anlgPlyrBtnVal <= 400 ) { _gameState->setPlayerButton( 2 ); }
    //     else if ( anlgPlyrBtnVal >= 550 && anlgPlyrBtnVal <= 650 ) { _gameState->setPlayerButton( 3 ); }
    //     else if ( anlgPlyrBtnVal >= 750 && anlgPlyrBtnVal <= 800 ) { _gameState->setPlayerButton( 4 ); }
    //     #if defined _WIN32 || defined _WIN64
    //         std::cout << "player button: " << _gameState->getPlayerButton() << std::endl;
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
    remoteDataStructure* remoteData = &remoteInitialData;
    int originalRemoteCode = this->readRemotePinArray( remoteData );
#if defined _WIN32
    // std::cout << "got original remote code: " << originalRemoteCode << std::endl;
#endif
    GameTimer::gameDelay( STEVE_DELAY ); // that delay steve was talking about...
    int freshRemoteCode = this->readRemotePinArray( remoteData );
#if defined _WIN32
    if ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON ) {
        std::cout << "got fresh remote code: " << std::to_string( freshRemoteCode ) << std::endl;
    }
#endif
    if ( ( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON ) ) {  // known code and a match?
        while ( ( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON ) ) {
#if defined _WIN32  // got a matching measurement gameDelay( x ) apart.  entering while...
            // std::cout <<  "inside while.  freshRemoteCode [" << std::to_string( freshRemoteCode ) << "]" << std::endl;
#endif
            GameTimer::gameDelay( REMOTE_READ_DELAY ); //  wait 250ms, then get a fresh one...
#if defined _WIN32
            // std::cout << "after delay within while reading getting fresh remote code again to verify" << std::endl;
#endif
            freshRemoteCode = this->readRemotePinArray( remoteData );
#if defined _WIN32
            _logger->logUpdate( " inputs [" + std::to_string( remoteData->pin_1 ) + "], " +
                                        "[" + std::to_string( remoteData->pin_2 ) + "], " +
                                        "[" + std::to_string( remoteData->pin_3 ) + "], " +
                                        "[" + std::to_string( remoteData->pin_4 ) + "]  ", "translatedRemoteCode()" );
            // std::cout << "after delay within while; freshRemoteCode [" << std::to_string( freshRemoteCode ) << "]" << std::endl;
#endif
        }
#if defined _WIN32
        std::cout << "exited while.  *** CODE IS VALID ***.  returning originalRemoteCode [" << std::to_string( originalRemoteCode ) << "]" << std::endl;
#endif
        return originalRemoteCode;
    } else { // false alarm.  we DO NOT have a matching measurement gameDelay( x ) apart.
        return UNKNOWN_REMOTE_BUTTON;
    }
}

// these libraries need to be included for the below to work.  
#include <atomic>
#include <thread>
#include <future>
// Updated read_mcp23017_value method to handle stable remote reading
int Inputs::read_mcp23017_value() {
    print( "setting originalRemoteCode..." );
    // check for null _pinInterface
    print ( "checking !_pinInterface..." );
    if ( !_pinInterface ) {
        print( "Error: _pinInterface is null" );
        return -1;
    }
    print( "checking _ppinInterface == nullptr ..." );
    if ( _pinInterface == nullptr ) {
        print( "Error: _pinInterface is null" );
        return -1;
    } else {
        print( "got _pinInterface" );
    }
    print( "getting originalRemoteCode..." );
    int originalRemoteCode = _pinInterface->read_mcp23017_value();
    print( "steve delay..." );
    GameTimer::gameDelay( STEVE_DELAY ); // that delay steve was talking about...
    print( "getting fresh remote code after steeve delay..." );
    int freshRemoteCode = _pinInterface->read_mcp23017_value();

    print( "got originalRemoteCode: " + std::to_string( originalRemoteCode ) );
    print( "got freshRemoteCode: " + std::to_string( freshRemoteCode ) );
    print( "checking for match..." );
    if ( ( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON ) ) {
        print( "Entering while loop for remote codes..." );
        while ( ( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON ) ) {
            GameTimer::gameDelay( REMOTE_READ_DELAY );
            freshRemoteCode = _pinInterface->read_mcp23017_value();
            int selection = 0; // Check for inputs from keyboard and remote
            if ( _inputQueue->dequeue( selection ) ) {
                if ( _remotePairingScreen->inPairingMode() && _is_on_pi && _pairingBlinker->awake() ) {
                    if ( selection == 7 ) {
                        _remotePairingScreen->greenPlayerPressed();
                        _pairingBlinker->setGreenPlayerPaired( true );
                    }
                    else if ( selection == 11 ) {
                        _remotePairingScreen->redPlayerPressed();
                        _pairingBlinker->setRedPlayerPaired( true );
                    }
                    else {
                        print( "Invalid selection." );
                    }
                }
                else {
                    print( "Processing input selection: " << selection );
                    _gameObject->process_selection( selection );
                }
            }
        }
        std::cout << "Exited while. *** CODE IS VALID ***. Returning originalRemoteCode [" << originalRemoteCode << "]" << std::endl;
        return originalRemoteCode;
    }
    else {
        return UNKNOWN_REMOTE_BUTTON;
    }
}

void Inputs::setInputQueue( ThreadSafeQueue<int>* inputQueue ) {
    _inputQueue = inputQueue;
}

ThreadSafeQueue<int>* Inputs::getInputQueue() const {
    return _inputQueue;
}

void Inputs::setRemotePairingScreen( RemotePairingScreen* remotePairingScreen ) {
    _remotePairingScreen = remotePairingScreen;
}

RemotePairingScreen* Inputs::getRemotePairingScreen() const {
    return _remotePairingScreen;
}

void Inputs::setPairingBlinker( PairingBlinker* pairingBlinker ) {
    _pairingBlinker = pairingBlinker;
}

PairingBlinker* Inputs::getPairingBlinker() const {
    return _pairingBlinker;
}

void Inputs::set_is_on_pi( bool* is_on_pi ) {
    _is_on_pi = is_on_pi;
}

bool* Inputs::get_is_on_pi() const {
    return _is_on_pi;
}

void Inputs::setGameObject( GameObject* gameObject ) {
    _gameObject = gameObject;
}

GameObject* Inputs::getGameObject() const {
    return _gameObject;
}
