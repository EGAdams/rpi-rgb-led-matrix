#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "../ConsoleDisplay/ConsoleDisplay.h"
#include "../IDisplay/IDisplay.h"
#include "../ITextDrawer/ITextDrawer.h"
#include "../FontManager/FontManager.h"
#include "../ColorManager/ColorManager.h"
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
#include <string>
#include <fstream>
#include <memory>
#include <thread> // Include the <thread> header for sleep function

#define PLAYER_1_SERVE  0
#define PLAYER_2_SERVE  1
#define PLAYER_1_SCORE  1
#define PLAYER_2_SCORE  2
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
    ScoreBoard(Player* player1, Player* player2, GameState* gameState, IDisplay* display,
               FontManager* fontManager, ColorManager* colorManager);
    ~ScoreBoard();
    void update();
    bool hasCanvas();
    void clearScreen();
    void drawGreenPeriod();
    void drawRedPeriod();
    void drawYellowPeriod();
    void drawBlankPeriod();
    void drawGames();
    void drawSets();
    void drawBlinkSets( int player_number );
    void writeMessage( std::string message );
    void drawText(    const std::string& text, int x, int y );
    void drawNewText( const std::string& text, int x, int y );
    std::string drawPlayerScore(   Player* player );
    RGBMatrix* getCanvas();
    Color _getColor( int color );
    bool onRaspberryPi();
    void blink_player_score( int player );
    void setDrawerForegroundColor( const Color& color );
    void setDrawerBackgroundColor( const Color& color );
    void setDrawerFont( const rgb_matrix::Font* font  );
    void setDrawerSize( Drawer::Size            size  );
    void setFontFile(   const char*             font_file  );
    void setLittleDrawerFont( const std::string& font_file );

private:
    Player*                      _player1;
    Player*                      _player2;
    GameState*                   _gameState;
    IDisplay*                    _display;
    FontManager*                 _fontManager;
    ColorManager*                _colorManager;
    ITextDrawer*                  _textDrawer;
    rgb_matrix::Font             _big_number_font;
    rgb_matrix::Font             _little_number_font;
    rgb_matrix::Font             _period_font;
    std::unique_ptr<Drawer>      _drawer;
    std::unique_ptr<Drawer>      _smallDrawer;
    std::unique_ptr<Drawer>      _playerOneScoreDrawer;
    std::unique_ptr<Drawer>      _playerTwoScoreDrawer;
    std::unique_ptr<Drawer>      _pipeDrawer;
    std::unique_ptr<Drawer>      _bluePipeDrawer;
    std::unique_ptr<Drawer>      _redPipeDrawer;
    std::unique_ptr<Drawer>      _greenPipeDrawer;
    std::unique_ptr<Drawer>      _blankPipeDrawer;
    std::unique_ptr<Drawer>      _redPeriodDrawer;
    std::unique_ptr<Drawer>      _greenPeriodDrawer;
    std::unique_ptr<Drawer>      _yellowPeriodDrawer;
    std::unique_ptr<Drawer>      _blankPeriodDrawer;
    std::unique_ptr<SetDrawer>   _setDrawer;
    std::unique_ptr<RGBMatrix>   _canvas;
    std::unique_ptr<Drawer>      _new_drawer;
    Drawer*                      _text_drawer;
    const char*                  _font_file;

    void _drawTieBreakerBar(    /* void */            );
    void _drawMatchWinDisplay(  /* void */            );
    int  _characterOffset(   std::string character    );
    std::string _translate(  int raw_score            );
};

#endif
