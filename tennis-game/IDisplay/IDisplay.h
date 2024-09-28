#ifndef IDISPLAY_H
#define IDISPLAY_H
#include <string>
#include "../../include/graphics.h"  // Adjust this path as necessary

class IDisplay {
public:
  virtual void drawText( const std::string& text, int x, int y ) = 0;
  virtual void clearScreen() = 0;
  virtual ~IDisplay() = default;
};


#endif // IDISPLAY_H
