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
    _player2 = new Player( _gameState, PLAYER_2_INITIALIZED ); // now set echother as opponents...
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
    std::cout << "GameObject constructed." << std::endl;
}

GameObject::~GameObject() {};
void GameObject::_signalHandler( int signal ) { GameObject::gSignalStatus = signal; }
volatile int GameObject::gSignalStatus = 0;

void GameObject::undo() {
    _gameModes->undo();
}

void GameObject::loopGame() {
        // std::cout << "reading reset from loopGame()..." << std::endl;
        // _gameInputs->readReset();
        // std::cout << "reading rotary from loopGame()..." << std::endl;
        std::cout << "*** //////////// starting another loop.  current set is: " << _gameState->getCurrentSet() << " ////////////////// ***" << std::endl;
        int rotaryValue = 1; // int rotaryValue = _gameInputs->readRotary(); TODO: actually read rotary
        // std::cout << "rotaryValue: " << rotaryValue << ".  setting game mode to " << rotaryValue << "." << std::endl;
        _gameModes->setGameMode( rotaryValue );  // mode1() called here <--- entry point !! --------------<<
        // std::cout << "delaying for " << GAME_LOOP_DELAY << " milliseconds..." << std::endl;
        GameTimer::gameDelay( GAME_LOOP_DELAY );
        // std::cout << "updating game state..." << std::endl;
        _subjectManager->gameStateUpdate( _gameState, _player1, _player2 ); }

GameState* GameObject::getGameState() { return _gameState; }

void GameObject::playerScore( int playerNumber ) {  // sets the gamestate player button
    _gameState->setCurrentAction( "Updating state after player " + std::to_string( playerNumber ) + " scored." );
    // std::cout << "GameObject::playerScore( " << playerNumber << " )" << std::endl;
    // std::cout << "updating game state...  setting player button to " << playerNumber << " ..." << std::endl;
    // int current_player_button = _gameState->getPlayerButton();
    // std::cout << "current_player_button: " << current_player_button << std::endl;
    _gameState->setPlayerButton( playerNumber );
    // current_player_button = _gameState->getPlayerButton();
    // std::cout << "current_player_button: " << current_player_button << std::endl;
    _gameState->setCurrentAction( "after player score in GameObject" );
}

PinInterface* GameObject::getPinInterface() { return _pinInterface; }

ScoreBoard* GameObject::getScoreBoard() { return _scoreBoard; }

History* GameObject::getHistory() { return _history; }

Undo* GameObject::createUndo() { return new Undo( _player1, _player2, _pinInterface, _gameState ); }

void GameObject::start() {
    _player1->setPoints( 0 );             // p1Points = 0;
    _player2->setPoints( 0 );             // p2Points = 0;
    std::cout << "setting player games... " << std::endl;
    _player1->setGames( 0 );              // p1Games = 0;
    _player2->setGames( 0 );              // p2Games = 0;
    std::cout << "setting player sets... " << std::endl;
    _player1->setSets( _gameState, 0 );               // p1Sets = 0;
    _player2->setSets( _gameState, 0 );               // p2Sets = 0;
    std::cout << "updating scoreboard in game object start()... " << std::endl;
    _scoreBoard->update();
    _gameState->setTieBreakOnly( 0 );     // tieBreakOnly = false;
    std::cout << "setting started to 1... " << std::endl;
    _gameState->setStarted( 1 );
}
