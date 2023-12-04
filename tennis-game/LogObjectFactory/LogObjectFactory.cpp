#include "LogObjectFactory.h"
#include <chrono>
#include <random>
#include <sstream>

LogObjectFactory::LogObjectFactory() {
    // Constructor implementation (if needed)
}

LogObjectFactory::~LogObjectFactory() {
    // Destructor implementation (if needed)
}

std::shared_ptr<LogObject> LogObjectFactory::createLogObject(const std::string& message, const std::string& method) {
    auto logObject = std::make_shared<LogObject>();
    logObject->setId(generateUniqueId());
    logObject->setTimestamp(getCurrentTimestamp());
    logObject->setMessage(message);
    logObject->setMethod(method);
    return logObject;
}

std::string LogObjectFactory::generateUniqueId() {
    // Unique ID generation logic (random, hash, etc.)
    std::stringstream ss;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);
    ss << "LOG_" << dis(gen);
    return ss.str();
}

uint64_t LogObjectFactory::getCurrentTimestamp() {
    // Getting the current timestamp
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}
