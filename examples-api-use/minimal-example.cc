// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Small example how to use the library.
// For more examples, look at demo-main.cc
//
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)

#include "led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>

using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo)
{
    interrupt_received = true;
}

static void DrawOnCanvas(Canvas *canvas)
{
    /*
     * Let's create a simple animation. We use the canvas to draw
     * pixels. We wait between each step to have a slower animation.
     */
    canvas->Fill(0, 0, 255);

    int center_x = canvas->width() / 2;
    int center_y = canvas->height() / 2;
    float radius_max = canvas->width() / 2;
    float angle_step = 1.0 / 360;
    for (float a = 0, r = 0; r < radius_max; a += angle_step, r += angle_step)
    {
        if (interrupt_received)
            return;
        float dot_x = cos(a * 2 * M_PI) * r;
        float dot_y = sin(a * 2 * M_PI) * r;
        canvas->SetPixel(center_x + dot_x, center_y + dot_y,
                         255, 0, 0);
        usleep(1 * 500); // wait a little to slow down things.
    }
}

static void DrawLine(Canvas *canvas)
{
    canvas->Fill(0, 0, 0);

    for ( int test_count = 0; test_count < 500; test_count++ ) {
        for ( int y_position = 0; y_position < 128; y_position++ ) {
            for ( int x_position = 0; x_position < 64; x_position++ ) {
                // canvas->Fill( 0, 0, 0 );
                canvas->SetPixel( x_position, y_position, 0, 255, 0 );
                // printf( "canvas width: %d  canvas height: %d \n", canvas->width(), canvas->height() );
                usleep ( 1 * 200 * 100 );
            }
        }
        usleep ( 1 * 500 * 500 );
        canvas->Fill( 0, 0, 0 );
    } // wait a little to slow down things.

    printf("done drawing lines\n\n");
    usleep( 20 * 1000 * 1000);
} // wait a little to slow down things.

int main(int argc, char *argv[])
{
    printf( "starting main\n" );
    RGBMatrix::Options defaults;
    defaults.hardware_mapping = "regular"; // "adafruit-hat"; // or e.g. "adafruit-hat"
    defaults.rows = 64;
    defaults.cols = 64;
    defaults.brightness = 85;
    defaults.pixel_mapper_config = "Rotate:180;264-Mapper";
    // defaults.pixel_mapper_config = "U-mapper";
    defaults.multiplexing = 2; // 19 is the Superbowl mapper
    defaults.show_refresh_rate = false;
    defaults.chain_length = 1;
    defaults.parallel = 1;

    printf( "creating canvas from flags\n" );
    Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults);
    if ( canvas == NULL ) { return 1; }

    // It is always good to set up a signal handler to cleanly exit when we
    // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
    // for that.
    signal( SIGTERM, InterruptHandler );
    signal( SIGINT, InterruptHandler  );

    DrawLine( canvas ); // Using the canvas.
    // Animation finished. Shut down the RGB matrix.
    canvas->Clear();
    delete canvas;
    return 0;
}
