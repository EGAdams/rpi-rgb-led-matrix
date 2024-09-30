```cpp
class IDisplay {
public:
    virtual void setColor( const std::string& colorName ) = 0;
    virtual void drawText( const std::string& text, int x, int y ) = 0;
    virtual void clearScreen() = 0;
    virtual ~IDisplay() = default;
}
```

```cpp


void printFilledLarge1() {
```

```cpp


void printFilledLarge10() {
```

```cpp
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
    void drawText( const std::string& text, int x, int y );
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
    const char*                 _font_file;

    void _drawTieBreakerBar(    /* void */            );
    void _drawMatchWinDisplay(  /* void */            );
    int  _characterOffset(   std::string character    );
    std::string _translate(  int raw_score            );
}
```

```cpp


void printFilledLargePipe10() {
```

```cpp

    : _player1( player1 ), _player2( player2 ), _gameState( gameState ),
    _display( display ), _fontManager( fontManager ), _colorManager( colorManager ) {
    if ( onRaspberryPi() == false ) {
        std::string brightness_string;
        if ( brightness_file.is_open() ) {
        if ( _gameState->getMatchBlink() == 1 ) {
            if ( strcmp( _gameState->getCurrentAction().c_str(), RUNNING_MATCH_WIN_SEQUENCE ) == 0 ) {
```

```cpp


void printFilledLarge1() {
```

