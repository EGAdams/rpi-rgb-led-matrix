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

void drawGridPattern(Canvas *canvas, int width, int height, int grid_size) {
  // Draw horizontal lines
  int iteration_count = 0;
  for (int y = 0; y < height; y += grid_size) {
    for (int x = 0; x < width; ++x) {
      canvas->SetPixel(x, y, 255, 255, 255);
      if (iteration_count % 100 == 0) {
        printf("Horizontal: Canvas: (%d, %d)\n", x, y);
      }
      iteration_count++;
    }
    // Pause for a moment after drawing each horizontal line
    usleep(500 * 1000); // 500 ms
  }

  // Draw vertical lines
  iteration_count = 0;
  for (int x = 0; x < width; x += grid_size) {
    for (int y = 0; y < height; ++y) {
      canvas->SetPixel(x, y, 255, 255, 255);
      if (iteration_count % 100 == 0) {
        printf("Vertical: Canvas: (%d, %d)\n", x, y);
      }
      iteration_count++;
    }
    // Pause for a moment after drawing each vertical line
    usleep(500 * 1000); // 500 ms
  }
  usleep(600000 * 1000); // 10 minutes
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
    defaults.rows = 64;
    defaults.cols = 32;
    defaults.brightness = 20;
    defaults.pixel_mapper_config = "164-mapper";
    defaults.multiplexing = 0;
    defaults.show_refresh_rate = false;
    defaults.chain_length = 4;
    defaults.parallel = 1;
    printf( "creating canvas from flags\n" );
    Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults);
    if ( canvas == NULL ) { return 1; }

    // It is always good to set up a signal handler to cleanly exit when we
    // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
    // for that.
    signal( SIGTERM, InterruptHandler );
    signal( SIGINT, InterruptHandler  );

    int num_panels_in_a_row = 1;
    int num_rows_of_panels = 1;
    // Initialize GPIO, matrix, and canvas
    // ...

    // Create the mapper object
    // OneSixFourMapper mapper(/*mapper arguments*/);

    // Set the mapper for the matrix
    // matrix->SetPixelMapper(&mapper);

    // Replace these values with the actual width and height of your panel
    int width = 64 * num_panels_in_a_row;
    int height = 64 * num_rows_of_panels;

    // Number of random pixels to display
    int num_pixels = 1000;

    // drawGridPattern(canvas, width, height, 10);
    drawGridPattern(canvas, width, height, 1);

    // ... (enter your main loop or other code)
    canvas->Clear();
    delete canvas;
    return 0;
    return 0;
}