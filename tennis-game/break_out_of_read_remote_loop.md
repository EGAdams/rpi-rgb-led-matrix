This is the main function that if frequently called during this tennis game.
```cpp
void GameModes::mode1() { 
    print( "just inside mode1()... " );
    _gameState->setNow( GameTimer::gameMillis());
    print( "reading undo button..." );
    _inputs.readUndoButton();
    if ( _gameState->getUndo() == 1 ) {  // undo button pressed
        _gameState->setUndo( 0 );
        _undo.mode1Undo( _history );
    }
    print( "reading _inputs.read_mcp23017_value()..." );  // digital read on GPIO pins.  
    int button_read = _inputs.read_mcp23017_value();      // sets playerButton if tripped.
    print( "read button:" << button_read );
    _serveLeds.serveSwitch(); // if serveSwitch >= 2, serveSwitch = 0; and toggle serve variable
    _logger->setName( "mode1" );
    _mode1Functions.mode1ButtonFunction(); // <--------- ENTRY POINT --------------<<
    if ( _gameState->getCurrentAction() == SLEEP_MODE ) {
        print( "not running point flash because sleep mode has been detected..." );
    } else {
        _mode1Functions.pointFlash();
    }
}
```

## read_mcp23017_value()
```c++
int Inputs::read_mcp23017_value() {
    int originalRemoteCode = _pinInterface->read_mcp23017_value();
    GameTimer::gameDelay( STEVE_DELAY ); // that delay steve was talking about...
    int freshRemoteCode = _pinInterface->read_mcp23017_value();
    if (( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON )) {  // known code and a match?
        print( "entering while reading remote codes..." );
        while(( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON )) {
            // std::cout <<  "inside while.  freshRemoteCode [" << std::to_string( freshRemoteCode ) << "]" << std::endl;
            GameTimer::gameDelay( REMOTE_READ_DELAY ); //  wait 250ms, then get a fresh one...
            // std::cout << "after delay within while reading getting fresh remote code again to verify" << std::endl;
            freshRemoteCode = _pinInterface->read_mcp23017_value();
            // std::cout << "after delay within while; freshRemoteCode [" << std::to_string( freshRemoteCode ) << "]" << std::endl;
        }
        std::cout << "exited while.  *** CODE IS VALID ***.  returning originalRemoteCode [" << std::to_string( originalRemoteCode ) << "]" << std::endl;
        return originalRemoteCode;
    } else {                        // false alarm.  we DO NOT have a matching measurement gameDelay( x ) apart.
        return UNKNOWN_REMOTE_BUTTON;
    }
 }
 ```

 

