#include "GameObject.h"
#include <csignal>
#include <chrono>
#include <thread>

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

GameObject::GameObject() {
    _webLiquidCrystal = new WebLiquidCrystal();
    _gameTimer = new GameTimer();
    _player1 = new Player( PLAYER_1 );
    _player2 = new Player( PLAYER_2 );
    _pinState = new PinState( _pin_map );
    _pinInterface = new PinInterface( _pinState );
    _history = new History();
    _gameState = new GameState();
    _gameInputs = new Inputs( _player1, _player2, _pinInterface, _gameState );
    _gameModes =  new GameModes( _player1, _player2, _pinInterface, _gameState, _history );
    _scoreBoard = new ScoreBoard( _player1, _player2, _gameState );
    _subjectManager = new SubjectManager();
    _logger = new Logger( "GameObject" );
}

GameObject::~GameObject() {};
void GameObject::_signalHandler(int signal) { _gSignalStatus = signal; }
volatile int GameObject::_gSignalStatus = 0;

void GameObject::loopGame() {
    std::signal( SIGINT, GameObject::_signalHandler );
    while ( _gSignalStatus != SIGINT ) {
        std::cout << "reading reset from loopGame()..." << std::endl;
        _gameInputs->readReset();
        std::cout << "reading rotary from loopGame()..." << std::endl;
        int rotaryValue = _gameInputs->readRotary();
        std::cout << "rotaryValue: " << rotaryValue << ".  setting game mode to " << rotaryValue << "." << std::endl;
        _gameModes->setGameMode( rotaryValue );
        std::cout << "delaying for " << GAME_LOOP_DELAY << " milliseconds..." << std::endl;
        GameTimer::gameDelay( GAME_LOOP_DELAY );
        std::cout << "updating game state..." << std::endl;
        _subjectManager->gameStateUpdate( _gameState, _player1, _player2 );
        std::cout << "end of loopGame()." << std::endl; }}

void GameObject::playerScore( int playerNumber ) { 
    std::cout << "GameObject::playerScore( " << playerNumber << " )" << std::endl;
    std::cout << "updating game state.. - setting player button to " << playerNumber << " ..." << std::endl;
    _gameState->setPlayerButton( playerNumber ); }    

PinInterface* GameObject::getPinInterface() { return _pinInterface; }

void GameObject::start() {}