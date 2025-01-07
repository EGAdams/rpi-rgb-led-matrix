#ifndef TennisConstants_h
#define TennisConstants_h

#include "../Arduino/Arduino.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

#define print( x ) \
    do { \
        auto now = std::chrono::system_clock::now(); \
        auto in_time_t = std::chrono::system_clock::to_time_t( now ); \
        std::tm buf; \
        localtime_r( &in_time_t, &buf ); \
        std::cout << "[" << std::put_time( &buf, "%H:%M:%S" ) << "] " \
                  << "[" << __FILE__ << "] " \
                  << "[: " << __LINE__ << "] " \
                  << "[" << __FUNCTION__ << "()] " \
                  << x << std::endl; \
    } while ( 0 )


//  Written by Chris Bean for Teensy 4.1 only
//  re-written in C++ by EG in June 2022


/////////////////////// REMOTE DEFINITIONS /////////////////////////////////////
# define REMOTE_INPUT 0 // Change from Remote in put or keyboard input for debug

#define REMOTE_DATA_0 25
#define REMOTE_DATA_1 24
#define REMOTE_DATA_2 40
#define REMOTE_DATA_3 17
#define RED_REMOTE_RED_SCORE     6
#define RED_REMOTE_GREEN_SCORE   10
#define RED_REMOTE_UNDO          2
#define RED_REMOTE_RESET         99
#define GREEN_REMOTE_GREEN_SCORE 7
#define GREEN_REMOTE_RED_SCORE   11
#define GREEN_REMOTE_UNDO        3
#define GREEN_REMOTE_RESET       99
#define NO_REMOTE_BUTTONS        0
#define UNKNOWN_REMOTE_BUTTON   99

#define STEVE_DELAY            25
#define REMOTE_READ_DELAY      25
///////////////////////////////////////////////////////////////////////////////

#define RUNNING_MATCH_TIE_BREAK     "running match tie breaker"
#define BOTH_PLAYER_BLINK           "blink both players"
#define PLAYER_1_BLINK              "player1 blink"
#define PLAYER_2_BLINK              "player2 blink"
#define PLAYER_ONE_BUTTON           1
#define PLAYER_TWO_BUTTON           2
#define PLAYER_ONE_SERVE            0
#define PLAYER_TWO_SERVE            1

///////////////////// GAME STATES /////////////////////////////////////////////
#define ACTIVE_BLINK                "blink"
#define NORMAL_GAME_STATE           "normal game state"
#define RUNNING_MATCH_WIN_SEQUENCE  "running match win sequence"
#define DRAW_BLANK_SETS             "draw blank sets"
#define SLEEP_MODE                  "sleep mode for power saving"
#define AFTER_UPDATE_GO_SCORE       "after update player score in GameObject"
#define IGNORE_EXTRA_SCORES         "ignoring scores after match win"
#define AFTER_SLEEP_MODE            "after sleep mode"
///////////////////////////////////////////////////////////////////////////////

#define TESTING                     1
#define GAME_FLASH_DELAY            500 // 500 // 200 // 50 // 200 // 010725
#define SET_WIN_FLASH_DELAY         500 // 750 // 1000 // 600 // 5 // 200 010725
#define FLASH_DELAY                 250 // 5 // 250 // 1000
#define BUTTON_DELAY                50 // 5 // 50 // 300
#define WIN_DELAY                   50 // 5 // 50 // 1000
#define WATCH_INTERVAL              100 // 5 // 250
#define UPDATE_DISPLAY_DELAY        50 // 5 // 50
#define ALL_SETS_FLASH_DELAY        500 // 750 // 1000 // 500 110924 010725
#define LOOP_MATCH_LAMP_WIN         2
#define MATCH_WIN_BLINK_COUNT       10 // 5 // added on clermont hill
#define SHOW_MATCH_WIN_TEXT_DELAY   3000 // 45000 // added anna marie island monday
#define MAX_SLEEP                   60 // 15 // in SECONDS // last day of august tarpon
#define MIN_SLEEP                   3000 // sleep for a few seconds in case of buttons pressed.
#define NUMBER_OF_GAME_LAMPS        7
#define LOOP_GAME_LAMP_WIN          5
#define TIE_PULSE_COUNT             5
#define SET_WIN_PULSE_COUNT         5
#define ALL_SETS_FLASH_COUNT        5
#define TIE_BREAK_WIN_BY_TWO        10
#define TIE_BREAK_MAX_POINTS        13
#define TIE_BREAK_BLINK_DELAY       250 // 750
#define TIE_BREAK_WATCH_INTERVAL    50
#define PAUSE_BEFORE_TIE_BREAK_WIN  1000
#define BATTERY_TEST_BLINK_DELAY    1000
#define GAMES_TO_WIN_SET            6
#define SET_WIN_DELAY               3000 // 500 // 3000
#define PLAYER_1_INITIALIZED        0
#define PLAYER_2_INITIALIZED        1
#define SET_HISTORY_COLUMN_1        1
#define SET_HISTORY_COLUMN_2        2
#define SET_HISTORY_COLUMN_3        3
#define TOTAL_SETS                  3
#define PLAYER_ONE_SET_INDEX        0
#define PLAYER_TWO_SET_INDEX        1
#define SETS_TO_WIN_MATCH           2
#define SCORE_CASE_4                4
#define SCORE_CASE_5                5
#define UNDEFINED_SCORE             99
#define BLUE_BAR_VERTICAL_OFFSET    124 // 126 // 132 // 124
#define BLUE_BAR_HORIZONTAL_OFFSET  56 // 54
#define RED_BAR_HORIZONTAL_OFFSET   44
#define GREEN_BAR_HORIZONTAL_OFFSET 30


// #define INPUT 0 // not in windows.. ouch!!
// #define OUTPUT 0
#define LOW 0
#define HIGH 1
#define BLINK 2

#define PLAYER_TOP 30
#define PRESS_TOP 47
#define YOUR_TOP 60
#define REMOTE_TOP 73
#define MIDDLE_OFFSET 2
#define GREEN_TOP 94
#define RED_TOP 94
#define BUTTON_TOP 107

// pairing screen vertical offsets for centering
#define GREEN_OFFSET 4
#define RED_OFFSET 12
#define YOUR_OFFSET 7
#define PRESS_OFFSET 4
#define REMOTE_OFFSET 0
#define BUTTON_OFFSET 1


///////////////////////////////////////  START WINDOWS SIMULATOR //////////////////////////////////
#define SIMULATION 1
#define PLAYER_BUTTONS 202
#define ROTARY 150
#define LOG_FILE_PATH          "C:\\Users\\EG\\Desktop\\2022\\june\\4th_week\\test.txt"
#define PIN_FILE_PATH          "C:\\Users\\EG\\Desktop\\2022\\june\\2nd_week\\tennis_cpp\\pin_data\\"
#define GAME_STATE_UPDATE_PATH "C:\\Users\\EG\\Desktop\\2022\\june\\2nd_week\\tennis_cpp\\game_state_data\\"
#define MATCH_WIN_FLASH_DELAY  800 // 1000 // 250 // 100
#define BATTERY_TEST_DELAY      500
#define BATTERY_TEST_FLASH_COUNT 5
#include <iostream>
//////////////////////////////////// END WINDOWS SIMULATION /////////////////////////////////////


#define UNDO  39
#define RESET 38

#define P1_POINTS_LED1 22
#define P1_POINTS_LED2 23
#define P1_POINTS_LED3 0
#define P1_POINTS_LED4 1
#define P1_SERVE 19
#define P1_GAMES_LED0 31
#define P1_GAMES_LED1 18
#define P1_GAMES_LED2 15
#define P1_GAMES_LED3 14
#define P1_GAMES_LED4 41
#define P1_GAMES_LED5 37
#define P1_GAMES_LED6 36
#define P1_TIEBREAKER 33
#define P1_SETS_LED1 29
#define P1_SETS_LED2 30
#define P2_POINTS_LED1 21
#define P2_POINTS_LED2 20
#define P2_POINTS_LED3 3
#define P2_POINTS_LED4 2
#define P2_SERVE 4
#define P2_GAMES_LED0 32
#define P2_GAMES_LED1 5
#define P2_GAMES_LED2 6
#define P2_GAMES_LED3 7
#define P2_GAMES_LED4 8
#define P2_GAMES_LED5 9
#define P2_GAMES_LED6 10
#define P2_TIEBREAKER 11
#define P2_SETS_LED1 28
#define P2_SETS_LED2 12
#endif
