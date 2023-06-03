#include "ScoreBoard.h"

ScoreBoard::ScoreBoard(Player* player1, Player* player2, GameState* gameState ) 
: _player1(player1), _player2(player2), _gameState(gameState) {
    printf("Constructing ScoreBoard...\n");

    // seems like the only logical place to create the canvas
    CanvasCreator canvasCreator(matrix_options, runtime_opt);
    RGBMatrix* canvas = canvasCreator.CreateCanvas();

    FontLoader fontLoader("fonts/mspgothic_042623.bdf"); // Load Fonts
    rgb_matrix::Font font;
    fontLoader.LoadFont(font);

    FontLoader bigNumberFontLoader("fonts/fgm_27_ee.bdf");
    rgb_matrix::Font bigNumberFont;
    bigNumberFontLoader.LoadFont(bigNumberFont);
    if (!_big_number_font.LoadFont( BIG_NUMBER_FONT )) { 
        fprintf( stderr, "Couldn't load font '%s'\n", BIG_NUMBER_FONT );
        exit( 1 ); }
    Color color( 255, 255, 0 );
    Color bg_color( 0, 0, 0);
    _bigNumberDrawer = new NumberDrawer( canvas, &_big_number_font, NumberDrawer::BIG, color, bg_color);
    _pipeDrawer      = new NumberDrawer( canvas, &_big_number_font, NumberDrawer::BIG, color, bg_color);
}

void ScoreBoard::update() {
    _drawPlayerScore( _player1 );
    _drawPlayerScore( _player2 );
}

void ScoreBoard::_drawPlayerScore(Player* player) {
    int vertical_offset = player->number() == 1 ? 0 : _big_number_font.height();
    std::string serve_bar = _gameState->getServe() == PLAYER_2_SERVE ? " " : "I";
    _pipeDrawer->DrawNumber(serve_bar, 1, _big_number_font.baseline());
    std::string score = _translate(player->getPoints());
    _bigNumberDrawer->DrawNumber(score.substr(0, 1), 16, _big_number_font.baseline() + vertical_offset);
    _bigNumberDrawer->DrawNumber(score.substr(1, 1), 38, _big_number_font.baseline() + vertical_offset);
}


std::string ScoreBoard::_translate(int raw_score) {
    switch (raw_score) {
    case 0: return "00";
    case 1: return "10";
    case 2: return "15";
    case 3: return "30";
    case 4: return "40";
    case 5: return "Ad";
    case 99: return "Ad";
    default: return "00";
    }
}

void _showLittleNumbers( rgb_matrix::Canvas *canvas ) {
    #define LITTLE_NUMBER_FONT "fonts/little_numbers.bdf"
    #define SPACE_BEFORE_SMALL_NUMBER   7
    #define SPACE_BETWEEN_SMALL_NUMBERS 17
    #define START_ROW                   86
    Color background_color(0, 0, 0);
    int letter_spacing = 0;
    rgb_matrix::Font little_number_font;
    if (!little_number_font.LoadFont( LITTLE_NUMBER_FONT )) {
        fprintf( stderr, "Couldn't load font '%s'\n", LITTLE_NUMBER_FONT );
        exit( 1 ); }
    int x = 0;
    int y = START_ROW;
    rgb_matrix::Font *outline_font = NULL;
    Color thirdRowColor( 0, 255, 0 );
    rgb_matrix::DrawText( canvas, little_number_font, x + SPACE_BEFORE_SMALL_NUMBER, y + little_number_font.baseline(), thirdRowColor,  outline_font ? NULL : &background_color, "0 0 0", letter_spacing );
    y += little_number_font.height() - 5;
    Color fourthRowColor( 255, 0, 0 );
    rgb_matrix::DrawText( canvas, little_number_font, x + SPACE_BEFORE_SMALL_NUMBER, y + little_number_font.baseline(), fourthRowColor, outline_font ? NULL : &background_color, "0", letter_spacing );
    rgb_matrix::DrawText( canvas, little_number_font, x + SPACE_BEFORE_SMALL_NUMBER + SPACE_BETWEEN_SMALL_NUMBERS, y + little_number_font.baseline(), fourthRowColor, outline_font ? NULL : &background_color, "0", letter_spacing );
    rgb_matrix::DrawText( canvas, little_number_font, x + SPACE_BEFORE_SMALL_NUMBER + (( 2 * SPACE_BETWEEN_SMALL_NUMBERS )), y + little_number_font.baseline(), fourthRowColor, outline_font ? NULL : &background_color, "0", letter_spacing );
}