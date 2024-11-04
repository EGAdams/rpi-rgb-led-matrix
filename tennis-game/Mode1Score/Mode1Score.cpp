#include "Mode1Score.h"

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
    _undo( player1, player2, pinInterface, gameState ) {
    _logger = new Logger( "Mode1Score" );
}
Mode1Score::~Mode1Score() {}

TieBreaker* Mode1Score::getTieBreaker() { return &_tieBreaker; }

void Mode1Score::setScoreBoard( ScoreBoard* scoreBoard ) {
    _scoreBoard = scoreBoard;
    _pointLeds.setScoreBoard( scoreBoard );
    _gameLeds.setScoreBoard( scoreBoard );
    _mode1WinSequences.setScoreBoards( scoreBoard );
    _setLeds.setScoreBoard( scoreBoard );
    _tieBreaker.setScoreBoards( scoreBoard ); // all day debug. in the future.
    _undo.setScoreBoard( scoreBoard );
} // find a way to avoid this.

ScoreBoard* Mode1Score::getScoreBoard() { return _scoreBoard; }

void Mode1Score::_resetGame() {
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    _player1->setPoints( 0 );
    _player2->setPoints( 0 );
    _gameState->setPlayer1Points( 0 );
    _gameState->setPlayer2Points( 0 );
    _gameState->setServeSwitch( 1 );
    _pointLeds.updatePoints();
}

void Mode1Score::updateScore( Player* currentPlayer ) { // This is the router.
    _logger->setName( "updateScore" );
    if ( _gameState->getTieBreak() == 1 ) {             // Set Tie Break
        _tieBreaker.run( currentPlayer );
    } else if ( _gameState->getMatchTieBreak() == 1 ) { // Match Tie Break
        _gameState->setCurrentAction( RUNNING_MATCH_TIE_BREAK );
        std::cout << "running tie breaker..." << std::endl;
        _tieBreaker.run( currentPlayer );
    } else {                                            // Regular Game
        Player* otherPlayer = currentPlayer->getOpponent();
        int current_player_points = currentPlayer->getPoints();
        int other_player_points = otherPlayer->getPoints();
        if ( current_player_points >= 3 ) {
            if ( current_player_points == other_player_points ) {
                currentPlayer->setPoints( 3 );
                otherPlayer->setPoints( 3 );
            } else if ( current_player_points > 3 && ( current_player_points - other_player_points ) > 1 ) {
                currentPlayer->setGames( currentPlayer->getGames() + 1 );
                _undo.memory();
                currentPlayer->number() == 0 ? playerOneGameWin() : playerTwoGameWin();  // Game Win
            } else  if ( currentPlayer->getPoints() == 4 ) {
                _gameState->setPointFlash( 1 );       // "Ad" mode
                _gameState->setPreviousTime( GameTimer::gameMillis() );
                _gameState->setToggle( 0 );
            }
        }
        if ( _gameState->getCurrentAction() != SLEEP_MODE ) {
            _pointLeds.updatePoints();
        }
    }
}

void Mode1Score::playerOneScore() { updateScore( _player1 ); }
void Mode1Score::playerTwoScore() { updateScore( _player2 ); }


//////////////////////////// GAME WIN SCENARIOS ///////////////////////////////
void Mode1Score::playerGameWin( Player* player ) { // 110124
    Player* opponent = player->getOpponent();
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    if ( player->getGames() >= GAMES_TO_WIN_SET ) { // if so, see of they are equal...
        if ( player->getGames() >= GAMES_TO_WIN_SET && opponent->getGames() >= GAMES_TO_WIN_SET ) {
            _tieBreaker.blinkSetScores();
            _gameState->setTieBreak( 1 );           // they are equal, set the NORMAL tiebreak flag.
            _tieBreaker.initializeTieBreakMode();   // now initialize tie-break mode.
        } // else this is not a regular tie break, but it may be a Match tie break.  let's see...
        if ( _gameState->getTieBreak() == 0 ) {     // if this is not a tie break game...
            if ( ( player->getGames() - opponent->getGames() ) > 1 ) {  // player ahead by 2 games.
                player->setSets( _gameState, player->getSets() + 1 ); // Set win
                _setLeds.updateSets(); // sets the tiebreak, wins the match, or just wins the set.
                if ( player->getSets() == opponent->getSets() && player->getSets() == SETS_TO_WIN_MATCH - 1 ) {  // MATCH Tie Break
                    _mode1WinSequences.enterMatchTieBreak();
                    _gameState->setMatchTieBreak( 1 );
                    _gameState->setCurrentAction( RUNNING_MATCH_TIE_BREAK );
                    _tieBreaker.setTieBreakEnable();
                    _tieBreaker.incrementSet();
                } else if ( player->getSets() == SETS_TO_WIN_MATCH ) {  // match win, done playing
                    MatchWinSequence mws;
                    mws.run( player, _gameState, &_gameLeds, &_setLeds );
                    _gameState->setCurrentAction( SLEEP_MODE );
                } else {                                              // regular set win, then reset
                    _gameState->setPlayer1SetHistory( player->getSetHistory() );
                    _gameState->setPlayer2SetHistory( opponent->getSetHistory() );
                    player->number() == PLAYER_1_INITIALIZED ? _mode1WinSequences.p1SetWinSequence() : _mode1WinSequences.p2SetWinSequence();
                    _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
                    _setLeds.updateSets();
                    GameTimer::gameDelay( _gameState->getWinDelay() );
                    _resetGame();
                    player->setGames( 0 );   // not sure about this but move on...
                    opponent->setGames( 0 ); // These are match win bugs!!
                }
            } else {     // player is ahead by 1 game, but not enough to win the set.
                player->number() == PLAYER_1_INITIALIZED ? _mode1WinSequences.p1GameWinSequence() : _mode1WinSequences.p2GameWinSequence();
                _gameLeds.updateGames();
                if ( player->number() == PLAYER_1_INITIALIZED ) {
                    _gameState->setPlayer1SetHistory( player->getSetHistory() );
                    _gameState->setPlayer2SetHistory( opponent->getSetHistory() );
                } else {
                    _gameState->setPlayer2SetHistory( player->getSetHistory() );
                    _gameState->setPlayer1SetHistory( opponent->getSetHistory() );
                }
                _resetGame();
            }
        }
    } else { // this is a regualar game win...
        if ( player->number() == PLAYER_1_INITIALIZED ) {
            _mode1WinSequences.p1GameWinSequence();
            _gameState->setPlayer1SetHistory( player->getSetHistory() );
            _gameState->setPlayer2SetHistory( opponent->getSetHistory() );
        } else {
            _mode1WinSequences.p2GameWinSequence();
            _gameState->setPlayer2SetHistory( player->getSetHistory() );
            _gameState->setPlayer1SetHistory( opponent->getSetHistory() );
        }
        _gameLeds.updateGames();
        _resetGame();
    }
}
void Mode1Score::playerOneGameWin() { playerGameWin( _player1 ); }
void Mode1Score::playerTwoGameWin() { playerGameWin( _player2 ); }
////////////////////////// END GAME WIN SCENARIOS /////////////////////////////

void Mode1Score::mode1TBP1Games() {
    _gameLeds.updateGames();
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player1->getGames() == 15 ) {
        _player1->setSets( _gameState, _player1->getSets() + 1 );

        if ( _player2->getSets() == _player1->getSets() ) {
            _tieBreaker.endTieBreak();
            _mode1WinSequences.enterMatchTieBreak();
            _gameState->setCurrentAction( RUNNING_MATCH_TIE_BREAK );
            _gameState->setMatchTieBreak( 1 );
            _gameState->setTieBreak( 1 );
            _tieBreaker.setTieBreakEnable();
        }
        else {
            _player1->setGames( _player1->getGames() );
            _gameState->setPlayer1SetHistory( _player1->getSetHistory() );
            _gameState->setPlayer2SetHistory( _player2->getSetHistory() );
            _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
            _mode1WinSequences.p1SetWinSequence();
            _tieBreaker.endTieBreak();
        }
    }
    if ( _player1->getGames() >= 10 &&
        ( _player1->getGames() - _player2->getGames() ) > 1 ) {
        _player1->setSets( _gameState, _player1->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets() ) {
            _tieBreaker.endTieBreak();
            _mode1WinSequences.enterMatchTieBreak();
            _gameState->setCurrentAction( RUNNING_MATCH_TIE_BREAK );
            _gameState->setMatchTieBreak( 1 );
            _gameState->setTieBreak( 1 );
            _tieBreaker.setTieBreakEnable();
        }
        else {
            _player1->setGames( _player1->getGames() );
            _gameState->setPlayer1SetHistory( _player1->getSetHistory() );
            _gameState->setPlayer2SetHistory( _player2->getSetHistory() );
            _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
            _mode1WinSequences.p1SetWinSequence();
            _tieBreaker.endTieBreak();
        }
    }
}

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
        // _gameState->stopGameRunning();
    }
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
}

void Mode1Score::mode1TBP2Games() {
    _gameLeds.updateGames();
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player2->getGames() == 15 ) {
        _player2->setSets( _gameState, _player2->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets() ) {
            _tieBreaker.endTieBreak();
            _mode1WinSequences.enterMatchTieBreak();
            _gameState->setCurrentAction( RUNNING_MATCH_TIE_BREAK );
            _gameState->setMatchTieBreak( 1 );
            _gameState->setTieBreak( 1 );
            _tieBreaker.setTieBreakEnable();
        }
        else {
            _player1->setGames( _player1->getGames() );
            _gameState->setPlayer1SetHistory( _player1->getSetHistory() );
            _gameState->setPlayer2SetHistory( _player2->getSetHistory() );
            _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
            _mode1WinSequences.p2SetWinSequence();
            _tieBreaker.endTieBreak();
        }
    }
    if ( _player2->getGames() >= 10 &&
        ( _player2->getGames() - _player1->getGames() ) > 1 ) {
        _player2->setSets( _gameState, _player2->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets() ) {
            _tieBreaker.endTieBreak();
            _mode1WinSequences.enterMatchTieBreak();
            _gameState->setCurrentAction( RUNNING_MATCH_TIE_BREAK );
            _gameState->setMatchTieBreak( 1 );
            _gameState->setTieBreak( 1 );
            _tieBreaker.setTieBreakEnable();
        }
        else {
            _player1->setGames( _player1->getGames() );
            _gameState->setPlayer1SetHistory( _player1->getSetHistory() );  // set set history, set++
            _gameState->setPlayer2SetHistory( _player2->getSetHistory() );
            _gameState->setCurrentSet( _gameState->getCurrentSet() + 1 );
            _mode1WinSequences.p2SetWinSequence();
            _tieBreaker.endTieBreak();
        }
    }
}

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
        _gameState->setCurrentAction( "after player two match win" );
    }
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
}
