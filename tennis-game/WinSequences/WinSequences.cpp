#include "WinSequences.h"

Mode1WinSequences::~Mode1WinSequences() {}
Mode1WinSequences::Mode1WinSequences( Player* player1,
    Player* player2,
    PinInterface* pinInterface,
    GameState* gameState )
    : _player1( player1 ),
    _player2( player2 ),
    _pinInterface( pinInterface ),
    _gameState( gameState ),
    _pointLeds( player1, player2, pinInterface ),
    _gameLeds( player1, player2, pinInterface ),
    _setLeds( player1, player2, pinInterface ),
    _reset( player1, player2, pinInterface, gameState ),
    _undo( player1, player2, pinInterface, gameState ) {}

void Mode1WinSequences::p1GameWinSequence() {
    _undo.memory();
    int pulse_count = _gameState->getGameWinPulseCount();
    for ( int currentPulseCount = 0; currentPulseCount < pulse_count; currentPulseCount++ ) {
        _player1->setGames( 99 );
        _gameLeds.updateGames();
        GameTimer::gameDelay( _gameState->getGameFlashDelay() );
        _player1->setGames( _gameState->getP1GamesMem() );
        _gameLeds.updateGames();
        GameTimer::gameDelay( _gameState->getGameFlashDelay() );
    }
    _player1->setPoints( 0 );
    _player2->setPoints( 0 );
}

/*********************************************************************************************/

void Mode1WinSequences::p1SetWinSequence() {
    _undo.memory();  // Memory();

    for ( int currentPulseCount = 0;
        currentPulseCount < SET_WIN_PULSE_COUNT;
        currentPulseCount++ ) {
        // _player1->setGames( 99 );   //  p1Games = 99;
        // _player2->setGames( 99 );   //  p2Games = 99;
        _player1->setSets( 0 );
        // _gameLeds.updateGames();  //  UpdateGames();
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());

        _player1->setSets(
            _gameState->getP1SetsMem() );
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
    }
    _player1->setGames( 0 );
    _player2->setGames( 0 );
    _gameLeds.updateGames();  // UpdateGames();
    _setLeds.updateSets();
}

void Mode1WinSequences::p1MatchWinSequence() {
    _undo.memory();
    _pointLeds.updateTBPoints();
    _player2->setGames( 99 );
    _gameLeds.updateGames();

    MatchWinSequence matchWinSequence; matchWinSequence.run( _player1, _gameState, &_gameLeds, &_setLeds );

    // for ( int currentPulseCount = 0; currentPulseCount < _gameState->getMatchWinPulseCount(); currentPulseCount++ ) {
    //     _pinInterface->pinDigitalWrite( P1_GAMES_LED0, HIGH );
    //     _pinInterface->pinDigitalWrite( P1_GAMES_LED1, HIGH );
    //     _pinInterface->pinDigitalWrite( P1_GAMES_LED2, HIGH );
    //     _pinInterface->pinDigitalWrite( P1_GAMES_LED3, HIGH );
    //     _pinInterface->pinDigitalWrite( P1_GAMES_LED4, HIGH );
    //     _pinInterface->pinDigitalWrite( P1_GAMES_LED5, HIGH );
    //     _pinInterface->pinDigitalWrite( P1_GAMES_LED6, HIGH );
    //     // _pinInterface->pinDigitalWrite( P1_TIEBREAKER, HIGH );

    //     _pinInterface->pinDigitalWrite( P1_SETS_LED1, HIGH );
    //     _pinInterface->pinDigitalWrite( P1_SETS_LED2, HIGH );
    //     GameTimer::gameDelay( _gameState->getFlashDelay());

    //     _pinInterface->pinDigitalWrite( P1_GAMES_LED0, LOW );
    //     _pinInterface->pinDigitalWrite( P1_GAMES_LED1, LOW );
    //     _pinInterface->pinDigitalWrite( P1_GAMES_LED2, LOW );
    //     _pinInterface->pinDigitalWrite( P1_GAMES_LED3, LOW );
    //     _pinInterface->pinDigitalWrite( P1_GAMES_LED4, LOW );
    //     _pinInterface->pinDigitalWrite( P1_GAMES_LED5, LOW );
    //     _pinInterface->pinDigitalWrite( P1_GAMES_LED6, LOW );
    //     // _pinInterface->pinDigitalWrite( P1_TIEBREAKER, LOW );

    //     _pinInterface->pinDigitalWrite( P1_SETS_LED1, LOW );
    //     _pinInterface->pinDigitalWrite( P1_SETS_LED2, LOW );
    //     GameTimer::gameDelay( _gameState->getFlashDelay());
    // }
    _reset.resetScoreboard();
}

/*********************************************************************************************/

void Mode1WinSequences::p2GameWinSequence() {
    _undo.memory();  // Memory();
    for ( int currentPulseCount = 0;
        currentPulseCount < _gameState->getGameWinPulseCount();
        currentPulseCount++ ) {
        _player2->setGames( 99 );   // p2Games = 99;
        _gameLeds.updateGames();  // UpdateGames();
        GameTimer::gameDelay( _gameState->getGameFlashDelay());

        _player2->setGames( _gameState->getP2GamesMem() );  // p2Games = p2GamesMem;
        _gameLeds.updateGames();                          // UpdateGames();
        GameTimer::gameDelay( _gameState->getGameFlashDelay());
    }
    _player1->setPoints( 0 );
    _player2->setPoints( 0 );
}

void Mode1WinSequences::p2SetWinSequence() {
    _undo.memory();  // Memory();
    for ( int currentPulseCount = 0;
        currentPulseCount < SET_WIN_PULSE_COUNT;
        currentPulseCount++ ) {
        // _player1->setGames( 99 );  // p1Games = 99;
        // _player2->setGames( 99 );  //    p2Games = 99;
        _player2->setSets( 0 ); /* p2Sets = */  // 0);
        // _gameLeds.updateGames();  //    UpdateGames();
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());

        // _player1->setGames( _gameState->getP1GamesMem() ); // p1Games =
        // p1GamesMem; _player2->setGames( _gameState->getP2GamesMem() ); // p2Games
        // = p2GamesMem;
        _player2->setSets( _gameState->getP2SetsMem() );
        // _gameLeds.updateGames();                           // UpdateGames();
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
    }
    _player1->setGames( 0 );
    _player2->setGames( 0 );
    _gameLeds.updateGames();  // UpdateGames();
    _setLeds.updateSets();
}

void Mode1WinSequences::p2MatchWinSequence() {
    _undo.memory();               // Memory();
    _pointLeds.updateTBPoints();  // UpdateTBPoints();
    _player1->setGames( 99 );
    _gameLeds.updateGames();      // UpdateGames();

    MatchWinSequence matchWinSequence; matchWinSequence.run( _player2, _gameState, &_gameLeds, &_setLeds );

    // for ( int currentPulseCount = 0;
    //     currentPulseCount < _gameState->getMatchWinPulseCount();
    //     currentPulseCount++ ) {
    //     _pinInterface->pinDigitalWrite( P2_GAMES_LED0, HIGH );
    //     _pinInterface->pinDigitalWrite( P2_GAMES_LED1, HIGH );
    //     _pinInterface->pinDigitalWrite( P2_GAMES_LED2, HIGH );
    //     _pinInterface->pinDigitalWrite( P2_GAMES_LED3, HIGH );
    //     _pinInterface->pinDigitalWrite( P2_GAMES_LED4, HIGH );
    //     _pinInterface->pinDigitalWrite( P2_GAMES_LED5, HIGH );
    //     _pinInterface->pinDigitalWrite( P2_GAMES_LED6, HIGH );
    //     // _pinInterface->pinDigitalWrite( P2_TIEBREAKER, HIGH );

    //     _pinInterface->pinDigitalWrite( P2_SETS_LED1, HIGH );
    //     _pinInterface->pinDigitalWrite( P2_SETS_LED2, HIGH );
    //     GameTimer::gameDelay( _gameState->getFlashDelay() );

    //     _pinInterface->pinDigitalWrite( P2_GAMES_LED0, LOW );
    //     _pinInterface->pinDigitalWrite( P2_GAMES_LED1, LOW );
    //     _pinInterface->pinDigitalWrite( P2_GAMES_LED2, LOW );
    //     _pinInterface->pinDigitalWrite( P2_GAMES_LED3, LOW );
    //     _pinInterface->pinDigitalWrite( P2_GAMES_LED4, LOW );
    //     _pinInterface->pinDigitalWrite( P2_GAMES_LED5, LOW );
    //     _pinInterface->pinDigitalWrite( P2_GAMES_LED6, LOW );
    //     // _pinInterface->pinDigitalWrite( P2_TIEBREAKER, LOW );

    //     _pinInterface->pinDigitalWrite( P2_SETS_LED1, LOW );
    //     _pinInterface->pinDigitalWrite( P2_SETS_LED2, LOW );
    //     GameTimer::gameDelay( _gameState->getFlashDelay());
    // }
    _reset.resetScoreboard();
    // Serial.println("End P2 Match Win Sequence");
}

void Mode1WinSequences::p1TBGameWinSequence() {
    _undo.memory();  // Memory();

    for ( int currentPulseCount = 0;
        currentPulseCount < _gameState->getGameWinPulseCount();
        currentPulseCount++ ) {
        //    p1Games = 99;
        //    p2Games = 99;
        _player1->setSets( 0 );
        //    TieLEDsOff();
        //    UpdateGames();
        _setLeds.updateSets();
        GameTimer::gameDelay(
            _gameState
            ->getFlashDelay());  // delay(
        // _gameState->getFlashDelay()
        // );

//    p1Games = p1GamesMem;
//    p2Games = p2GamesMem;
        _player1->setSets(
            _gameState->getP1SetsMem() );
        // tieLEDsOn();
        // _gameLeds.updateGames();       // UpdateGames();
        _setLeds.updateSets();
        GameTimer::gameDelay(
            _gameState
            ->getFlashDelay());  // delay(
        // _gameState->getFlashDelay()
        // );
    }
    tieLEDsOff();
    _gameState->setTieBreak( 0 );
    _player1->setGames( 0 );
    _player2->setGames( 0 );
    _gameLeds.updateGames();
}

void Mode1WinSequences::p2TBGameWinSequence() {
    _undo.memory();  // Memory();

    for ( int currentPulseCount = 0;
        currentPulseCount < _gameState->getGameWinPulseCount();
        currentPulseCount++ ) {
        //    p1Games = 99;
        //    p2Games = 99;
        _player2->setSets( 0 );
        //    TieLEDsOff();
        //    UpdateGames();
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());

//    p1Games = p1GamesMem;
//    p2Games = p2GamesMem;
        _player2->setSets(
            _gameState->getP2SetsMem() /* p2SetsMem */ );  //  p2Sets = p2SetsMem;
        //    TieLEDsOn();
        //    UpdateGames();
        _setLeds.updateSets();
        GameTimer::gameDelay(
            _gameState
            ->getFlashDelay());  // delay(
        // _gameState->getFlashDelay()
        // );
    }
    tieLEDsOff();
    _gameState->setTieBreak( 0 );  // tieBreak = false;
    _player1->setGames( 0 );       // p1Games = 0;
    _player2->setGames( 0 );       // p2Games = 0;
    _gameLeds.updateGames();     // UpdateGames();
}


////////////////////////////////// SET WIN SEQUENCES //////////////////////////////////////////////
void Mode1WinSequences::p1TBSetWinSequence() {  // for entering set t/b
    _undo.memory();                               // Memory();
    for ( int currentPulseCount = 0; currentPulseCount < SET_WIN_PULSE_COUNT; currentPulseCount++ ) {
        _player1->setSets( 0 );
        tieLEDsOff();
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
        _player1->setSets( 1 );
        tieLEDsOn();
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
    }
    _player1->setGames( 0 );
    _player2->setGames( 0 );
    tieLEDsOn();
}

void Mode1WinSequences::p2TBSetWinSequence() {  // for entering set t/b
    _undo.memory();                               // Memory();
    for ( int currentPulseCount = 0;
        currentPulseCount < SET_WIN_PULSE_COUNT;
        currentPulseCount++ ) {
        _player2->setSets( 0 );
        tieLEDsOff();
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());

        _player2->setSets( 1 );
        tieLEDsOn();
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
    }
    _player1->setGames( 0 );
    _player2->setGames( 0 );
    tieLEDsOn();
}
///////////////////////////// END OF SET WIN SEQUENCES ////////////////////////////////////////////



////////////////////////// SET TIE BREAKER WIN SEQUENCES //////////////////////////////////////////
void Mode1WinSequences::p1SetTBWinSequence() {  // for winning set t/b
    _undo.memory();                               // Memory();

    for ( int currentPulseCount = 0; currentPulseCount < SET_WIN_PULSE_COUNT; currentPulseCount++ ) {
        _player1->setSets( 0 );
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
        _player1->setSets( 3 );
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
    }
}

void Mode1WinSequences::p2SetTBWinSequence() {  // for winning set t/b
    _undo.memory();                               // Memory();
    for ( int currentPulseCount = 0; currentPulseCount < SET_WIN_PULSE_COUNT; currentPulseCount++ ) {
        _player2->setSets( 0 );
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());

        _player2->setSets( 3 );
        _setLeds.updateSets();
        GameTimer::gameDelay( _gameState->getFlashDelay());
    }
}
//////////////////////// END OF SET TIE BREAKER WIN SEQUENCES /////////////////////////////////////

void Mode1WinSequences::p1TBMatchWinSequence() {
    _player2->setGames( 99 );   // p2Games = 99;
    _gameLeds.updateGames();  // UpdateGames();
    _undo.memory();           // Memory();
    tieLEDsOff();

    MatchWinSequence matchWinSequence; matchWinSequence.run( _player1, _gameState, &_gameLeds, &_setLeds );

    // for ( int currentPulseCount = 0; currentPulseCount < _gameState->getMatchWinPulseCount(); currentPulseCount++ ) {
    //     _player1->setSets( 0 );
    //     _setLeds.updateSets();
    //     GameTimer::gameDelay( _gameState->getFlashDelay());
    //     _player1->setSets( _gameState->getP1SetsMem() );
    //     _setLeds.updateSets();
    //     GameTimer::gameDelay( _gameState->getFlashDelay()); }
    _reset.resetScoreboard();
}

void Mode1WinSequences::p2TBMatchWinSequence() {
    _player1->setGames( 99 );
    _gameLeds.updateGames();  // UpdateGames();
    _undo.memory();           // Memory();
    tieLEDsOff();

    MatchWinSequence matchWinSequence; matchWinSequence.run( _player2, _gameState, &_gameLeds, &_setLeds );

    // for ( int currentPulseCount = 0; currentPulseCount < _gameState->getMatchWinPulseCount(); currentPulseCount++ ) {
    //     _player2->setSets( 0 );
    //     _setLeds.updateSets();
    //     GameTimer::gameDelay( _gameState->getFlashDelay());
    //     _player2->setSets( _gameState->getP2SetsMem() );
    //     _setLeds.updateSets();
    //     GameTimer::gameDelay( _gameState->getFlashDelay());
    // }
    _reset.resetScoreboard();
}

void Mode1WinSequences::tieLEDsOn() {
    _gameState->setTieLEDsOn( 1 );  // tieLEDsOn = true;
    _pinInterface->pinDigitalWrite( P1_TIEBREAKER, HIGH );
    _pinInterface->pinDigitalWrite( P2_TIEBREAKER, HIGH );
}

void Mode1WinSequences::tieLEDsOff() {
    _gameState->setTieLEDsOn( 0 );  // tieLEDsOn = false;
    _pinInterface->pinDigitalWrite( P1_TIEBREAKER, LOW );
    _pinInterface->pinDigitalWrite( P2_TIEBREAKER, LOW );
}
