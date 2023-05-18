#pragma once
#include "led-matrix.h"

using namespace rgb_matrix;

class CanvasCreator {
public:
    CanvasCreator(RGBMatrix::Options matrix_options, rgb_matrix::RuntimeOptions runtime_opt);
    RGBMatrix* CreateCanvas();

private:
    RGBMatrix::Options         _matrixOptions;
    rgb_matrix::RuntimeOptions _runtimeOptions;};