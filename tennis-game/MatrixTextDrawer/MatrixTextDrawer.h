#ifndef MATRIXTEXTDRAWER_H
#define MATRIXTEXTDRAWER_H

#include "../../include/graphics.h"  
#include "../ITextDrawer/ITextDrawer.h"
#include "../IDisplay/IDisplay.h"

class MatrixTextDrawer : public ITextDrawer {
private:
  IDisplay* _display;
  std::string _font_name;
  std::string _color_name;

public:
  // Constructor
  MatrixTextDrawer( IDisplay* display, const std::string& fontName, const std::string& colorName );

  // Set the font for the text drawer
  void setFont( const std::string& fontName );

  // Set the color for the text drawer
  void setColor( const std::string& colorName );

  // Override the drawText method
  void drawText( const std::string& text, int x, int y ) override;
};

#endif // MATRIXTEXTDRAWER_H
