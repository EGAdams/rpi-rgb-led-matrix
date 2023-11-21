#include "../MonitorLedClassObject/MonitorLedClassObject.h"

MonitorLedClassObject::MonitorLedClassObject() 
    : background_color("lightyellow"), text_align("left"), margin_top("2px"), color("black") {}

std::string MonitorLedClassObject::getBackgroundColor() const {
    return background_color;
}

std::string MonitorLedClassObject::getTextAlign() const {
    return text_align;
}

std::string MonitorLedClassObject::getMarginTop() const {
    return margin_top;
}

std::string MonitorLedClassObject::getColor() const {
    return color;
}

void MonitorLedClassObject::setColor(const std::string& color) {
    this->color = color;
}

void MonitorLedClassObject::setBackgroundColor(const std::string& color) {
    this->background_color = color;
}
