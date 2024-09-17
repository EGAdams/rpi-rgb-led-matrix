#include "GameObject.h"
#include <csignal>
#include <chrono>
#include <thread>
#include <iostream>

GameObject::GameObject( Player* player1,
    Player* player2,
    PinState* pinState,
    PinInterface* pinInterface,
    GameState* gameState,
    GameTimer* gameTimer,
    Inputs* gameInputs,
    GameModes* gameModes,
    ScoreBoard* scoreBoard,
    WebLiquidCrystal* lcd ) {
    _player1 = player1;
    _player2 = player2;
    _pinState = pinState;
    _pinInterface = pinInterface;
    _gameState = gameState;
    _gameTimer = gameTimer;
    _gameInputs = gameInputs;
    _gameModes = gameModes;
    _scoreBoard = scoreBoard;
    _webLiquidCrystal = lcd;
}

GameObject::GameObject( GameState* gameState ) : _gameState( gameState ) {
    _webLiquidCrystal = new WebLiquidCrystal();
    _gameTimer = new GameTimer();
    _player1 = new Player( _gameState, PLAYER_1_INITIALIZED );
    _player2 = new Player( _gameState, PLAYER_2_INITIALIZED ); // now set each other as opponents...
    _player2->setOpponent( _player1 ); _player1->setOpponent( _player2 );
    _pinState = new PinState( _pin_map );
    _pinInterface = new PinInterface( _pinState );
    _history = new History();
    _gameInputs = new Inputs( _player1, _player2, _pinInterface, _gameState );
    _gameModes =  new GameModes( _player1, _player2, _pinInterface, _gameState, _history );
    _scoreBoard = new ScoreBoard( _player1, _player2, _gameState );
    _gameModes->setScoreBoards( _scoreBoard );
    _subjectManager = new SubjectManager();
    _logger = new Logger( "GameObject" );
}

Player* GameObject::getPlayer1() { return _player1; }
Player* GameObject::getPlayer2() { return _player2; }

GameObject::~GameObject() {};
void GameObject::_signalHandler( int signal ) { GameObject::gSignalStatus = signal; }
volatile int GameObject::gSignalStatus = 0;

void GameObject::undo() {
    _gameModes->undo();
}

void GameObject::loopGame() {
    _logger->setName( "loopGame" );                   
    int rotaryValue = 1; // int rotaryValue = _gameInputs->readRotary(); TODO: actually read rotary
    _gameModes->runGameMode( rotaryValue );  // <--- entry point !! --------------<<
    GameTimer::gameDelay( GAME_LOOP_DELAY );
    _subjectManager->gameStateUpdate( _gameState, _player1, _player2 );
}

GameState* GameObject::getGameState() { return _gameState; }

void GameObject::playerScore( int playerNumber ) {  // sets the gamestate player button
    if( _gameState->getCurrentAction() == AFTER_SLEEP_MODE ) { 
        print( "clearing history because of after sleep and 1st player score... " );
        getHistory()->clearHistory();
        print( "done clearing history because of after sleep and 1st player score... " );
    }
    _gameState->setCurrentAction( "Updating state after player " + std::to_string( playerNumber ) + " scored." );
    _gameState->setPlayerButton( playerNumber );
    _gameState->setCurrentAction( AFTER_UPDATE_GO_SCORE );
}

void GameObject::resetMatch() {
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    _player1->clearSetHistory();  // Clear Player's Set history 
    _player2->clearSetHistory();
    _player1->clearGameHistory(); // Clear Player's Game history
    _player2->clearGameHistory();
    _gameState->setPlayer1SetHistory( _player1->getSetHistory() ); // clear gamestate sets
    _gameState->setPlayer2SetHistory( _player2->getSetHistory() );
    _gameState->setGameHistory( _player1->getGameHistory() );      // clear gamestate games
    _gameState->setGameHistory( _player2->getGameHistory() );
    _gameState->setCurrentSet( 1 );                               // set back to beginning
    _gameState->setServeSwitch( 1 );
    _player1->setPoints( 0 );           // Reset Player's points
    _player2->setPoints( 0 );
    _player1->setGames( 0 );           // Reset Player's games
    _player2->setGames( 0 );
    _player1->setSets( 0 );             // Reset Player's sets
    _player2->setSets( 0 );
    _gameState->setPlayer1Points( 0 );  // now gamestate points
    _gameState->setPlayer2Points( 0 );
    _gameState->setTieBreak( 0 );       // turn tie break flags off
    _gameState->setMatchTieBreak( 0 );
    _gameState->setServe( 0 );          // reset serve
    _gameState->setP1SetsMem( 0 );
    _gameState->setP2SetsMem( 0 );
    _gameState->setP1GamesMem( 0 );
    _gameState->setP2GamesMem( 0 );
    _gameState->setP1PointsMem( 0 );
    _gameState->setP2PointsMem( 0 );
    // _pointLeds.updatePoints(); may need this for the digi
    _scoreBoard->clearScreen();
    _scoreBoard->update();
}

PinInterface* GameObject::getPinInterface() { return _pinInterface; }

ScoreBoard* GameObject::getScoreBoard() { return _scoreBoard; }

History* GameObject::getHistory() { return _history; }

Undo* GameObject::createUndo() { return new Undo( _player1, _player2, _pinInterface, _gameState ); }

void GameObject::start() {
    _player1->setPoints( 0 );             // p1Points = 0;
    _player2->setPoints( 0 );             // p2Points = 0;
    _player1->setGames( 0 );              // p1Games = 0;
    _player2->setGames( 0 );              // p2Games = 0;
    _player1->setSets( _gameState, 0 );   // p1Sets = 0;
    _player2->setSets( _gameState, 0 );   // p2Sets = 0;
    _scoreBoard->update();
    _gameState->setTieBreakOnly( 0 );     // tieBreakOnly = false;
    _gameState->setCurrentSet( 1 );
    _gameState->setStarted( 1 );
}
