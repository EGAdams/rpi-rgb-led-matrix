#include <iostream>
#include <unistd.h>
#include "../include/led-matrix.h"

using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;

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
        for (int y = 0; y < matrix_options.rows; ++y) {
            int red = (x % 8) * 32;
            int green = (y % 8) * 32;
            int blue = ((x + y) % 8) * 32;
            matrix->SetPixel(x, y, red, green, blue);
        }
    }

    // Sleep for a while to keep the pattern visible
    usleep(10000000); // 10 seconds

    // Clean up and exit
    delete matrix;
    return 0;
}