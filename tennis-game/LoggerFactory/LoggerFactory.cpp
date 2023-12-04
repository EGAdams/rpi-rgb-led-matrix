#include "LoggerFactory.h"
#include <iostream>

std::string LoggerFactory::FACTORY_ID = "2021";
std::string LoggerFactory::OBJECT_STORAGE_URL = ""; // Should be set externally

std::unique_ptr<MonitoredObject> LoggerFactory::createLogger(const std::string& target_object) {
    if (OBJECT_STORAGE_URL.empty()) {
        // In the absence of a direct JS equivalent, we use a simple console logger.
        return std::make_unique<MonitoredObject>(target_object + "_ConsoleLogger");
    } else {
        // Building the logger with more complex functionality
        std::string new_id = target_object + "_" + FACTORY_ID;
        auto logger = std::make_unique<MonitoredObject>(new_id);
        // Additional setup or configuration can be added here.
        return logger;
    }
}
