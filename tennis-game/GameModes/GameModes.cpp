#include "GameModes.h"

GameModes::~GameModes() {
    // std::cout << "*** GameModes destructor called. ***" << std::endl;
    delete _logger; }

GameModes::GameModes(
    Player*       player1,
    Player*       player2,
    PinInterface* pinInterface,
    GameState*    gameState,
    History*      history )
    : _player1( player1 ),
    _player2( player2 ),
    _pinInterface( pinInterface ),
    _gameState( gameState ),
    _history( history ),
    _pointLeds( player1, player2, pinInterface ),
    _gameLeds( player1, player2, pinInterface ),
    _setLeds( player1, player2, pinInterface ),
    _inputs( player1, player2, pinInterface, gameState ),
    _undo( player1, player2, pinInterface, gameState ),
    _serveLeds( pinInterface, gameState ),
    _tieBreaker( player1, player2, pinInterface, gameState, history ),
    _mode1Functions( player1, player2, pinInterface, gameState, history ),
    _mode2Functions( player1, player2, pinInterface, gameState ) {
    _logger = new Logger( "GameModes" );
}

void GameModes::undo() { _undo.mode1Undo( _history );}

void GameModes::setScoreBoards( ScoreBoard* scoreBoard ) {
    _pointLeds.setScoreBoard(      scoreBoard );
    _gameLeds.setScoreBoard(       scoreBoard );
    _setLeds.setScoreBoard(        scoreBoard );
    _mode1Functions.setScoreBoard( scoreBoard );
    _undo.setScoreBoard(           scoreBoard ); 
    _tieBreaker.setScoreBoards(    scoreBoard ); }

void GameModes::gameStart() {
    // std::cout << "inside gameStart() checking if gameStarted = zero or not..." << std::endl;
    if ( _gameState->getStarted() == 0 ) {  // if not started...
        // std::cout << "setting player points... " << std::endl;
        _player1->setPoints( 0 );             // p1Points = 0;
        _player2->setPoints( 0 );             // p2Points = 0;
        // std::cout << "setting player games... " << std::endl;
        _player1->setGames( 0 );              // p1Games = 0;
        _player2->setGames( 0 );              // p2Games = 0;
        // std::cout << "setting player sets... " << std::endl;
        _player1->setSets( _gameState, 0 );               // p1Sets = 0;
        _player2->setSets( _gameState, 0 );               // p2Sets = 0;
        // std::cout << "updating points... " << std::endl;
        _pointLeds.updatePoints();          // UpdatePoints();
        // std::cout << "updating games... " << std::endl;
        _gameLeds.updateGames();            // UpdateGames();
        // std::cout << "updating sets... " << std::endl;
        _setLeds.updateSets();              // UpdateSets();
        _gameState->setTieBreakOnly( 0 );     // tieBreakOnly = false;
        // std::cout << "setting started to 1... " << std::endl;
        _gameState->setStarted( 1 );
    } else {
        // std::cout << "Game already started. " << std::endl;
    }}

void GameModes::mode1() {
    // std::cout << "inside game mode 1." << std::endl;
    _gameState->setNow( GameTimer::gameMillis());
    _inputs.readUndoButton();
    if ( _gameState->getUndo() == 1 ) {  // undo button pressed
        // std::cout << "undo button pressed!" << std::endl;
        _gameState->setUndo( 0 );
        // std::cout << "calling mode1Undo( _history )... " << std::endl;
        // std::cout << "calling mode1Undo.setScoreboard... " << std::endl;
        // _undo_.setScoreBoard( _history->getScoreBoard());
        _undo.mode1Undo( _history ); }
    // std::cout << "reading player buttons... " << std::endl;
    _inputs.readPlayerButtons();  // digital read on player buttons.  sets playerButton if tripped.
    _serveLeds.serveSwitch(); // if serveSwitch >= 2, serveSwitch = 0; and toggle serve variable
    // std::cout << "checking for tie breaker... " << std::endl;
    // TODO: took out on july 8
    // if ( _gameState->getSetMatchBreak() == 1 ) { //TODO: took out on july 8
    //     _logger->logUpdate( "running tie breaker..." );
    //     // _tieBreaker.setTieBreaker();
    //     // if the _playerButton member fo _gameState is 1, use player 1 in the _tieBreker.run( _player1 ) method
    //     // if it is 2, use player 2
    //     if ( _gameState->getPlayerButton() == 1 ) {
    //         _logger->logUpdate( "calling tieBreaker.run( _player1 )" );
    //         _tieBreaker.run( _player1 );
    //     } else if ( _gameState->getPlayerButton() == 2 ) {
    //         _logger->logUpdate( "calling tieBreaker.run( _player2 )" );
    //         _tieBreaker.run( _player2 );
    //     }
        // _tieBreaker.run();  // we need a player here.  there must be some place else...
        // got player above
    // } else {
        _logger->setName( "mode1" );
        _mode1Functions.mode1ButtonFunction(); // <--------- ENTRY POINT --------------<<
        _mode1Functions.pointFlash();
    //}
}

void GameModes::mode2() {
    _gameState->setNow( GameTimer::gameMillis() );
    if ( _gameState->getTieBreakOnly() == 0 ) {
        _gameState->setTieBreak( 1 );
        _tieBreaker.initializeTieBreakMode();
        _gameState->setTieBreakOnly( 1 );
    }
    mode1(); }

void GameModes::mode4() {
    _gameState->setNow( GameTimer::gameMillis() );
    if ( _gameState->getTieBreakOnly() == 0 ) {
        _gameState->setTieBreak( 1 );
        _tieBreaker.initializeTieBreakMode();
        _gameState->setTieBreakOnly( 1 );
    }
    mode1(); }

void GameModes::testStart() { if ( _gameState->getStarted() == 0 ) {  /* if not started... */ }}

void GameModes::noCode() {
    _player1->setPoints( _player1->getPoints() + 1 );  // p1Points++;
    _pointLeds.updatePoints();                        // UpdatePoints();
    GameTimer::gameDelay( 1000 );
    _player1->setPoints( _player1->getPoints() - 1 ); // p1Points--;
    _pointLeds.updatePoints();                        // UpdatePoints();
    GameTimer::gameDelay( 1000 ); }

void GameModes::runGameMode( int rotaryPosition ) {
    WatchTimer *watchTimer = new WatchTimer();
    BatteryTest batteryTest( _player1, _player2, _pinInterface, &_pointLeds, &_inputs );
    switch ( rotaryPosition ) {
    case 0:
        break;

    case 1:
        // std::cout << "calling gameStart()... " << std::endl;
        gameStart();  // sets gameStart to true. resets player and score board.
        // std::cout << "calling mode1()... " << std::endl;
        mode1();
        break;

    case 4:
        #if defined _WIN32 || defined _WIN64
            std::cout << "Game Mode 2" << std::endl;
            #define TEST_DELAY 150
        #else
            #define TEST_DELAY 500
        #endif
        _player1->setPoints( 100 );
        _player2->setPoints( 100 );
        _pointLeds.updatePoints();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player1->setPoints( 0 );
        _pointLeds.updatePoints();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player1->setPoints( 1 );
        _pointLeds.updatePoints();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player1->setPoints( 2 );
        _pointLeds.updatePoints();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player1->setPoints( 3 );
        _pointLeds.updatePoints();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player1->setPoints( 4 );
        _pointLeds.updatePoints();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player1->setPoints( 100 );
        _pointLeds.updatePoints();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };

        _player2->setPoints( 0 );
        _pointLeds.updatePoints();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player2->setPoints( 1 );
        _pointLeds.updatePoints();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player2->setPoints( 2 );
        _pointLeds.updatePoints();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player2->setPoints( 3 );
        _pointLeds.updatePoints();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player2->setPoints( 4 );
        _pointLeds.updatePoints();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player2->setPoints( 100 );
        _pointLeds.updatePoints();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        // std::cout << "setting serve to 1..." << std::endl;
        _gameState->setServe( 1 );
        _serveLeds.updateServeLED();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };

        // std::cout << "setting serve to 0..." << std::endl;
        _gameState->setServe( 0 );
        _serveLeds.updateServeLED();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };

        _pinInterface->pinDigitalWrite( P1_SERVE, LOW);
        _pinInterface->pinDigitalWrite( P2_SERVE, LOW);
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };

        _player1->setSets( _gameState, 1 );
        _setLeds.updateSets();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player1->setSets( _gameState, 2 );
        _setLeds.updateSets();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player1->setSets( _gameState, 0 );
        _setLeds.updateSets();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player2->setSets( _gameState, 1 );
        _setLeds.updateSets();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player2->setSets( _gameState, 2 );
        _setLeds.updateSets();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player2->setSets( _gameState, 0 );
        _setLeds.updateSets();
        _player1->setGames( 99 );
        _player2->setGames( 99 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player1->setGames( 0 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player1->setGames( 1 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player1->setGames( 2 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player1->setGames( 3 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player1->setGames( 4 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player1->setGames( 5 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player1->setGames( 6 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player1->setGames( 99 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player2->setGames( 99 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player2->setGames( 0 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player2->setGames( 1 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player2->setGames( 2 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player2->setGames( 3 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player2->setGames( 4 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player2->setGames( 5 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player2->setGames( 6 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _player2->setGames( 99 );
        _gameLeds.updateGames();
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _pinInterface->pinDigitalWrite( P1_TIEBREAKER, HIGH );
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _pinInterface->pinDigitalWrite( P1_TIEBREAKER, LOW );
        _pinInterface->pinDigitalWrite( P2_TIEBREAKER, HIGH );
        if ( watchTimer->watchInputDelay( TEST_DELAY, &_inputs, WATCH_INTERVAL ) > 0 ) { return; };
        _pinInterface->pinDigitalWrite( P2_TIEBREAKER, LOW);
        break;

    case 2:
        #if defined _WIN32 || defined _WIN64
            std::cout << "Game Mode 3" << std::endl;
        #endif
        gameStart();
        _gameState->setNow( GameTimer::gameMillis() );
        if ( _gameState->getTieBreakOnly() == 0 ) {
            _gameState->setTieBreak( 1 );
            _tieBreaker.initializeTieBreakMode();
            _gameState->setTieBreakOnly( 1 );
        }
        mode1();
        break;

    case 3:
        batteryTest.execute();
        break;

    case 5:
        gameStart();
        _player1->setPoints( 5 );
        _pointLeds.updatePoints();
        mode1(); // <----------------- ENTRY POINT -------------------<<
        break;
    }
}
