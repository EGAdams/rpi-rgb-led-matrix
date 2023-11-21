#ifndef MONITOR_LED_H
#define MONITOR_LED_H

#include "../MonitorLedClassObject/MonitorLedClassObject.h"
#include <string>

class MonitorLed {
public:
    MonitorLed();

    void setFail();
    void setPass();
    void setLedBackgroundColor(const std::string& newColor);
    void setLedTextColor(const std::string& newColor);
    void setLedText(const std::string& newText);

private:
    MonitorLedClassObject classObject;
    std::string ledText;
    static const std::string RUNNING_COLOR;
    static const std::string PASS_COLOR;
    static const std::string FAIL_COLOR;
};

#endif // MONITOR_LED_H
