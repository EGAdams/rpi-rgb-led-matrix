#ifndef MONITOR_LED_CLASS_OBJECT_H
#define MONITOR_LED_CLASS_OBJECT_H

#include <string>

class MonitorLedClassObject {
public:
    MonitorLedClassObject();

    std::string getBackgroundColor() const;
    void setBackgroundColor(const std::string& color);
    std::string getTextAlign() const;
    void setTextAlign(const std::string& textAlign);
    std::string getMarginTop() const;
    void setMarginTop(const std::string& marginTop);
    std::string getColor() const;
    void setColor(const std::string& color);

private:
    std::string background_color;
    std::string text_align;
    std::string margin_top;
    std::string color;
};
#endif // MONITOR_LED_CLASS_OBJECT_H
