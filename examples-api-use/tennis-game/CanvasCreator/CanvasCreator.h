#pragma once
using namespace rgb_matrix;
#include "led-matrix.h"
class CanvasCreator {
public:
    CanvasCreator(RGBMatrix::Options matrix_options, rgb_matrix::RuntimeOptions runtime_opt);
    RGBMatrix* CreateCanvas();
private:
    RGBMatrix::Options _matrixOptions;
    rgb_matrix::RuntimeOptions _runtimeOptions;};