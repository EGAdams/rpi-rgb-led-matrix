// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// The main tennis game entry point.
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)

#include "../include/led-matrix.h"
#include "../include/graphics.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "FontLoader/FontLoader.h"
#include "TextDrawer/TextDrawer.h"
#include "NumberDrawer/NumberDrawer.h"
#include "ScoreBoard/ScoreBoard.h"
#include "GameObject/GameObject.h"


using namespace rgb_matrix;

static bool parseColor(Color *c, const char *str) { return sscanf(str, "%hhu,%hhu,%hhu", &c->r, &c->g, &c->b) == 3; }

int main(int argc, char *argv[]) {
    RGBMatrix::Options matrix_options;
    printf( "Matrix options:\n" );
    printf( "  rows: %d\n", matrix_options.rows );
    printf( "  chain_length: %d\n", matrix_options.chain_length );
    printf( "  parallel: %d\n", matrix_options.parallel );
    printf( "  pwm_bits: %d\n", matrix_options.pwm_bits );
    printf( "  pwm_lsb_nanoseconds: %d\n", matrix_options.pwm_lsb_nanoseconds );

    rgb_matrix::RuntimeOptions runtime_opt;
    printf( "Runtime options:\n" );
    printf( "  daemon: %d\n", runtime_opt.daemon );
    printf( "  do_gpio_init: %d\n", runtime_opt.do_gpio_init );
    printf( "  drop_privileges: %d\n", runtime_opt.drop_privileges );
    printf( "  gpio_slowdown: %d\n", runtime_opt.gpio_slowdown );
    printf( "  hardware_mapping: %s\n", runtime_opt.hardware_mapping );
    printf( "  led_rgb_sequence: %s\n", runtime_opt.led_rgb_sequence );
    printf( "  limit_refresh_rate_hz: %d\n", runtime_opt.limit_refresh_rate_hz );
    printf( "  show_refresh_rate: %d\n", runtime_opt.show_refresh_rate );
    printf( "  inverse_colors: %d\n", runtime_opt.inverse_colors );
    printf( "  led_rgb_sequence: %s\n", runtime_opt.led_rgb_sequence );
    printf( "  pwm_bits: %d\n", runtime_opt.pwm_bits );
    printf( "  pwm_dither_bits: %d\n", runtime_opt.pwm_dither_bits );
    printf( "  pwm_lsb_nanoseconds: %d\n", runtime_opt.pwm_lsb_nanoseconds );
    printf( "  pwm_slowdown_gpio: %d\n", runtime_opt.pwm_slowdown_gpio );
    printf( "  rgb_sequence: %s\n", runtime_opt.rgb_sequence );
    printf( "  row_address_type: %d\n", runtime_opt.row_address_type );
    printf( "  scan_mode: %d\n", runtime_opt.scan_mode );
    printf( "  show_refresh_rate: %d\n", runtime_opt.show_refresh_rate );
    printf( "  swap_green_blue: %d\n", runtime_opt.swap_green_blue );
    printf( "  multiplexing: %d\n", runtime_opt.multiplexing );
    printf( "  panel_type: %d\n", runtime_opt.panel_type );

    if (!rgb_matrix::ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_opt)) {
        return 0;
    } else {
        printf( "Matrix options parsed.\n" );
    }
    Color pipe_color(255, 255, 0); // yellow
    Color background_color(0, 0, 0);
    Color big_number_color(0, 255, 0); // green
    Color flood_color(0, 0, 0);
    Color outline_color(0,0,0);
    bool with_outline = false;

    const char *bdf_font_file = NULL;
    int x_orig = 0;
    int y_orig = 0;
    int letter_spacing = 0;
    int opt;

    // FontLoader littleNumberFontLoader("fonts/little_numbers.bdf");
    // rgb_matrix::Font littleNumberFont;
    // littleNumberFontLoader.LoadFont(littleNumberFont);
    // create new big number drawer


    // bigNumberDrawer.DrawNumber("8", 16, bigNumberFont.baseline() - 1);

    // NumberDrawer smallNumberDrawer(canvas, &littleNumberFont, NumberDrawer::SMALL, color, background_color);
    // smallNumberDrawer.DrawNumber("1 2 3", 7, littleNumberFont.baseline());

    ///////// Start Game Loop /////////////
    bool game_running = true;
    int loop_count = 0;
    #define MAX_LOOP_COUNT 60
    #define SCORE_DELAY    2
    #define A_SPACE        13
    #define FOUR_SPACE     14
    #define THREE_SPACE    15

    GameObject gameObject;


    while ( game_running ) {
        if ( loop_count >  MAX_LOOP_COUNT ) { game_running = false; }
        gameObject.setPlayer1Score( "0" );
        gameObject.setPlayer2Score( "0" );
        _player1->setScore( "0" );
        _player2->setScore( "0" );
        _scoreboard->update();

        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b); // clear screen

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 16, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b); // clear screen


        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b); // clear screen


        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        bigNumberDrawer.DrawNumber( " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b); // clear screen



        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill( flood_color.r, flood_color.g, flood_color.b ); // clear screen



        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "2", 16, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "2", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers( canvas );
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen

        loop_count++; }
    ///////// End Game Loop /////////////
    delete canvas;
    return 0;
}
