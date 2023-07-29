#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "../GameTimer/GameTimer.h"
#include "../Player/Player.h"
#include "../GameState/GameState.h"
#include "../NumberDrawer/NumberDrawer.h"
#include "../CanvasCreator/CanvasCreator.h"
#include "../FontLoader/FontLoader.h"
#include "../SetDrawer/SetDrawer.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <memory>

#define PLAYER_1_SERVE  0
#define PLAYER_2_SERVE  1
#define BIG_NUMBER_FONT "fonts/fgm_27_ee.bdf"

class ScoreBoard {
public:
    ScoreBoard( IPlayer* player1, IPlayer* player2, GameState* gameState );       
    ~ScoreBoard();
    void update();
    bool hasCanvas();
    void clearScreen();
    void drawGames();
    
private:
    IPlayer*                        _player1;
    IPlayer*                        _player2;
    GameState*                     _gameState;
    rgb_matrix::Font               _big_number_font;
    std::unique_ptr<NumberDrawer>  _smallNumberDrawer;
    std::unique_ptr<NumberDrawer>  _playerOneScoreDrawer;
    std::unique_ptr<NumberDrawer>  _playerTwoScoreDrawer;
    std::unique_ptr<NumberDrawer>  _pipeDrawer;
    std::unique_ptr<SetDrawer>     _setDrawer;
    std::unique_ptr<RGBMatrix>     _canvas;

    void _drawPlayerScore(  IPlayer* player        );
    int  _characterOffset(  std::string character );
    std::string _translate( int raw_score         ); };
#endif