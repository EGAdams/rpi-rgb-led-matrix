//
//
#include "led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>

using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;

#include <ctime>
#include <cstdlib>
#include <vector>

void displayRandomPixels(Canvas *canvas, int width, int height, int num_pixels) {
  std::srand(std::time(0));

  for (int i = 0; i < num_pixels; ++i) {
    int x = std::rand() % width;
    int y = std::rand() % height;

    // Set the pixel color (You can choose any color you want)
    canvas->SetPixel(x, y, 255, 0, 0);

    // Sleep for a moment to slow down the process
    usleep(500 * 1000);
  }
}

volatile bool interrupt_received = false;
static void InterruptHandler(int signo)
{
    interrupt_received = true;
}

int main(int argc, char *argv[]) {
    // ... (initialize canvas and other objects)
    printf( "starting main\n" );
    RGBMatrix::Options defaults;
    defaults.hardware_mapping = "regular"; // or e.g. "adafruit-hat"
    defaults.rows = 32;
    defaults.cols = 32;
    defaults.brightness = 20;
    defaults.pixel_mapper_config = "164-mapper";
    defaults.multiplexing = 0; // 19 is the Superbowl mapper
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

    // Initialize GPIO, matrix, and canvas
    // ...

    // Create the mapper object
    // OneSixFourMapper mapper(/*mapper arguments*/);

    // Set the mapper for the matrix
    // matrix->SetPixelMapper(&mapper);

    // Replace these values with the actual width and height of your panel
    int width = 64 * num_panels_in_a_row;
    int height = 32 * num_rows_of_panels;

    // Number of random pixels to display
    int num_pixels = 25;

    displayRandomPixels(canvas, width, height, num_pixels);

    // ... (enter your main loop or other code)
    canvas->Clear();
    delete canvas;
    return 0;
    return 0;
}