#include "GameObject.h"

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
    _player1 = new Player( 1 );
    _player2 = new Player( 1 );
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

void GameObject::loopGame() {
    _gameInputs->readReset();
    std::cout << "reading rotary from loopGame()..." << std::endl;
    int rotaryValue = _gameInputs->readRotary();
    std::cout << "rotaryValue: " << rotaryValue << ".  setting game mode to " << rotaryValue << "." << std::endl;
    _gameModes->setGameMode( rotaryValue );
    GameTimer::gameDelay( 25 );
    _subjectManager->gameStateUpdate( _gameState, _player1, _player2 );
}

PinInterface* GameObject::getPinInterface() { return _pinInterface; }

void GameObject::start() {}
