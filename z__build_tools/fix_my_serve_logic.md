# Persona
- World-class C++ Tennis Scoreboard Developer
- Expert C++ Tennis Scoreboard Architect
- World-class Object-Oriented C++ Programmer
- Seasoned Tennis Score Keeper

# Background Information
- The player scores "1", "2", "3", "4", and "5" are translated to "0", "15", "30", "40", and "Ad" respectively.

# Your Goal
- I think the part that determine who serves is flawed.  Please fix it.

## Mode1Functions.cpp
```cpp
#include "Mode1Functions.h"

Mode1Functions::Mode1Functions( Player* player1,
    Player* player2,
    PinInterface* pinInterface,
    GameState* gameState,
    History* history )
    : _player1( player1 ),
    _player2( player2 ),
    _gameState( gameState ),
    _history( history ),
    _undo( player1, player2, pinInterface, gameState ),
    _pointLeds( player1, player2, pinInterface ),
    _mode1Score( player1, player2, pinInterface, gameState, history ),
    _serveLeds( pinInterface, gameState ) {}

Mode1Functions::~Mode1Functions() {}

void Mode1Functions::setScoreBoard( ScoreBoard* scoreBoard ) {
    _scoreBoard = scoreBoard;
    _pointLeds.setScoreBoard( scoreBoard );
    _mode1Score.setScoreBoard( scoreBoard ); }

void Mode1Functions::mode1ButtonFunction() {
    switch ( _gameState->getPlayerButton()) {
    case 0:
        break;

    case 1: // Player 1 Score
        _undo.snapshot( _history );
        if ( _gameState->getPointFlash() == 1 ) {
            _gameState->setPointFlash( 0 );
            _player1->setPoints( _gameState->getP1PointsMem());
            _player2->setPoints( _gameState->getP2PointsMem());
            _gameState->setPlayer1Points( _player1->getPoints());
            _gameState->setPlayer2Points( _player2->getPoints());
        }
        GameTimer::gameDelay( _gameState->getButtonDelay());
        // if serving, increment score.  if not serving, set serve to 1 and don't increment score.
        if ( _gameState->getServe() == PLAYER_ONE_SERVE || 
            /*aways increment when tie break is enabled*/ _gameState->getTieBreak() == 1 )  {
            _player1->setPoints( _player1->getPoints() + 1 );
            _gameState->setPlayer1Points( _player1->getPoints());
        } else {
            _gameState->setServe( PLAYER_ONE_SERVE ); }
        _undo.memory();
        _mode1Score.playerOneScore();
        break;

    case 3: // UNDO button pressed
        GameTimer::gameDelay( _gameState->getButtonDelay());
        _undo.mode1Undo( _history );  // Mode1Undo();
        break;

    case 2: // Player 2 Score
        _undo.snapshot( _history );
        if ( _gameState->getPointFlash() == 1 ) {
            _gameState->setPointFlash( 0 );
            _player1->setPoints( _gameState->getP1PointsMem());
            _player2->setPoints( _gameState->getP2PointsMem());
            _gameState->setPlayer1Points( _player1->getPoints());
            _gameState->setPlayer2Points( _player2->getPoints()); }

        GameTimer::gameDelay( _gameState->getButtonDelay());
        if ( _gameState->getServe() == PLAYER_TWO_SERVE || 
            /*aways increment when tie break is enabled*/ _gameState->getTieBreak() == 1 )  {
            _player2->setPoints( _player2->getPoints() + 1 );
            _gameState->setPlayer2Points( _player2->getPoints());
        } else {
            _gameState->setServe( PLAYER_TWO_SERVE ); }
        _undo.memory();
        _mode1Score.playerTwoScore();
        break;

    case 4:
        GameTimer::gameDelay( _gameState->getButtonDelay());
        _undo.mode1Undo( _history );
        break; }                        // end switch ( _gameState->getPlayerButton())
    _gameState->setPlayerButton( 0 ); } // reset player button here!

void Mode1Functions::mode1ServeFunction() {
    _undo.snapshot( _history );
    _serveLeds.serveSwitch(); }

void Mode1Functions::pointFlash() {
    if ( _gameState->getPointFlash() == 1 ) {
        if ( _player1->getPoints() > 3 ) {
            if ( _gameState->getNow() - _gameState->getPreviousTime() > _gameState->getFlashDelay()) {
                if ( _gameState->getToggle() == 0 ) {
                    _player1->setPoints( SCORE_CASE_4 );      
                    _pointLeds.updatePoints();
                    _gameState->setToggle( 1 );
                } else {
                    _player1->setPoints(
                    _gameState->getP1PointsMem());
                    _pointLeds.updatePoints();
                    _gameState->setToggle( 0 ); }
                _gameState->setPreviousTime( _gameState->getNow()); }}

        if ( _player2->getPoints() > 3 ) {
                if ( _gameState->getNow() - _gameState->getPreviousTime() > _gameState->getFlashDelay()) {
                    if ( _gameState->getToggle() == 0 ) {
                        _player2->setPoints( SCORE_CASE_4 );      
                        _pointLeds.updatePoints();
                        _gameState->setToggle( 1 );
                    } else {
                        _player2->setPoints(
                        _gameState->getP2PointsMem());
                        _pointLeds.updatePoints();
                        _gameState->setToggle( 0 ); }
                    _gameState->setPreviousTime( _gameState->getNow()); }}}}
```

## Mode1Score.cpp
```cpp
Mode1Score::Mode1Score( 
    Player* player1,
    Player* player2,
    PinInterface* pinInterface,
    GameState* gameState,
    History* history )
    : _player1( player1 ),
    _player2( player2 ),
    _gameState( gameState ),
    _history( history ),
    _tieBreaker( player1, player2, pinInterface, gameState, history ),
    _pointLeds( player1, player2, pinInterface ),
    _gameLeds( player1, player2, pinInterface ),
    _setLeds( player1, player2, pinInterface ),
    _mode1WinSequences( player1, player2, pinInterface, gameState ),
    _undo( player1, player2, pinInterface, gameState ) {}
Mode1Score::~Mode1Score() {}

TieBreaker* Mode1Score::getTieBreaker() { return &_tieBreaker; }

void Mode1Score::setScoreBoard( ScoreBoard* scoreBoard ) {
    _scoreBoard = scoreBoard;
    _pointLeds.setScoreBoard(          scoreBoard ); 
    _gameLeds.setScoreBoard(           scoreBoard ); 
    _mode1WinSequences.setScoreBoards( scoreBoard ); 
    _setLeds.setScoreBoard(            scoreBoard ); 
    _tieBreaker.setScoreBoards(   scoreBoard );         // all day debug. in the future.
    _undo.setScoreBoard(               scoreBoard ); }    // need to find a way to avoid this

ScoreBoard* Mode1Score::getScoreBoard() { return _scoreBoard; }

void Mode1Score::_resetGame() {
     GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    _player1->setPoints( 0 );
    _player2->setPoints( 0 );
    _gameState->setPlayer1Points( 0 );
    _gameState->setPlayer2Points( 0 );
    _gameState->setServeSwitch( 1 );
    _gameState->setServe( 0 );
    _pointLeds.updatePoints(); }

void Mode1Score::updateScore( Player* currentPlayer ) {
    if ( _gameState->getTieBreak() == 1 ) {           // Tie Break
        _tieBreaker.run( currentPlayer );        
    } else if ( _gameState->getSetTieBreak() == 1 ) { // Set Tie Break
        _tieBreaker.setTieBreaker();            
    } else {                                          // Regular Game
        Player* otherPlayer = currentPlayer->getOpponent();
        int current_player_points = currentPlayer->getPoints();
        int other_player_points   = otherPlayer->getPoints();
        if ( current_player_points >= 3 ) {
            if ( current_player_points == other_player_points ) {
                currentPlayer->setPoints( 3 );
                otherPlayer->setPoints(   3 );
            } else if ( current_player_points > 3 
                && ( current_player_points - other_player_points ) > 1 ) {
                currentPlayer->setGames( currentPlayer->getGames() + 1);
                _undo.memory();
                currentPlayer->number() == 0 ? playerOneGameWin() : playerTwoGameWin(); } 

            if ( currentPlayer->getPoints() == 4 ) {
                _gameState->setPointFlash( 1 );       // "Ad" mode
                _gameState->setPreviousTime( GameTimer::gameMillis());
                _gameState->setToggle( 0 );
            }
        }
        _pointLeds.updatePoints();
    }
}

void Mode1Score::playerOneScore() { updateScore( _player1 );}
void Mode1Score::playerTwoScore() { updateScore( _player2 );}


//////////////////////////////// GAME WIN SCENARIOS ///////////////////////////
void Mode1Score::playerGameWin( Player* player ) {
    Player* opponent = player->getOpponent();
    std::cout << "player " << player->number() << " games: " << player->getGames() << std::endl;
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    if (player->getGames() >= GAMES_TO_WIN_SET) {
        if (player->getGames() == GAMES_TO_WIN_SET && opponent->getGames() == GAMES_TO_WIN_SET) {
            _gameState->setTieBreak(1);
            _tieBreaker.initializeTieBreakMode();
        }
        if (_gameState->getTieBreak() == 0) {
            if ((player->getGames() - opponent->getGames()) > 1) {
                player->setSets(_gameState, player->getSets() + 1);
                _setLeds.updateSets();
                
                if (player->getSets() == opponent->getSets()) {
                    player->number() == PLAYER_1_INITIALIZED ? _mode1WinSequences.p1TBSetWinSequence() : _mode1WinSequences.p2TBSetWinSequence();
                    _gameState->setSetTieBreak(1);
                    _tieBreaker.setTieBreakEnable();
                } else if (player->getSets() == SETS_TO_WIN_MATCH) {
                    player->number() == PLAYER_1_INITIALIZED ? _mode1WinSequences.playerOneMatchWin() : _mode1WinSequences.playerTwoMatchWin();
                    _gameState->stopGameRunning();
                } else {
                    _gameState->setPlayer1SetHistory(player->getSetHistory());
                    _gameState->setPlayer2SetHistory(opponent->getSetHistory());
                    player->number() == PLAYER_1_INITIALIZED ? _mode1WinSequences.p1SetWinSequence() : _mode1WinSequences.p2SetWinSequence();
                    _gameState->setCurrentSet(_gameState->getCurrentSet() + 1);
                    _setLeds.updateSets();
                    GameTimer::gameDelay(_gameState->getWinDelay());
                    _resetGame();
                }
                player->setGames(0);
                opponent->setGames(0);
            } else {
                player->number() == PLAYER_1_INITIALIZED ? _mode1WinSequences.p1GameWinSequence() : _mode1WinSequences.p2GameWinSequence();
                _gameLeds.updateGames();
                _gameState->setPlayer1SetHistory(player->getSetHistory());
                _gameState->setPlayer2SetHistory(opponent->getSetHistory());
                _resetGame();
            }
        }
    } else {
        if ( player->number() == PLAYER_1_INITIALIZED ) {
            _mode1WinSequences.p1GameWinSequence();
            _gameState->setPlayer1SetHistory(player->getSetHistory());
            _gameState->setPlayer2SetHistory(opponent->getSetHistory());
        } else {
            _mode1WinSequences.p2GameWinSequence();
            _gameState->setPlayer2SetHistory(player->getSetHistory());
            _gameState->setPlayer1SetHistory(opponent->getSetHistory());
        } 
        _gameLeds.updateGames();
        _resetGame();
    }
}
void Mode1Score::playerOneGameWin() { playerGameWin( _player1 ); }
void Mode1Score::playerTwoGameWin() { playerGameWin( _player2 ); }
//////////////////// END GAME WIN SCENARIOS ///////////////////////////////////

void Mode1Score::mode1TBP1Games() {
    _gameLeds.updateGames();
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player1->getGames() == 15 ) {
        _player1->setSets( _gameState, _player1->getSets() + 1 );

        if ( _player2->getSets() == _player1->getSets()) {
            _tieBreaker.endTieBreak();
            _mode1WinSequences.p1TBSetWinSequence();
            _gameState->setSetTieBreak( 1 );
            _tieBreaker.setTieBreakEnable();
        }
        else {
            _player1->setGames( _player1->getGames() );
            _gameState->setPlayer1SetHistory( _player1->getSetHistory());
            _gameState->setPlayer2SetHistory( _player2->getSetHistory());
            _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
            _mode1WinSequences.p1SetWinSequence();
            _tieBreaker.endTieBreak();
        }}
    if ( _player1->getGames() >= 10 &&
        ( _player1->getGames() - _player2->getGames()) > 1 ) {
        _player1->setSets( _gameState, _player1->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets()) {
            _tieBreaker.endTieBreak();
            _mode1WinSequences.p1TBSetWinSequence();
            _gameState->setSetTieBreak( 1 );
            _tieBreaker.setTieBreakEnable();
        }
        else {
            _player1->setGames( _player1->getGames() );
            _gameState->setPlayer1SetHistory( _player1->getSetHistory());
            _gameState->setPlayer2SetHistory( _player2->getSetHistory());
            _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
            _mode1WinSequences.p1SetWinSequence();
            _tieBreaker.endTieBreak();
        }}}

void Mode1Score::mode1SetTBP1Games() {
    _gameLeds.updateGames();
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player1->getGames() == 7 ) {
        _player1->setSets( _gameState, _player1->getSets() + 1 );
        _setLeds.updateSets();
        GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
        _mode1WinSequences.p1SetTBWinSequence();
        _tieBreaker.tieLEDsOff();
        _mode1WinSequences.playerOneMatchWin();
        _gameState->stopGameRunning(); }
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 ); }

void Mode1Score::mode1TBP2Games() {
    _gameLeds.updateGames();
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player2->getGames() == 15 ) {
        _player2->setSets( _gameState, _player2->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets()) {
            _tieBreaker.endTieBreak();
            _mode1WinSequences.p2TBSetWinSequence();
            _gameState->setSetTieBreak( 1 );
            _tieBreaker.setTieBreakEnable();
        } else {
            _player1->setGames( _player1->getGames() );
            _gameState->setPlayer1SetHistory( _player1->getSetHistory());
            _gameState->setPlayer2SetHistory( _player2->getSetHistory());
            _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
            _mode1WinSequences.p2SetWinSequence();
            _tieBreaker.endTieBreak(); }}
    if ( _player2->getGames() >= 10 &&
        ( _player2->getGames() - _player1->getGames()) > 1 ) {
        _player2->setSets( _gameState, _player2->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets()) {
            _tieBreaker.endTieBreak();
            _mode1WinSequences.p2TBSetWinSequence();
            _gameState->setSetTieBreak( 1 );
            _tieBreaker.setTieBreakEnable();
        } else {
            _player1->setGames( _player1->getGames() );
            _gameState->setPlayer1SetHistory( _player1->getSetHistory());  // set set history, set++
            _gameState->setPlayer2SetHistory( _player2->getSetHistory());
            _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
            _mode1WinSequences.p2SetWinSequence();
            _tieBreaker.endTieBreak(); }}}

void Mode1Score::mode1SetTBP2Games() {
    _gameLeds.updateGames();
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player2->getGames() == 7 ) {
        _player2->setSets( _gameState, _player2->getSets() + 1 );
        _setLeds.updateSets();
        GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
        _mode1WinSequences.p2SetTBWinSequence();
        _tieBreaker.tieLEDsOff();
        _mode1WinSequences.playerTwoMatchWin(); 
        _gameState->stopGameRunning(); 
    }
    std::cout << "setting serve switch to: " << _gameState->getServeSwitch() + 1 << std::endl;
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 ); 
}
```