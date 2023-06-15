#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "../Player/Player.h"
#include "../GameState/GameState.h"
#include "../NumberDrawer/NumberDrawer.h"
#include "../CanvasCreator/CanvasCreator.h"
#include "../FontLoader/FontLoader.h"
#include "../SetDrawer/SetDrawer.h"
#include <string.h>
#include <stdio.h>
#include <iostream>

#define PLAYER_1_SERVE  0
#define PLAYER_2_SERVE  1
#define BIG_NUMBER_FONT "fonts/fgm_27_ee.bdf"

class ScoreBoard {
public:
    ScoreBoard( Player* player1, Player* player2, GameState* gameState );       
    ~ScoreBoard();
    void update();
    bool hasCanvas();
    void clearScreen();
    void drawGames();
    
private:
    Player*          _player1;
    Player*          _player2;
    GameState*       _gameState;
    NumberDrawer*    _smallNumberDrawer;
    NumberDrawer*    _bigNumberDrawer;
    NumberDrawer*    _pipeDrawer;
    rgb_matrix::Font _big_number_font;
    SetDrawer*       _setDrawer;
    RGBMatrix* _canvas;

    void _drawPlayerScore(  Player* player        );
    void _drawPlayerSets(   Player* player        );
    int  _characterOffset(  std::string character );
    std::string _translate( int raw_score         ); };
#endif