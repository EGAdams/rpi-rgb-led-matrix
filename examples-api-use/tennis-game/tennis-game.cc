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
        return usage(argv[0]);
      }
      break;
    default:
      return usage(argv[0]); }}

  if (bdf_font_file == NULL) {
    fprintf(stderr, "Need to specify BDF font-file with -f\n");
    return usage(argv[0]); }
  rgb_matrix::Font font; /* Load font. This needs to be a filename with a bdf bitmap font. */
  if (!font.LoadFont(bdf_font_file)) {
    fprintf(stderr, "Couldn't load font '%s'\n", bdf_font_file);
    return 1; }
                                                   
  rgb_matrix::Font *outline_font = NULL;                          // If we want an outline around the font,
  if ( with_outline ) { outline_font = font.CreateOutlineFont(); }  // we create a new font with the original font
                                                                  // as a template that is just an outline font.
  RGBMatrix *canvas = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
  if ( canvas == NULL ) { return 1; }

  const bool all_extreme_colors = (matrix_options.brightness == 100)
    && FullSaturation( color ) && FullSaturation( bg_color ) && FullSaturation( outline_color );
  if ( all_extreme_colors ) { canvas->SetPWMBits( 1 ); }
  const int x = x_orig;
  int y = y_orig;

  if (isatty(STDIN_FILENO)) {
    // Only give a message if we are interactive. If connected via pipe, be quiet
    printf("Enter lines. Full screen or empty line clears screen.\n"
           "Supports UTF-8. CTRL-D for exit.\n"); }
  
  canvas->Fill(flood_color.r, flood_color.g, flood_color.b); // clear screen
    /*if (outline_font) {
        // The outline font, we need to write with a negative (-2) text-spacing,
        // as we want to have the same letter pitch as the regular text that we then write on top.
        rgb_matrix::DrawText( canvas, *outline_font, x - 1, y + font.baseline(),
                            outline_color, &bg_color, line, letter_spacing - 2); } */
    // The regular text. Unless we already have filled the background with the outline font, we also fill the background here.
  Color firstRowColor( 0, 255, 0 );
  rgb_matrix::Font period_font;
  if (!period_font.LoadFont( "fonts/mspgothic_030623.bdf" )) {
    fprintf( stderr, "Couldn't load font '%s'\n", "fonts/mspgothic_030623.bdf" );
    return 1; }

  #define BIG_NUMBER_FONT "fonts/fgm_27_ee.bdf"
  rgb_matrix::Font big_number_font;
  if (!big_number_font.LoadFont( BIG_NUMBER_FONT )) { 
    fprintf( stderr, "Couldn't load font '%s'\n", BIG_NUMBER_FONT );
    return 1; }
  
  #define SPACE_BEFORE_1ST_NUMBER 16
  #define SPACE_BEFORE_2ND_NUMBER 2
  #define SPACE_BEFORE_SMALL_NUMBER 7
  #define SPACE_BETWEEN_SMALL_NUMBERS 17
  #define COORDS_FOR_SECOND_NUMBER 42
  #define SPACE_BETWEEN_FIRST_NUMBERS 38
  #define PIPE_SPACE 1
  #define LETTER_SPACING 1
  Color pipe_color( 255, 255, 0 );
  rgb_matrix::DrawText( canvas, big_number_font, x + PIPE_SPACE, y + big_number_font.baseline(), pipe_color,  outline_font ? NULL : &bg_color, "I", letter_spacing );
  rgb_matrix::DrawText( canvas, big_number_font, x + SPACE_BEFORE_1ST_NUMBER, y + big_number_font.baseline() - 1, firstRowColor, outline_font ? NULL : &bg_color, "8", LETTER_SPACING );
  rgb_matrix::DrawText( canvas, big_number_font, x + SPACE_BETWEEN_FIRST_NUMBERS, y + big_number_font.baseline() - 1, firstRowColor, outline_font ? NULL : &bg_color, "9", LETTER_SPACING );
  y += big_number_font.height();
  printf("Font height before second row: %d\n", big_number_font.height());
  rgb_matrix::DrawText( canvas, big_number_font, x + PIPE_SPACE, y + big_number_font.baseline() + 2, pipe_color,  outline_font ? NULL : &bg_color, "I", letter_spacing );
  Color secondRowColor( 255, 0, 0 );
  rgb_matrix::DrawText( canvas, big_number_font, x + SPACE_BEFORE_1ST_NUMBER, y + big_number_font.baseline() - 1, firstRowColor, outline_font ? NULL : &bg_color, "A", LETTER_SPACING );
  rgb_matrix::DrawText( canvas, big_number_font, x + COORDS_FOR_SECOND_NUMBER, y + big_number_font.baseline() - 1, firstRowColor, outline_font ? NULL : &bg_color, "d", LETTER_SPACING );
  
  y += big_number_font.height() + 4;
  printf("Font height before third row: %d\n", big_number_font.height());

  #define LITTLE_NUMBER_FONT "fonts/little_numbers.bdf"
  rgb_matrix::Font little_number_font;
  if (!little_number_font.LoadFont( LITTLE_NUMBER_FONT )) { 
    fprintf( stderr, "Couldn't load font '%s'\n", LITTLE_NUMBER_FONT );
    return 1; }
  Color thirdRowColor( 0, 255, 0 );
  rgb_matrix::DrawText( canvas, little_number_font, x + SPACE_BEFORE_SMALL_NUMBER, y + little_number_font.baseline(), thirdRowColor,  outline_font ? NULL : &bg_color, "1 2 3", letter_spacing );
  y += little_number_font.height() - 5;
  printf("Font height before fourth row: %d\n", little_number_font.height());
  Color fourthRowColor( 255, 0, 0 );
  rgb_matrix::DrawText( canvas, little_number_font, x + SPACE_BEFORE_SMALL_NUMBER, y + little_number_font.baseline(), fourthRowColor, outline_font ? NULL : &bg_color, "4", letter_spacing );
  rgb_matrix::DrawText( canvas, little_number_font, x + SPACE_BEFORE_SMALL_NUMBER + SPACE_BETWEEN_SMALL_NUMBERS, y + little_number_font.baseline(), fourthRowColor, outline_font ? NULL : &bg_color, "5", letter_spacing );
  rgb_matrix::DrawText( canvas, little_number_font, x + SPACE_BEFORE_SMALL_NUMBER + (( 2 * SPACE_BETWEEN_SMALL_NUMBERS )), y + little_number_font.baseline(), fourthRowColor, outline_font ? NULL : &bg_color, "6", letter_spacing );
  bool game_running = true;
  int loop_count = 0;
  #define MAX_LOOP_COUNT 3
  while ( game_running ) {
    if ( loop_count >  MAX_LOOP_COUNT ) { game_running = false; }
    printf( "sleeping... " );
    sleep( 1 );
    printf( "Loop count: %d\n", loop_count++ ); }
  delete canvas; // Finished. Shut down the RGB matrix.
  return 0;
}
