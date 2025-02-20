#include "ScoreBoard.h"

ScoreBoard::ScoreBoard( Player* player1, Player* player2, GameState* gameState, IDisplay* display,
                       FontManager* fontManager, ColorManager* colorManager )
    : _player1( player1 ), _player2( player2 ), _gameState( gameState ),
    _display( display ), _fontManager( fontManager ), _colorManager( colorManager ) {
    if ( onRaspberryPi() == false ) {
        std::cout << "constructing scoreboard without matrix..." << std::endl;
        _setDrawer = std::make_unique<SetDrawer>( _canvas.get(), _gameState );
    }
    else {
        // const rgb_matrix::Font& defaultFont = _fontManager->getFont( "fonts/fgm_27_ee.bdf" );
        // rgb_matrix::Color defaultColor = _colorManager->getColor( "WHITE" );
        _font_file = LITTLE_NUMBER_FONT;
        printf( "setting up matrix...\n" );
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
        std::ifstream brightness_file( "smart_menu/brightness.txt" );
        std::string brightness_string;
        bool brightness_file_read = false;
        if ( brightness_file.is_open()) {
            std::getline( brightness_file, brightness_string );
            brightness_file.close();
            brightness_file_read = true;
        }
        int brightness_value = 35;
        if ( brightness_file_read ) {
            brightness_value = std::stoi( brightness_string );
        }
        print( "done checking brightness file" );
        matrix_options.brightness = brightness_value; // 35; // 85; // inc jan 22, 22024 // 35 is best for demo videos in largo
        matrix_options.pwm_bits = 11;
        matrix_options.multiplexing = 1;
        rgb_matrix::RuntimeOptions runtime_opt;
        runtime_opt.drop_privileges = 0;
        runtime_opt.gpio_slowdown = 2;
        runtime_opt.daemon = 0;
        runtime_opt.do_gpio_init = 1;
        CanvasCreator canvasCreator( matrix_options, runtime_opt );
        _canvas = std::unique_ptr<rgb_matrix::RGBMatrix>( canvasCreator.createCanvas());

        // start loading fonts...
        FontLoader smallNumberFontLoader( "fonts/mspgothic_042623.bdf" );       // little numbers
        rgb_matrix::Font small_number_font;
        smallNumberFontLoader.LoadFont( small_number_font );
        # define FONT_FILE "fonts/8x13B.bdf"
        if ( !_little_number_font.LoadFont( FONT_FILE )) { // massacre aniversary 090724
            fprintf( stderr, "*** ERROR: Could not load font '%s' ***\n", FONT_FILE ); exit( 1 );
        }
        // if ( !_little_number_font.LoadFont( LITTLE_NUMBER_FONT )) {
        //     fprintf( stderr, "*** ERROR: Could not load font '%s' ***\n", LITTLE_NUMBER_FONT ); exit( 1 );
        // }

        // FontLoader bigNumberFontLoader( "fonts/fgm_27_ee.bdf" );     // big numbers
        FontLoader bigNumberFontLoader( "fonts/jan_8_bar.bdf" );     // big numbers
        rgb_matrix::Font bigNumberFont;
        bigNumberFontLoader.LoadFont( bigNumberFont );
        if ( !_big_number_font.LoadFont( BIG_NUMBER_FONT )) {
            fprintf( stderr, "Couldn't load font '%s'\n", BIG_NUMBER_FONT ); exit( 1 );
        }

        FontLoader periodFontLoader( "fonts/mspgothic_030623.bdf" ); // that period
        periodFontLoader.LoadFont( _period_font );
        if ( !_period_font.LoadFont( "fonts/mspgothic_030623.bdf" )) {
            fprintf( stderr, "*** ERROR: Couldn't load font '%s'\n", "fonts/mspgothic_030623.bdf ***" ); exit( 1 );
        }
        else {
            std::cout << "loaded period font." << std::endl;
        }
        // end loading fonts

        Color color( 255, 255, 0 );
        Color bg_color( 0, 0, 0 );
        Color blue_color( 0, 0, 255 );
        Color red_color( 255, 0, 0 );
        Color green_color( 0, 255, 0 );
        Color yellow_color( 255, 255, 0 );
        Color black_color( 0, 0, 0 );
        _playerOneScoreDrawer = std::make_unique<Drawer>(
            _canvas.get(), &_big_number_font, Drawer::BIG, player_one_score_color, bg_color );
        _playerTwoScoreDrawer = std::make_unique<Drawer>(
            _canvas.get(), &_big_number_font, Drawer::BIG, player_two_score_color, bg_color );

        _drawer = std::make_unique<Drawer>( _canvas.get(), &_big_number_font, Drawer::SMALL, color, bg_color );
        _new_drawer = std::make_unique<Drawer>( _canvas.get(), &_little_number_font, Drawer::BIG, color, bg_color );
        _pipeDrawer = std::make_unique<Drawer>( _canvas.get(), &_big_number_font, Drawer::BIG, color, bg_color );
        _bluePipeDrawer = std::make_unique<Drawer>( _canvas.get(), &_big_number_font, Drawer::BIG, blue_color, bg_color );
        _redPipeDrawer = std::make_unique<Drawer>( _canvas.get(), &_big_number_font, Drawer::BIG, red_color, bg_color );
        _greenPipeDrawer = std::make_unique<Drawer>( _canvas.get(), &_big_number_font, Drawer::BIG, green_color, bg_color );
        _blankPipeDrawer = std::make_unique<Drawer>( _canvas.get(), &_big_number_font, Drawer::BIG, black_color, bg_color );
        _redPeriodDrawer = std::make_unique<Drawer>( _canvas.get(), &_period_font, Drawer::BIG, red_color, bg_color );
        _greenPeriodDrawer = std::make_unique<Drawer>( _canvas.get(), &_period_font, Drawer::BIG, green_color, bg_color );
        _yellowPeriodDrawer = std::make_unique<Drawer>( _canvas.get(), &_period_font, Drawer::BIG, yellow_color, bg_color );
        _blankPeriodDrawer = std::make_unique<Drawer>( _canvas.get(), &_period_font, Drawer::BIG, black_color, bg_color );
        _text_drawer = new Drawer( _canvas.get(), &_little_number_font, Drawer::SMALL, yellow_color, bg_color );
        _setDrawer = std::make_unique<SetDrawer>( _canvas.get(), _gameState );
        print( "done constructing unique pointers.  updating scoreboard..." );
    } // fi onRaspberryPi
}

ScoreBoard::~ScoreBoard() {
    // std::cout << "destroying ScoreBoard..." << std::endl;
    if ( _canvas != NULL ) {
        std::cout << "NOT deleting _canvas..." << std::endl;
        delete _display;
    }
    else { /* std::cout << "*** WARNING: _canvas == NULL, not deleting. ***" << std::endl; */ }
}

void ScoreBoard::flashRemote( int current_remote_button ) {
    if ( current_remote_button == GREEN_REMOTE_GREEN_SCORE || 
         current_remote_button == GREEN_REMOTE_RED_SCORE ||
         current_remote_button == GREEN_REMOTE_UNDO ) {
        drawGreenPeriod();
        GameTimer::gameDelay( BUTTON_LED_FLASH_DELAY );
        drawBlankPeriod();
    } else if ( current_remote_button == UMPIRE_REMOTE_GREEN_SCORE  ||
                current_remote_button == UMPIRE_REMOTE_RED_SCORE    ||
                current_remote_button == UMPIRE_REMOTE_UNDO ) {
        print( "*** TODO: draw red period ***" );
        GameTimer::gameDelay( BUTTON_LED_FLASH_DELAY );
        drawBlankPeriod();
    } else if ( current_remote_button == RED_REMOTE_GREEN_SCORE || 
                current_remote_button == RED_REMOTE_RED_SCORE ||
                current_remote_button == RED_REMOTE_UNDO ) {
        drawRedPeriod();
        GameTimer::gameDelay( BUTTON_LED_FLASH_DELAY );
        drawBlankPeriod();
    } else {
        print ( "*** Warning: unhandled button press ***" );
    }
}

void ScoreBoard::setFontFile( const char* font_file_arg ) { _font_file = font_file_arg; }

void ScoreBoard::drawText( const std::string& message, int x, int y ) {
    _drawer->drawText( message, x, y );
}

void ScoreBoard::drawNewText( const std::string& message, int x, int y ) {
    if ( onRaspberryPi() == false ) {
        print( message );
    } else {
        _text_drawer->drawText( message, x, y );    
    }
}

Color ScoreBoard::_getColor( int color_constant ) {
    switch ( color_constant ) {
    case RED:    return Color( 255, 0, 0 );
    case GREEN:  return Color( 0, 255, 0 );
    case BLUE:   return Color( 0, 0, 255 );
    case YELLOW: return Color( 255, 255, 0 );
    case ORANGE: return Color( 255, 165, 0 );
    case PURPLE: return Color( 128, 0, 128 );
    case WHITE:  return Color( 255, 255, 255 );
    case BLACK:  return Color( 0, 0, 0 );
    default:       return Color( 0, 0, 0 );
    }
}

void ScoreBoard::writeMessage( std::string message ) {
    if ( hasCanvas() == false ) {
        std::cout << "/// " << message << " ///" << std::endl;
    }
    else {
        Color color( 255, 255, 0 );
        Color bg_color( 0, 0, 0 );
        int baseline = _big_number_font.baseline();            // set the coordinates for the text
        int first_offset = 2;
        _drawer->drawNumber( message, first_offset, baseline + _big_number_font.height());
    }
}

void ScoreBoard::drawGames() { /* std::cout << "inside ScoreBoard::drawGames()" << std::endl; */ }

RGBMatrix* ScoreBoard::getCanvas() { return _canvas.get(); }

void ScoreBoard::drawSets() {
    _setDrawer->drawSets();
}

void ScoreBoard::drawBlinkSets( int player_number ) { _setDrawer->drawBlinkSets( player_number ); }

bool ScoreBoard::hasCanvas() {
    if ( _canvas != NULL ) { return true;
    } else { /* std::cout << "*** WARNING: canvas is NULL ***" << std::endl; */ return false; }}

void ScoreBoard::update() {
    bool tie_break = _gameState->getTieBreak();
    bool match_tie_break = _gameState->getMatchTieBreak();
    bool tie_break_on = tie_break || match_tie_break;
    clearScreen();
    drawPlayerScore( _player1 );
    drawPlayerScore( _player2 );
    if ( hasCanvas() == false ) { std::cout << "==========================" << std::endl; } // only for terminal
    bool blink = _gameState->getCurrentAction().find( "blink" ) != std::string::npos;
    if ( blink ) {
        int playerToBlink = _gameState->getCurrentAction().find( "player1" ) != std::string::npos ?
            PLAYER_1_INITIALIZED : PLAYER_2_INITIALIZED;
        print( "playerToBlink: " + std::to_string( playerToBlink ));
        _setDrawer->drawBlinkSets( playerToBlink ); // checks current action ignoring playerToBlink
    }

    if ( _gameState->getCurrentAction() == DRAW_BLANK_SETS ) { 
        print( "drawing blank sets..." );
        blankTieBreakerBar();
        _setDrawer->blankSets();
    } else {
        _setDrawer->drawSets();
    }

    if ( tie_break_on ) { drawTieBreakerBar();}

    else if ( _gameState->getCurrentAction() == RUNNING_MATCH_WIN_SEQUENCE ) {
        _drawMatchWinDisplay();
    } else { /* print( "tie break is false, not calling _drawTieBreakerBar()..."); */ }
}

void ScoreBoard::_drawMatchWinDisplay() {
    const std::string yellow = "\033[93m";
    const std::string reset = "\033[0m";
    const std::string bright_green = "\033[92m";  // Bright green
    const std::string blue = "\033[94m";  // Blue
    if ( onRaspberryPi() == false ) {
        if ( _gameState->getMatchBlink() == 1 ) {
            if ( strcmp( _gameState->getCurrentAction().c_str(), RUNNING_MATCH_WIN_SEQUENCE ) == 0 ) {
                std::cout << bright_green << "/// *** !!! MATCH WIN !!! *** ///\n" << reset << std::endl;
            } else {
                std::cout << "\n" << reset << std::endl;
            }
        }
    } else {
        // _bluePipeDrawer->drawNumber( "I", BLUE_BAR_HORIZONTAL_OFFSET, BLUE_BAR_VERTICAL_OFFSET ); // draw pipe
    }
}

void ScoreBoard::drawTieBreakerBar() {
    const std::string reset = "\033[0m";
    const std::string blue = "\033[94m";  // Blue
    if ( onRaspberryPi() == false ) {
        if ( _gameState->getTieLEDsOn() == 1 ) {
            if ( _gameState->getMatchTieBreak()) {
                std::cout << blue << "/// MATCH TIE BREAK MODE ///\n" << reset << std::endl;
            }
            else {
                std::cout << blue << "/// TIE BREAK MODE ///\n" << reset << std::endl;
            }
        }
    } else {                             // constants are not working here.  103024
        _bluePipeDrawer->drawNumber( "I", BLUE_BAR_HORIZONTAL_OFFSET, BLUE_BAR_VERTICAL_OFFSET );// BLUE_BAR_HORIZONTAL_OFFSET, BLUE_BAR_VERTICAL_OFFSET ); // draw pipe
    }
}

void ScoreBoard::blankTieBreakerBar() {
    const std::string reset = "\033[0m";
    const std::string blue = "\033[94m";
    if( onRaspberryPi() == true ) {
        _bluePipeDrawer->drawNumber( " ", BLUE_BAR_HORIZONTAL_OFFSET, BLUE_BAR_VERTICAL_OFFSET ); // draw pipe
    }
}

void ScoreBoard::drawYellowPeriod() {
    if ( !onRaspberryPi()) {
        print( "inside drawYellowPeriod()" );
        return; 
    }
    int period_lr_offset = 53;
    int period_ud_offset = 129;
    _yellowPeriodDrawer->drawNumber( ".", period_lr_offset, period_ud_offset - 20 );
}

void ScoreBoard::drawGreenPeriod() {
    if ( !onRaspberryPi()) /* return if not on Pi */ { return; }
#define PERIOD_LR_OFFSET 51
#define PERIOD_UD_OFFSET 92
    _greenPeriodDrawer->drawNumber( ".", PERIOD_LR_OFFSET, PERIOD_UD_OFFSET - 20 );
}

void ScoreBoard::drawRedPeriod() {
    if ( !onRaspberryPi()) /* return if not on Pi */ { return; }
#define PERIOD_LR_OFFSET 51
#define PERIOD_UD_OFFSET 92
    _redPeriodDrawer->drawNumber( ".", PERIOD_LR_OFFSET, PERIOD_UD_OFFSET - 20 );
}

void ScoreBoard::drawBlankPeriod() {
    if ( !onRaspberryPi()) /* return if not on Pi */ { return; }
#define PERIOD_LR_OFFSET 51
#define PERIOD_UD_OFFSET 92
    _blankPeriodDrawer->drawNumber( ".", PERIOD_LR_OFFSET, PERIOD_UD_OFFSET - 20 );
}

void ScoreBoard::blink_player_score( int player ) {  // fast blinking ball code here
    if ( !onRaspberryPi()) /* return if not on Pi */ { return; }
#define BLINK_DELAY 100
#define BLINK_COUNT 3
#define PERIOD_LR_OFFSET 51
#define PERIOD_UD_OFFSET 92
    if ( player == PLAYER_1_SCORE ) {           // blink player 1
        for ( int i = 0; i < BLINK_COUNT; i++ ) {
            _greenPeriodDrawer->drawNumber( ".", PERIOD_LR_OFFSET, PERIOD_UD_OFFSET - 20 );
            GameTimer::gameDelay( BLINK_DELAY );
            _blankPeriodDrawer->drawNumber( ".", PERIOD_LR_OFFSET, PERIOD_UD_OFFSET - 20 );
            GameTimer::gameDelay( BLINK_DELAY );
        }
    }
    else {                                    // blink player 2
        for ( int i = 0; i < BLINK_COUNT; i++ ) {
            _redPeriodDrawer->drawNumber( ".", PERIOD_LR_OFFSET, PERIOD_UD_OFFSET - 20 );
            GameTimer::gameDelay( BLINK_DELAY );
            _blankPeriodDrawer->drawNumber( ".", PERIOD_LR_OFFSET, PERIOD_UD_OFFSET - 20 );
            GameTimer::gameDelay( BLINK_DELAY );
        }
    }
}

void ScoreBoard::clearScreen() {
    if ( hasCanvas() == false ) {
        std::cout << "\033[2J\033[H";   // Clear screen and move cursor to the top-left 
        // system( "clear" );           // execute a system `clear` command  
    }
    else {
        if ( !hasCanvas()) { std::cout << "*** ERROR: canvas == NULL.  exiting... ***" << std::endl; exit( 1 ); }
        Color flood_color( 0, 0, 0 ); _canvas->Fill( flood_color.r, flood_color.g, flood_color.b );
    }
}

std::string ScoreBoard::drawPlayerScore( Player* player ) {
    std::string serve_bar_text = hasCanvas() == true ? "I" : "\033[34m|";    // filled in "I"
    // std::string serve_bar_text = hasCanvas() == true ? "i" : "\033[34m|"; // hollow "i"
    std::string serve_bar = _gameState->getServe() == player->number() ? serve_bar_text : " ";
    std::string other_serve_bar = _gameState->getServe() == player->getOpponent()->number() ? serve_bar_text : " ";
    std::string score = _translate( player->getPoints());
    if ( hasCanvas() == false ) {
        std::cout << "==========================" << std::endl;
        player->number() == PLAYER_1_INITIALIZED ?  // type player 1 score, else type player 2 score
            std::cout << "| \033[92mPLAYER 1: //// " << serve_bar << "\033[92m " << score << " //// " << std::endl :
            std::cout << "| \033[31mPLAYER 2: //// " << serve_bar << "\033[31m " << score << " //// \033[35m" << std::endl;
    } else {
        if ( _gameState->getTieBreak() || _gameState->getMatchTieBreak()) { _drawTieBreakScore( player ); return ""; } // 102324
        #define BIG_NUMBER_VERTICAL_OFFSET 2
        int vertical_offset = player->number() == 0 ? BIG_NUMBER_VERTICAL_OFFSET : 
            _big_number_font.height() + BIG_NUMBER_VERTICAL_OFFSET;
        _pipeDrawer->drawNumber( serve_bar, 2, _big_number_font.baseline() + vertical_offset );
        int baseline = _big_number_font.baseline();                  // set the coordinates for the text
        int first_offset = _firstCharacterOffset( score.substr( 0, 1 ));
        int second_offset = ( score.length() > 1 ) ? _characterOffset( score.substr( 1, 1 )) : 0;
        if ( player->number() == PLAYER_1_INITIALIZED ) { // then draw text depending on player
            _playerOneScoreDrawer->drawNumber( score.substr( 0, 1 ), first_offset + 16, baseline + vertical_offset );
            if ( score.length() > 1 ) {
                _playerOneScoreDrawer->drawNumber( score.substr( 1, 1 ), second_offset + 38, baseline + vertical_offset );
            }
        } else {
            _playerTwoScoreDrawer->drawNumber( score.substr( 0, 1 ), first_offset + 16, baseline + vertical_offset );
            if ( score.length() > 1 ) {
                _playerTwoScoreDrawer->drawNumber( score.substr( 1, 1 ), second_offset + 38, baseline + vertical_offset );
            }
        } // return player 1 score, else type player 2 score
    }
    // created a concatenated string with "PLAYER 1: ////// " + serve_bar
    std::string returnString = "*** WARNING: return string is not set. this is not normal ***";
    std::string player1ScoreString = "PLAYER 1: ////// " + serve_bar + " " + score + " //////";
    std::string player2ScoreString = "PLAYER 2: ////// " + serve_bar + " " + score + " //////";
    player->number() == PLAYER_1_INITIALIZED ?
        returnString = player1ScoreString : returnString = player2ScoreString;
    return returnString;
}

void ScoreBoard::_drawTieBreakScore( Player* player ) {
    // std::string serve_bar = _gameState->getServe() == player->number() ? serve_bar_text : " ";
    // std::string other_serve_bar = _gameState->getServe() == player->getOpponent()->number() ? serve_bar_text : " ";
    std::string serve_bar_text = hasCanvas() == true ? "I" : "\033[34m|";    // filled in "I"
    std::string serve_bar;
    // if ( _gameState->getServe() == player->getOpponent()->number()) {
    if ( _gameState->getServe() == player->number()) {
        serve_bar = serve_bar_text;
    } else {
        serve_bar = " ";
    }

    std::string score = _translate( player->getPoints());
    // print( "score[" + score + "]" );
    #define BIG_NUMBER_VERTICAL_OFFSET 2
    #define TB_X_OFFSET 26  // Tie Breaker x-axis offset

    int tb_x_offset = 0;
    if ( score == "1" ) {
        // print( "adding 4 to tie breaker x offset" );
        tb_x_offset = TB_X_OFFSET + 1; // from +2 on october 25
        // print( "score is 1.  after adding the offset, tb_x_offset is "  + std::to_string( tb_x_offset ));
    } else if ( score == "3" ) {
        // print( "subtracting 1 from the tie breaker x offset" );
        tb_x_offset = TB_X_OFFSET - 1; // from +2 on october 25
        // print( "score is 3.  subtracting from the offset, tb_x_offset is "  + std::to_string( tb_x_offset ));
    } else if ( score == "4" ) {
        // print( "score is 4, keeping the tie breaker offse at " << TB_X_OFFSET );
        tb_x_offset = TB_X_OFFSET - 1;
     } else if ( score == "6" ) {
        // print( "subtracting 1 from the tie breaker x offset" );
        tb_x_offset = TB_X_OFFSET - 1; // from -2 on Nov 1 // from +2 on october 25
        // print( "score is 6.  subtracting from the offset, tb_x_offset is "  + std::to_string( tb_x_offset ));
    } else if ( score == "9" ) {
        tb_x_offset = TB_X_OFFSET;
        // print( "score is 9.  subtracting from the offset, tb_x_offset is "  + std::to_string( tb_x_offset ));
    } else if ( score == "10" ) {
        // print( "score is 10, subtracting 1 from the tie breaker x offset" );
        tb_x_offset = TB_X_OFFSET - 1; // from +2 on october 25
        // print( "score is 10.  subtracting from the offset, tb_x_offset is "  + std::to_string( tb_x_offset ));
    } else {
        // print( "score is not anything in the list, tb_x_offset is " + std::to_string( tb_x_offset ) );
       tb_x_offset =TB_X_OFFSET;
    }

    int vertical_offset = player->number() == 0 ? BIG_NUMBER_VERTICAL_OFFSET : 
        _big_number_font.height() + BIG_NUMBER_VERTICAL_OFFSET;
    _pipeDrawer->drawNumber( serve_bar, 2, _big_number_font.baseline() + vertical_offset );
    int baseline = _big_number_font.baseline();  // set the coordinates for the text
    
    // introduce first offset for tie-break 2-digit scores
    int
    first_offset = _firstCharacterOffset( score.substr( 0, 1 ));

    // introduce second offset for tie-break 2-digit scores
    int 
    second_offset  = ( score.length() > 1 ) ? _characterOffset( score.substr( 1, 1 )) : 0;
    second_offset  = score == "10" ? second_offset - 1 : second_offset;
    second_offset  = score == "11" ? second_offset + 1 : second_offset;
    // if ( score == "12" ) {
    //     first_offset = first_offset
    // }
    second_offset  = score == "12" ? second_offset - 1 : second_offset; // move "2" one space left on october 25

    if ( player->number() == PLAYER_1_INITIALIZED ) {  // then draw text depending on player
        if ( score.length() > 1 ) {
            _playerOneScoreDrawer->drawNumber( score.substr( 0, 1 ), first_offset  + 16, baseline + vertical_offset );
            _playerOneScoreDrawer->drawNumber( score.substr( 1, 1 ), second_offset + 38, baseline + vertical_offset );
        } else {
            _playerOneScoreDrawer->drawNumber( score.substr( 0, 1 ), tb_x_offset,        baseline + vertical_offset );
            // print( "score is: " + score + " tb_x_offset is " + std::to_string( tb_x_offset ));
        }
    } else {
        if ( score.length() > 1 ) {
            _playerTwoScoreDrawer->drawNumber( score.substr( 0, 1 ), first_offset  + 16, baseline + vertical_offset  );
            _playerTwoScoreDrawer->drawNumber( score.substr( 1, 1 ), second_offset + 38, baseline + vertical_offset  );
        } else {
            _playerTwoScoreDrawer->drawNumber( score.substr( 0, 1 ), tb_x_offset,        baseline + vertical_offset  );
            // print( "score is: " + score + " tb_x_offset is " + std::to_string( tb_x_offset ));
        }
    } // return player 1 score, else type player 2 score
}

int ScoreBoard::_characterOffset( std::string character ) {
    int char_offset = 0;
    if ( character == "A" ) {
        return -3;
    } else if ( character == "d" ) {
        return 2;
    } else if ( character == "4" ) {
        return 1;
    } else { 
        char_offset = std::stoi( character );
    }

    switch ( char_offset ) {
        case 3: return -1;
        case 4: return -4;
        case 5: return -1;
        default: return 0;
    }
}

int ScoreBoard::_firstCharacterOffset( std::string character ) {
    int char_offset = 0;
    #define FIRST_ZERO_OFFSET -2
    if ( character == "A" ) {
        return -3;
    } else if ( character == "d" ) {
        return 2;
    } else if ( character == "0" ) {
        return FIRST_ZERO_OFFSET;
    } else if ( character == "3" ) {
        return -3;
    } else if ( character == "1" ) {
        return 1;
    } else { char_offset = std::stoi( character ); }

    switch ( char_offset ) {
        case 3: return -1;
        case 4: return -4;
        case 5: return -1;
        default: return 0;
    }
}

std::string ScoreBoard::_translate( int raw_score ) {
    if ( _gameState->getTieBreak() == false && _gameState->getMatchTieBreak() == false ) {
        switch ( raw_score ) {
        case 0:               return "00";
        case 1:               return "15";
        case 2:               return "30";
        case 3:               return "40";

        case SCORE_CASE_4:
            if ( _gameState->getPointFlash()) {
                return "Ad";
            }
            else {
                return "40";
            }

        case SCORE_CASE_5:    return "Ad";
        case UNDEFINED_SCORE: return "99";
        default:              return "00";
        }
    }
    else {
        switch ( raw_score ) {
        case 0:               return "0";
        case 1:               return "1";
        case 2:               return "2";
        case 3:               return "3";
        case 4:               return "4";
        case 5:               return "5";
        case 6:               return "6";
        case 7:               return "7";
        case 8:               return "8";
        case 9:               return "9";
        case 10:              return "10";
        case 11:              return "11";
        case 12:              return "12";
        case 13:              return "13";
        case 14:              return "14";
        case 15:              return "15";
        }
    }
    return "*** ERROR: can not translate. ***";
}

bool ScoreBoard::onRaspberryPi() {
    std::ifstream file( "/proc/device-tree/model" );
    std::string line;
    if ( file.is_open()) {
        std::getline( file, line );
        file.close();
        if ( line.find( "Raspberry Pi" ) != std::string::npos ) { return true; }
    }
    return false;
}

void ScoreBoard::setDrawerBackgroundColor( const Color& color ) { _drawer->setBackgroundColor( color ); }
void ScoreBoard::setDrawerForegroundColor( const Color& color ) { _text_drawer->setForegroundColor( color ); }
void ScoreBoard::setDrawerFont( const rgb_matrix::Font* font ) { _drawer->setFont( font ); }
void ScoreBoard::setDrawerSize( Drawer::Size size ) { _drawer->setSize( size ); }

void ScoreBoard::setLittleDrawerFont( const std::string& font_file ) {
    Color color( 255, 255, 0 );
    Color bg_color( 0, 0, 0 );
    // print( "loading little number font: " + font_file );
    FontLoader fontLoader( font_file.c_str());
    fontLoader.LoadFont( _little_number_font );
    _text_drawer = new Drawer( _canvas.get(), &_little_number_font, Drawer::BIG, color, bg_color );
    // print( "little number font loaded" );
    // print( "set little number font disabled on october massacre. " );
}

std::string ScoreBoard::displayAndLoadFontMenu( const std::string& fontDirectory ) {
    // Step 1: Get the list of font files in the directory
    std::vector<std::string> fonts;
    for ( const auto& entry : std::filesystem::directory_iterator( fontDirectory )) {
        if ( entry.is_regular_file()) {
            std::string fileName = entry.path().filename().string();
            if ( fileName.find( ".bdf" ) != std::string::npos ) {
                fonts.push_back( fileName );
            }
        }
    }

    // Step 2: Display the font options
    if ( fonts.empty()) {
        std::cout << "No fonts available in the directory." << std::endl;
        return "*** ERROR: no fonts available in the directory. ***";
    }

    std::cout << "Available fonts:" << std::endl;
    for ( size_t i = 0; i < fonts.size(); ++i ) {
        std::cout << i + 1 << ". " << fonts[i] << std::endl;
    }

    // Step 3: Get user input
    int fontChoice = 0;
    while ( true ) {
        std::cout << "Enter the number of the font to load: ";
        std::cin >> fontChoice;

        if ( fontChoice > 0 && fontChoice <= static_cast< int >( fonts.size()) ) {
            break;
        }
        else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    // Step 4: Load the selected font
    std::string selectedFont = fonts[fontChoice - 1];
    std::string fontFilePath = fontDirectory + "/" + selectedFont;
    print( "using font loader to load path: " + fontFilePath );
    return fontFilePath;
}
