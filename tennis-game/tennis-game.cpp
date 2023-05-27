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

int main(int argc, char *argv[]) {
    RGBMatrix::Options matrix_options;
    rgb_matrix::RuntimeOptions runtime_opt;
    if (!rgb_matrix::ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_opt)) {
        return 0;
    } else {
        printf( "Matrix options parsed.\n" )
    }
    Color color(255, 255, 0);
    Color bg_color(0, 0, 0);
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
    NumberDrawer bigNumberDrawer(canvas, &bigNumberFont, NumberDrawer::BIG, color, bg_color);

    bigNumberDrawer.DrawNumber("8", 16, bigNumberFont.baseline() - 1);

    // NumberDrawer smallNumberDrawer(canvas, &littleNumberFont, NumberDrawer::SMALL, color, bg_color);
    // smallNumberDrawer.DrawNumber("1 2 3", 7, littleNumberFont.baseline());

    ///////// Start Game Loop /////////////

    bool game_running = true;
    int loop_count = 0;
    #define MAX_LOOP_COUNT 6
    while ( game_running ) {
        if ( loop_count >  MAX_LOOP_COUNT ) { game_running = false; }
        printf( "sleeping... " );
        sleep( 1 );
        printf( "Loop count: %d\n", loop_count++ ); }
    ///////// End Game Loop /////////////

    return 0;
}
