#include "MonitorLed.h"

const std::string MonitorLed::RUNNING_COLOR = "lightyellow";
const std::string MonitorLed::PASS_COLOR = "lightgreen";
const std::string MonitorLed::FAIL_COLOR = "#fb6666"; // Assuming CSS-like color notation

MonitorLed::MonitorLed() {
    this->ledText = "ready.";
}

void MonitorLed::setFail() {
    setLedBackgroundColor(FAIL_COLOR);
    setLedTextColor("white");
}

void MonitorLed::setPass() {
    setLedBackgroundColor(PASS_COLOR);
    setLedTextColor("black");
}

void MonitorLed::setLedBackgroundColor(const std::string& newColor) {
    classObject.setBackgroundColor(newColor);
}

void MonitorLed::setLedTextColor(const std::string& newColor) {
    classObject.setColor(newColor);
}

void MonitorLed::setLedText(const std::string& newText) {
    this->ledText = newText;
}
