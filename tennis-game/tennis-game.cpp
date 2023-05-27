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
#include "CanvasCreator/CanvasCreator.h"
#include "FontLoader/FontLoader.h"
#include "TextDrawer/TextDrawer.h"
#include "NumberDrawer/NumberDrawer.h"

using namespace rgb_matrix;

static bool parseColor(Color *c, const char *str) { return sscanf(str, "%hhu,%hhu,%hhu", &c->r, &c->g, &c->b) == 3; }

void showLittleNumbers() {
    #define LITTLE_NUMBER_FONT "fonts/little_numbers.bdf"
    #define SPACE_BEFORE_SMALL_NUMBER 7
    #define SPACE_BETWEEN_SMALL_NUMBERS 17
    Color background_color(0, 0, 0);
    int letter_spacing = 0;
    rgb_matrix::Font little_number_font;
    if (!little_number_font.LoadFont( LITTLE_NUMBER_FONT )) {
        fprintf( stderr, "Couldn't load font '%s'\n", LITTLE_NUMBER_FONT );
        return 1; }

    int x = 0;
    int y = bigNumberFont.baseline() + bigNumberFont.height();
    rgb_matrix::Font *outline_font = NULL;

    Color thirdRowColor( 0, 255, 0 );
    rgb_matrix::DrawText( canvas, little_number_font, x + SPACE_BEFORE_SMALL_NUMBER, y + little_number_font.baseline(), thirdRowColor,  outline_font ? NULL : &background_color, "1 2 3", letter_spacing );

    y += little_number_font.height() - 5;
    printf("Font height before fourth row: %d\n", little_number_font.height());

    Color fourthRowColor( 255, 0, 0 );
    rgb_matrix::DrawText( canvas, little_number_font, x + SPACE_BEFORE_SMALL_NUMBER, y + little_number_font.baseline(), fourthRowColor, outline_font ? NULL : &background_color, "4", letter_spacing );
    rgb_matrix::DrawText( canvas, little_number_font, x + SPACE_BEFORE_SMALL_NUMBER + SPACE_BETWEEN_SMALL_NUMBERS, y + little_number_font.baseline(), fourthRowColor, outline_font ? NULL : &background_color, "5", letter_spacing );
    rgb_matrix::DrawText( canvas, little_number_font, x + SPACE_BEFORE_SMALL_NUMBER + (( 2 * SPACE_BETWEEN_SMALL_NUMBERS )), y + little_number_font.baseline(), fourthRowColor, outline_font ? NULL : &background_color, "6", letter_spacing );
}

int main(int argc, char *argv[]) {
    RGBMatrix::Options matrix_options;
    rgb_matrix::RuntimeOptions runtime_opt;
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
    printf("Updating ScoreBoard...\n");

    // Create Canvas
    CanvasCreator canvasCreator(matrix_options, runtime_opt);
    RGBMatrix* canvas = canvasCreator.CreateCanvas();

    FontLoader fontLoader("fonts/mspgothic_042623.bdf"); // Load Fonts
    rgb_matrix::Font font;
    fontLoader.LoadFont(font);

    FontLoader bigNumberFontLoader("fonts/fgm_27_ee.bdf");
    rgb_matrix::Font bigNumberFont;
    bigNumberFontLoader.LoadFont(bigNumberFont);

    // FontLoader littleNumberFontLoader("fonts/little_numbers.bdf");
    // rgb_matrix::Font littleNumberFont;
    // littleNumberFontLoader.LoadFont(littleNumberFont);
    // create new big number drawer
    NumberDrawer bigNumberDrawer( canvas, &bigNumberFont, NumberDrawer::BIG, big_number_color, background_color );
    NumberDrawer pipeDrawer(      canvas, &bigNumberFont, NumberDrawer::BIG, pipe_color,       background_color );

    // bigNumberDrawer.DrawNumber("8", 16, bigNumberFont.baseline() - 1);

    // NumberDrawer smallNumberDrawer(canvas, &littleNumberFont, NumberDrawer::SMALL, color, background_color);
    // smallNumberDrawer.DrawNumber("1 2 3", 7, littleNumberFont.baseline());

    ///////// Start Game Loop /////////////
    bool game_running = true;
    int loop_count = 0;
    #define MAX_LOOP_COUNT 60
    #define SCORE_DELAY    2
    #define A_SPACE        14
    #define FOUR_SPACE     14
    #define THREE_SPACE    15

    while ( game_running ) {
        if ( loop_count >  MAX_LOOP_COUNT ) { game_running = false; }
        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());

        showLittleNumbers();

        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b); // clear screen



        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        bigNumberDrawer.DrawNumber( " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b); // clear screen



        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill( flood_color.r, flood_color.g, flood_color.b ); // clear screen



        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "2", 16, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "1", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "5", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen



        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "2", 16, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "3", THREE_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "4", FOUR_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "0", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      " ", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      "I", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen


        pipeDrawer.DrawNumber(      "I", 1, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline());
        bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline());

        pipeDrawer.DrawNumber(      " ", 1,  bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "A", A_SPACE, bigNumberFont.baseline() + bigNumberFont.height());
        bigNumberDrawer.DrawNumber( "d", 38, bigNumberFont.baseline() + bigNumberFont.height());
        sleep( SCORE_DELAY );
        canvas->Fill(flood_color.r, flood_color.g, flood_color.b ); // clear screen

        printf( "Loop count: %d\n", loop_count++ ); }
    ///////// End Game Loop /////////////
    delete canvas;
    return 0;
}
