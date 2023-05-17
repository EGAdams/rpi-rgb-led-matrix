// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// The main tennis game entry point.
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)

#include "led-matrix.h"
#include "graphics.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "CanvasCreator/CanvasCreator.h"
#include "FontLoader/FontLoader.h"
#include "TextDrawer/TextDrawer.h"

using namespace rgb_matrix;

static int usage(const char *progname) {
    fprintf(stderr, "usage: %s [options]\n", progname);
    fprintf(stderr, "Reads text from stdin and displays it. "
            "Empty string: clear screen\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr,
            "\t-f <font-file>    : Use given font.\n"
            "\t-x <x-origin>     : X-Origin of displaying text (Default: 0)\n"
            "\t-y <y-origin>     : Y-Origin of displaying text (Default: 0)\n"
            "\t-S <spacing>      : Spacing pixels between letters (Default: 0)\n"
            "\t-C <r,g,b>        : Color. Default 255,255,0\n"
            "\t-B <r,g,b>        : Font Background-Color. Default 0,0,0\n"
            "\t-O <r,g,b>        : Outline-Color, e.g. to increase contrast.\n"
            "\t-F <r,g,b>        : Panel flooding-background color. Default 0,0,0\n"
            "\n"
            );
    rgb_matrix::PrintMatrixFlags(stderr);
    return 1;}

    static bool parseColor(Color *c, const char *str) {
    return sscanf(str, "%hhu,%hhu,%hhu", &c->r, &c->g, &c->b) == 3;}

    static bool FullSaturation(const Color &c) {
    return (c.r == 0 || c.r == 255)
        && (c.g == 0 || c.g == 255)
        && (c.b == 0 || c.b == 255); }

    int main(int argc, char *argv[]) {
    RGBMatrix::Options matrix_options;
    rgb_matrix::RuntimeOptions runtime_opt;
    if (!rgb_matrix::ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_opt)) { return usage(argv[0]); }
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
    while ((opt = getopt(argc, argv, "x:y:f:C:B:O:S:F:")) != -1) {
        switch (opt) {
        case 'x': x_orig = atoi(optarg); break;
        case 'y': y_orig = atoi(optarg); break;
        case 'f': bdf_font_file = strdup(optarg); break;
        case 'S': letter_spacing = atoi(optarg); break;
        case 'C':
        if (!parseColor(&color, optarg)) {
            fprintf(stderr, "Invalid color spec: %s\n", optarg);
            return usage(argv[0]);
        }
        break;
        case 'B':
        if (!parseColor(&bg_color, optarg)) {
            fprintf(stderr, "Invalid background color spec: %s\n", optarg);
            return usage(argv[0]);
        }
        break;
        case 'O':
        if (!parseColor(&outline_color, optarg)) {
            fprintf(stderr, "Invalid outline color spec: %s\n", optarg);
            return usage(argv[0]);
        }
        with_outline = true;
        break;
        case 'F':
        if (!parseColor(&flood_color, optarg)) {
            fprintf(stderr, "Invalid background color spec: %s\n", optarg);
            return usage(argv[0]); }
        break;
        default:
        return usage(argv[0]); }}

    printf("Updating ScoreBoard...\n");

    // RGBMatrix::Options matrix_options;
    // rgb_matrix::RuntimeOptions runtime_opt;

    // Create Canvas
    CanvasCreator canvasCreator(matrix_options, runtime_opt);
    RGBMatrix* canvas = canvasCreator.CreateCanvas();

    FontLoader fontLoader("fonts/mspgothic_042623.bdf"); // Load Fonts
    rgb_matrix::Font font;
    fontLoader.LoadFont(font);

    FontLoader bigNumberFontLoader("fonts/fgm_27_ee.bdf");
    rgb_matrix::Font bigNumberFont;
    bigNumberFontLoader.LoadFont(bigNumberFont);

    FontLoader littleNumberFontLoader("fonts/little_numbers.bdf");
    rgb_matrix::Font littleNumberFont;
    littleNumberFontLoader.LoadFont(littleNumberFont);

    TextDrawer textDrawer(canvas, font, 0, 0, color, bg_color, "I", 0); // Draw Text
    textDrawer.DrawText();

    TextDrawer bigNumberDrawer1(canvas, bigNumberFont, 16, bigNumberFont.baseline() - 1, color, bg_color, "8", 1);
    bigNumberDrawer1.DrawText();

    TextDrawer bigNumberDrawer2(canvas, bigNumberFont, 38, bigNumberFont.baseline() - 1, color, bg_color, "9", 1);
    bigNumberDrawer2.DrawText();

    TextDrawer littleNumberDrawer(canvas, littleNumberFont, 7, littleNumberFont.baseline(), color, bg_color, "1 2 3", 0);
    littleNumberDrawer.DrawText();

    delete canvas;

    ///////// Start Game Loop /////////////
    bool game_running = true;
    int loop_count = 0;
    #define MAX_LOOP_COUNT 3
    while ( game_running ) {
        if ( loop_count >  MAX_LOOP_COUNT ) { game_running = false; }
        printf( "sleeping... " );
        sleep( 1 );
        printf( "Loop count: %d\n", loop_count++ ); }
    ///////// End Game Loop /////////////
    
    return 0;
}
