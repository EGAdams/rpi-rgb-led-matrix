#ifndef MATRIX_DISPLAY_H
#define MATRIX_DISPLAY_H

#include "../TennisConstants/TennisConstants.h"
#include "../IDisplay/IDisplay.h"
#include "../../include/led-matrix.h"
#include "../ColorManager/ColorManager.h"
#include "../FontManager/FontManager.h"

class MatrixDisplay : public IDisplay {
private:
  rgb_matrix::RGBMatrix* _canvas;
  rgb_matrix::Font       _currentFont;
  rgb_matrix::Color      _currentColor;

public:
  MatrixDisplay( rgb_matrix::RGBMatrix* canvas );
  ~MatrixDisplay();

  void setFont( const std::string& fontName );
  void setColor( const std::string& colorName );
 
  void drawText( const std::string& text, int x, int y ) override; // IDisplay implementations
  void clearScreen() override;
};
#endif // MATRIX_DISPLAY_H
