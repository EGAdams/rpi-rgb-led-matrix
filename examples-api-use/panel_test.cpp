#include <iostream>
#include <unistd.h>
#include "../include/led-matrix.h"
#include <unistd.h>

using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;

// void DrawBorder(RGBMatrix *matrix, const Color &color) {
//   for (int x = 0; x < matrix->width(); ++x) {
//     matrix->SetPixel(x, 0, color.r, color.g, color.b);
//     matrix->SetPixel(x, matrix->height() - 1, color.r, color.g, color.b);
//   }
//   for (int y = 0; y < matrix->height(); ++y) {
//     matrix->SetPixel(0, y, color.r, color.g, color.b);
//     matrix->SetPixel(matrix->width() - 1, y, color.r, color.g, color.b);
//   }
// }

// void DrawArrow(RGBMatrix *matrix, const Color &color) {
//   int center_x = matrix->width() / 2;
//   int center_y = matrix->height() / 2;
//   matrix->SetPixel(center_x, center_y, color.r, color.g, color.b);
//   matrix->SetPixel(center_x - 1, center_y, color.r, color.g, color.b);
//   matrix->SetPixel(center_x + 1, center_y, color.r, color.g, color.b);
//   matrix->SetPixel(center_x, center_y - 1, color.r, color.g, color.b);
//   matrix->SetPixel(center_x, center_y + 1, color.r, color.g, color.b);
//   matrix->SetPixel(center_x - 1, center_y - 1, color.r, color.g, color.b);
// }

int main(int argc, char *argv[]) {
    printf( "running panel_test.cpp... \n" );
    RGBMatrix::Options matrix_options;
    rgb_matrix::RuntimeOptions runtime_options;

    // last working command:
    // sudo examples-api-use/panel_test --led-no-hardware-pulse --led-multiplexing=1 --led-pixel-mapper="Rotate:270;Mirror:V"
    // Configure the RGB matrix
    // RGBMatrix::Options matrix_options;
    matrix_options.rows = 128;         // Set the number of rows of your panel
    matrix_options.cols = 64;         // Set the number of columns of your panel
    matrix_options.chain_length = 1;  // Set the number of chained panels
    matrix_options.parallel = 1;      // Set the number of parallel chains
    matrix_options.hardware_mapping = "regular"; // Set the GPIO mapping
    matrix_options.brightness = 20;
    // matrix_options.multiplexing = 2;
    // matrix_options.pixel_mapper_config = "164-mapper"; 
    // matrix_options.panel_type = "";
    if (!ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_options)) { return 1; }

    // Create the RGB matrix
    // RGBMatrix *matrix = CreateMatrixFromOptions( matrix_options, argc, &argv );
    RGBMatrix *matrix = CreateMatrixFromOptions ( matrix_options, runtime_options );
    if (matrix == nullptr) {
        std::cerr << "Failed to create RGB matrix." << std::endl;
        return 1;
    }

    printf( "number of rows: %d \n" , matrix_options.rows );
    printf( "number of cols: %d \n" , matrix_options.cols );

    printf( "drawing test pattern... " );
    for (int x = 0; x < matrix_options.cols; ++x) {
        usleep( 500 );
        for (int y = 0; y < matrix_options.rows; ++y) {
            int red = 0;
            int green = 255;
            int blue = 0;
            matrix->SetPixel(x, y, red, green, blue);
            printf( "x: %d, y: %d \n" , x, y );
            usleep( 5000 );
        }
    }

    // Sleep for a while to keep the pattern visible
    usleep( 2000000 ); // 10 seconds

    // // Draw a border around the panel
    // DrawBorder(matrix, Color(255, 255, 255));

    // // Draw an arrow in the center of the panel to indicate orientation
    // DrawArrow(matrix, Color(255, 0, 0));

    // // Wait for a while to observe the pattern
    // sleep(10);

    // Clean up and exit
    delete matrix;
    return 0;
}