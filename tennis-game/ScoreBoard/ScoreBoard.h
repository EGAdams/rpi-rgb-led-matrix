#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "../GameTimer/GameTimer.h"
#include "../Player/Player.h"
#include "../GameState/GameState.h"
#include "../Drawer/Drawer.h"
#include "../CanvasCreator/CanvasCreator.h"
#include "../FontLoader/FontLoader.h"
#include "../SetDrawer/SetDrawer.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <memory>

#define PLAYER_1_SERVE  0
#define PLAYER_2_SERVE  1
#define BIG_NUMBER_FONT    "fonts/fgm_27_ee.bdf"
#define LITTLE_NUMBER_FONT "fonts/little_numbers.bdf"
#define RED    2
#define GREEN  5
#define BLUE   6
#define YELLOW 4
#define ORANGE 3
#define PURPLE 7
#define WHITE  9
#define BLACK  0

class ScoreBoard {
public:
    ScoreBoard( Player* player1, Player* player2, GameState* gameState );
    ~ScoreBoard();
    void update();
    bool hasCanvas();
    void clearScreen();
    void drawGames();
    void writeMessage( std::string message );
    void drawText( std::string message, int color, int x, int y );
    std::string drawPlayerScore(   Player* player );
    RGBMatrix* getCanvas();
    Color _getColor( int color );

private:
    Player*                        _player1;
    Player*                        _player2;
    GameState*                     _gameState;
    rgb_matrix::Font               _big_number_font;
    std::unique_ptr<Drawer>  _drawer;
    std::unique_ptr<Drawer>  _smallDrawer;
    std::unique_ptr<Drawer>  _playerOneScoreDrawer;
    std::unique_ptr<Drawer>  _playerTwoScoreDrawer;
    std::unique_ptr<Drawer>  _pipeDrawer;
    std::unique_ptr<Drawer>  _bluePipeDrawer;
    std::unique_ptr<SetDrawer>     _setDrawer;
    std::unique_ptr<RGBMatrix>     _canvas;

    void _drawTieBreakerBar( /* void */            );
    int  _characterOffset(   std::string character );
    std::string _translate(  int raw_score         ); };
#endif
