#include "MODE_1_SCORE.h"
#include "Arduino.h"
#include "DIGI_V6_15.h"

Mode1Score::Mode1Score( Player* player1,
    Player* player2,
    PinInterface* pinInterface,
    GameState* gameState,
    History* history )
    : _player1( player1 ),
    _player2( player2 ),
    _gameState( gameState ),
    _history( history ),
    _mode1TieBreaker( player1, player2, pinInterface, gameState, history ),
    _pointLeds( player1, player2, pinInterface ),
    _gameLeds( player1, player2, pinInterface ),
    _setLeds( player1, player2, pinInterface ),
    _mode1WinSequences( player1, player2, pinInterface, gameState ),
    _undo( player1, player2, pinInterface, gameState ) {}
Mode1Score::~Mode1Score() {}

void Mode1Score::mode1P1Score() {
    if ( _player1->getPoints() >= 3 ) {
        if ( _player1->getPoints() == _player2->getPoints() ) {
            // Tie, Back to Deuce
            _player1->setPoints( 3 );  
            _player2->setPoints( 3 );  
        } else if ( _player1->getPoints() > 3 && ( _player1->getPoints() - _player2->getPoints() ) > 1 ) {
            // Game win Scenario
            _player1->setGames( _player1->getGames() + 1 );
            _undo.memory();
            _pointLeds.updatePoints();
            mode1P1Games();
        }

        if ( _player1->getPoints() == 4 ) {
            _gameState->setPointFlash( 1 );  
            _gameState->setPreviousTime( GameTimer::gameMillis()); 
            _gameState->setToggle( 0 );                          
        }
    }
    _pointLeds.updatePoints();
}

void Mode1Score::mode1P2Score() {
    if ( _player2->getPoints() >= 3 ) {
        if ( _player2->getPoints() == _player1->getPoints() ) {  // Tie, Back to Deuce
            _player1->setPoints( 3 );                              
            _player2->setPoints( 3 );                              
        }

        // Game win Scenario
        else if ( _player2->getPoints() > 3 && ( _player2->getPoints() - _player1->getPoints()) > 1 ) {                                // Game win Scenario
            _player2->setGames( _player2->getGames() + 1 );  
            _undo.memory();                                
            _pointLeds.updatePoints();                    
            mode1P2Games();
        }

        if ( _player2->getPoints() == 4 ) {
            _gameState->setPointFlash( 1 );
            _gameState->setPreviousTime( GameTimer::gameMillis());
            _gameState->setToggle( 0 );                              
        }
    }
    _pointLeds.updatePoints(); 
}



/////////////////////////////////////// MODE 1 GAMES //////////////////////////////////////////////
void Mode1Score::mode1P1Games() { 
    _gameLeds.updateGames();
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    if ( _player1->getGames() >= 6 ) {
        if ( _player1->getGames() == 6 && _player2->getGames() == 6 ) {
            _gameState->setTieBreak( 1 );
            _mode1TieBreaker.tieBreakEnable();
        }
        if ( _gameState->getTieBreak() == 0 ) {
            if (( _player1->getGames() - _player2->getGames() ) > 1 ) { 
                _player1->setSets( _player1->getSets() + 1 );
                _setLeds.updateSets();
                if ( _player1->getSets() == _player2->getSets() ) {
                    _mode1WinSequences.p1TBSetWinSequence();
                    _gameState->setSetTieBreak( 1 );
                    _mode1TieBreaker.setTieBreakEnable();
                } else if ( _player1->getSets() == 2 ) {
                    _mode1WinSequences.p1MatchWinSequence();
                } else {
                    _mode1WinSequences.p1SetWinSequence();  
                    _setLeds.updateSets();                  
                    GameTimer::gameDelay( _gameState->getWinDelay());
                    _player1->setPoints( 0 );
                    _player2->setPoints( 0 );
                }
                _player1->setGames( 0 );
                _player2->setGames( 0 );
            } else {
                _mode1WinSequences.p1GameWinSequence(); 
                _gameLeds.updateGames();                 
                GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );  
                _player1->setPoints( 0 );
                _player2->setPoints( 0 );
            }
        }
    } else {
        _mode1WinSequences.p1GameWinSequence(); 
        _gameLeds.updateGames();                 
        GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );  
        _player1->setPoints( 0 );             
        _player2->setPoints( 0 );             
    }
}

void Mode1Score::mode1P2Games() {
    _gameLeds.updateGames();
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    if ( _player2->getGames()  >= 6 ) {
        if ( _player2->getGames()  == 6 && _player1->getGames() == 6 ) {
            _gameState->setTieBreak( 1 );        
            _mode1TieBreaker.tieBreakEnable(); 
        }
        if ( _gameState->getTieBreak() == 0 ) {
            if (( _player2->getGames() - _player1->getGames() ) > 1 ) {
                _player2->setSets( _player2->getSets() + 1 );  
                _setLeds.updateSets();
                if ( _player2->getSets() == _player1->getSets() ) {
                    _mode1WinSequences.p2TBSetWinSequence(); 
                    _gameState->setSetTieBreak( 1 );         
                    _mode1TieBreaker.setTieBreakEnable();     
                }
                else if ( _player2->getSets() == 2 ) {
                    _mode1WinSequences.p2MatchWinSequence();  
                }  else {
                    _mode1WinSequences.p2SetWinSequence();  
                    _setLeds.updateSets();                  
                    GameTimer::gameDelay( _gameState->getWinDelay());
                    _player1->setPoints( 0 ); 
                    _player2->setPoints( 0 ); 
                }
                _player1->setGames( 0 );  
                _player2->setGames( 0 );  
            }
            else {
                _mode1WinSequences.p2GameWinSequence();  
                _gameLeds.updateGames();                
                GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
                _player1->setPoints( 0 ); 
                _player2->setPoints( 0 ); 
            }
        }
    }
    else {
        _mode1WinSequences.p2GameWinSequence();  
        _gameLeds.updateGames();                 
        GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
        _player1->setPoints( 0 );
        _player2->setPoints( 0 );
    }
}
////////////////////////////////// END MODE 1 GAMES ///////////////////////////////////////////////



void Mode1Score::mode1TBP1Games() {
    _gameLeds.updateGames();
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );

    if ( _player1->getGames() == 15 ) {
        _player1->setSets( _player1->getSets() + 1 );

        if ( _player2->getSets() == _player1->getSets() ) {
            _mode1TieBreaker.endTieBreak();           
            _mode1WinSequences.p1TBSetWinSequence(); 
            _gameState->setSetTieBreak( 1 );          
            _mode1TieBreaker.setTieBreakEnable();     
        }
        else {
            _mode1WinSequences.p1SetWinSequence();  
            _mode1TieBreaker.endTieBreak();         
        }
    }

    if ( _player1->getGames() >= 10 &&
        ( _player1->getGames() - _player2->getGames() ) > 1 ) {
        _player1->setSets( _player1->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets() ) {
            _mode1TieBreaker.endTieBreak();          
            _mode1WinSequences.p1TBSetWinSequence();  
            _gameState->setSetTieBreak( 1 );           
            _mode1TieBreaker.setTieBreakEnable();     
        }
        else {
            _mode1WinSequences.p1SetWinSequence();  
            _mode1TieBreaker.endTieBreak();         
        }
    }
}

void Mode1Score::mode1SetTBP1Games() {
    _gameLeds.updateGames();
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );

    if ( _player1->getGames() == 7 ) {
        _player1->setSets( _player1->getSets() + 1 );
        _setLeds.updateSets();                       
        GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
        _mode1WinSequences.p1SetTBWinSequence();  
        _mode1TieBreaker.tieLEDsOff();            
        _mode1WinSequences.p1MatchWinSequence();  
    }
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );
}

void Mode1Score::mode1TBP2Games() {
    _gameLeds.updateGames();  
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 ); 
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );

    if ( _player2->getGames() == 15 ) {
        _player2->setSets( _player2->getSets() + 1 );

        if ( _player2->getSets() == _player1->getSets() ) {
            _mode1TieBreaker.endTieBreak();           
            _mode1WinSequences.p2TBSetWinSequence();  
            _gameState->setSetTieBreak( 1 );           
            _mode1TieBreaker.setTieBreakEnable();    
        }
        else {
            _mode1WinSequences.p2SetWinSequence(); 
            _mode1TieBreaker.endTieBreak();         
        }
    }

    if ( _player2->getGames() >= 10 &&
        ( _player2->getGames() - _player1->getGames() ) > 1 ) {
        _player2->setSets( _player2->getSets() + 1 );
        if ( _player2->getSets() == _player1->getSets() ) {
            _mode1TieBreaker.endTieBreak();           
            _mode1WinSequences.p2TBSetWinSequence();
            _gameState->setSetTieBreak( 1 );         
            _mode1TieBreaker.setTieBreakEnable(); 
        }
        else {
            _mode1WinSequences.p2SetWinSequence();
            _mode1TieBreaker.endTieBreak();         
        }
    }
}

void Mode1Score::mode1SetTBP2Games() {
    _gameLeds.updateGames();
    GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
    if ( _player2->getGames() == 7 ) {
        _player2->setSets( _player2->getSets() + 1 );  
        _setLeds.updateSets();                      
        GameTimer::gameDelay( UPDATE_DISPLAY_DELAY );
        _mode1WinSequences.p2SetTBWinSequence();  
        _mode1TieBreaker.tieLEDsOff();           
        _mode1WinSequences.p2MatchWinSequence();  
    }
    _gameState->setServeSwitch( _gameState->getServeSwitch() + 1 );  
}
