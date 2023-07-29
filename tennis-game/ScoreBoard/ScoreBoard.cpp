#include "ScoreBoard.h"

ScoreBoard::ScoreBoard( IPlayer* player1, IPlayer* player2, GameState* gameState ): 
    _player1( player1 ), _player2( player2 ), _gameState( gameState ) {
    printf( "Constructing ScoreBoard...\n" );
    if ( MATRIX_DISABLED == 1 ) { 
        printf( "MATRIX_DISABLED == 1 is true.  Skipping matrix setup...\n" );
    } else {
        printf( "MATRIX_DISABLED == 1 is false.  Setting up matrix...\n" );
        Color pipe_color( 255, 255, 0 ); // yellow
        Color background_color( 0, 0, 0 );
        Color player_one_score_color( 0, 255, 0 ); // green
        Color player_two_score_color( 255, 0, 0 ); // red
        Color outline_color( 0, 0, 0 );
        RGBMatrix::Options matrix_options; // seems like the only logical place to create the canvas
        matrix_options.hardware_mapping = "regular";  // or e.g. "adafruit-hat"
        matrix_options.pixel_mapper_config = "Rotate:180;264-Mapper"; // or e.g. "U-mapper"
        matrix_options.rows = 64;
        matrix_options.cols = 64;
        matrix_options.chain_length = 1;
        matrix_options.parallel = 2;
        matrix_options.show_refresh_rate = false;
        matrix_options.disable_hardware_pulsing = true; // --led-no-hardware-pulse
        matrix_options.brightness = 35; // 35 is best for demo videos in largo
        matrix_options.pwm_bits = 11;
        matrix_options.multiplexing = 1;
        rgb_matrix::RuntimeOptions runtime_opt;
        runtime_opt.drop_privileges = 0;
        runtime_opt.gpio_slowdown = 2;
        runtime_opt.daemon = 0;
        runtime_opt.do_gpio_init = 1;
        printf( "Matrix options:\n" );
        printf( "  rows: %d\n", matrix_options.rows );
        printf( "  chain_length: %d\n", matrix_options.chain_length );
        printf( "  parallel: %d\n", matrix_options.parallel );
        printf( "  pwm_bits: %d\n", matrix_options.pwm_bits );
        printf( "  pwm_lsb_nanoseconds: %d\n", matrix_options.pwm_lsb_nanoseconds );
        printf( "Runtime options:\n" );
        printf( "  daemon: %d\n", runtime_opt.daemon );
        printf( "  do_gpio_init: %d\n", runtime_opt.do_gpio_init );
        printf( "  drop_privileges: %d\n", runtime_opt.drop_privileges );
        printf( "  gpio_slowdown: %d\n", runtime_opt.gpio_slowdown );
        CanvasCreator canvasCreator( matrix_options, runtime_opt );
        _canvas = std::unique_ptr<rgb_matrix::RGBMatrix>( canvasCreator.createCanvas());
        FontLoader fontLoader( "fonts/mspgothic_042623.bdf" ); // Load Fonts
        rgb_matrix::Font font;
        fontLoader.LoadFont( font );
        FontLoader bigNumberFontLoader( "fonts/fgm_27_ee.bdf" );
        rgb_matrix::Font bigNumberFont;
        bigNumberFontLoader.LoadFont( bigNumberFont );
        if (!_big_number_font.LoadFont( BIG_NUMBER_FONT )) { 
            fprintf( stderr, "Couldn't load font '%s'\n", BIG_NUMBER_FONT ); exit( 1 );}
        Color color( 255, 255, 0 );
        Color bg_color( 0, 0, 0 );
        _playerOneScoreDrawer   = std::make_unique<NumberDrawer>( 
            _canvas.get(), &_big_number_font, NumberDrawer::BIG, player_one_score_color, bg_color );
        _playerTwoScoreDrawer   = std::make_unique<NumberDrawer>( 
            _canvas.get(), &_big_number_font, NumberDrawer::BIG, player_two_score_color, bg_color );
        
        _smallNumberDrawer = std::make_unique<NumberDrawer>( _canvas.get(), &_big_number_font, NumberDrawer::BIG, color, bg_color );
        _pipeDrawer        = std::make_unique<NumberDrawer>( _canvas.get(), &_big_number_font, NumberDrawer::BIG, color, bg_color ); 
        _setDrawer         = std::make_unique<SetDrawer>(    _canvas.get(), _gameState                                            ); 
        } // fi
    update(); 
}

ScoreBoard::~ScoreBoard() {
    std::cout << "destroying ScoreBoard..." << std::endl;
    if ( _canvas != NULL ) {
        std::cout << "NOT deleting _canvas..." << std::endl;
        // delete _canvas.get(); // this causes some error.  only one scoreBoard is created anyway.
    } else { std::cout << "*** WARNING: _canvas == NULL, not deleting. ***" << std::endl; }}

void ScoreBoard::drawGames() {  std::cout << "inside ScoreBoard::drawGames()" << std::endl; }

bool ScoreBoard::hasCanvas() { 
    if ( _canvas != NULL ) { return true;
    } else { std::cout << "*** WARNING: canvas is NULL ***" << std::endl; return false; }}

void ScoreBoard::update() {
    std::cout << "gamestate current action: " << _gameState->getCurrentAction() << std::endl;
    clearScreen();
    std::cout << "inside ScoreBoard::update()  player1 points: " << _player1->getPoints() << std::endl;
    std::cout << "inside ScoreBoard::update()  player2 points: " << _player2->getPoints() << std::endl;
    _drawPlayerScore( _player1 ); _drawPlayerScore( _player2 );
    
    if ( MATRIX_DISABLED == 1 ) {
        // std::cout << "MATRIX_DISABLED == 1 is true.  skipping blink..." << std::endl;
    } else {
        bool blink = _gameState->getCurrentAction().find( "blink" ) != std::string::npos;
        if ( blink ) {
            std::cout << "blink is true, calling _setDrawer->drawBlinkSets()..." << std::endl;
            int playerToBlink = _gameState->getCurrentAction().find( "player1" ) != std::string::npos ? 
                PLAYER_1_INITIALIZED : PLAYER_2_INITIALIZED;
            _setDrawer->drawBlinkSets( playerToBlink );
        } else { _setDrawer->drawSets(); }}}

void ScoreBoard::clearScreen() { 
    if ( MATRIX_DISABLED == 1 ) {
        // std::cout << "clearScreen called, no matrix." << std::endl;
    } else {
        if ( !hasCanvas()) { std::cout << "*** ERROR: canvas == NULL.  exiting... ***" << std::endl; exit( 1 ); }
        Color flood_color( 0, 0, 0 ); _canvas->Fill( flood_color.r, flood_color.g, flood_color.b ); }}

void ScoreBoard::_drawPlayerScore( IPlayer* player ) {
    std::string serve_bar = _gameState->getServe() == player->number() ? "I" : " "; // or p1 serve and swap
    std::string score = _translate( player->getPoints());
    if( MATRIX_DISABLED == 1 ) {
        player->number() == PLAYER_1_INITIALIZED ?  // type player 1 score, else type player 2 score
        std::cout << "PLAYER 1: ////// " << serve_bar << " " << score << " ////// " << std::endl :
        std::cout << "PLAYER 2: ////// " << serve_bar << " " << score << " ////// " << std::endl;
    } else {
        int vertical_offset = player->number() == 0 ? 0 : _big_number_font.height();
        _pipeDrawer->DrawNumber(serve_bar, 1, _big_number_font.baseline() + vertical_offset ); // draw pipe
        int baseline = _big_number_font.baseline();                     // set the coordinates for the text
        int first_offset  = _characterOffset( score.substr( 0, 1 ));
        int second_offset = _characterOffset( score.substr( 1, 1 ));    // then draw text depending on player
        if( player->number() == PLAYER_1_INITIALIZED ) {
            _playerOneScoreDrawer->DrawNumber( score.substr( 0, 1 ), first_offset  + 16, baseline + vertical_offset );
            _playerOneScoreDrawer->DrawNumber( score.substr( 1, 1 ), second_offset + 38, baseline + vertical_offset );
        } else {
            _playerTwoScoreDrawer->DrawNumber( score.substr( 0, 1 ), first_offset  + 16, baseline + vertical_offset );
            _playerTwoScoreDrawer->DrawNumber( score.substr( 1, 1 ), second_offset + 38, baseline + vertical_offset ); }}}

int ScoreBoard::_characterOffset( std::string character ) {
    int char_offset = 0;
    if ( character == "A" ) {
        return -4;
    } else if ( character == "d" ) {
        return 0;
    } else { char_offset = std::stoi( character ); }

    switch ( char_offset ) {
    case 3: return -1;
    case 4: return -4;
    case 5: return -1;
    default: return 0; }}

std::string ScoreBoard::_translate( int raw_score ) {
    switch ( raw_score ) {
    case 0:               return "00";
    case 1:               return "15";
    case 2:               return "30";
    case 3:               return "40";
    case SCORE_CASE_4:    return "Ad"; // jul20 TODO: just a test!
    case SCORE_CASE_5:    return "Ad";
    case UNDEFINED_SCORE: return "99";
    default:              return "00"; }}