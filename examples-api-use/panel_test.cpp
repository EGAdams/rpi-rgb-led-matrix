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
    RGBMatrix::Options matrix_options;
    rgb_matrix::RuntimeOptions runtime_options;

    if (!ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_options)) {
        return 1;
    }

    RGBMatrix *matrix = CreateMatrixFromOptions(matrix_options, runtime_options);
    if (matrix == NULL) {
        return 1;
    }
    // Configure the RGB matrix
    // RGBMatrix::Options matrix_options;
    matrix_options.rows = 32;         // Set the number of rows of your panel
    matrix_options.cols = 64;         // Set the number of columns of your panel
    matrix_options.chain_length = 1;  // Set the number of chained panels
    matrix_options.parallel = 1;      // Set the number of parallel chains
    matrix_options.hardware_mapping = "regular"; // Set the GPIO mapping
    matrix_options.brightness = 20;

    // Create the RGB matrix
    // RGBMatrix *matrix = CreateMatrixFromOptions(matrix_options, argc, &argv);
    if (matrix == nullptr) {
        std::cerr << "Failed to create RGB matrix." << std::endl;
        return 1;
    }

    // Draw a test pattern
    for (int x = 0; x < matrix_options.cols; ++x) {
        usleep( 500000 );
        for (int y = 0; y < matrix_options.rows; ++y) {
            int red = (x % 8) * 32;
            int green = (y % 8) * 32;
            int blue = ((x + y) % 8) * 32;
            matrix->SetPixel(x, y, red, green, blue);
            usleep( 100000 );
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