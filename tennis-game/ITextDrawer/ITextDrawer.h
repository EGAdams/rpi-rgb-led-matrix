#ifndef TEXTDRAWER_H
#define TEXTDRAWER_H

#include <string>

class ITextDrawer {
public:
    virtual void drawText(const std::string& text, int x, int y ) = 0;
    virtual ~ITextDrawer() = default;
    virtual void setFont(  const std::string& fontName  ) = 0;
    virtual void setColor( const std::string& colorName ) = 0;

private:
    std::string _fontName;
    std::string _colorName;
};

#endif // TEXTDRAWER_H