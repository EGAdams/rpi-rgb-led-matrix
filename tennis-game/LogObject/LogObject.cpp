#include "LogObject.h"

LogObject::LogObject() : id(""), timestamp(0), message(""), method("") {}

LogObject::~LogObject() {}

const std::string& LogObject::getId() const {
    return id;
}

void LogObject::setId(const std::string& id) {
    this->id = id;
}

uint64_t LogObject::getTimestamp() const {
    return timestamp;
}

void LogObject::setTimestamp(uint64_t timestamp) {
    this->timestamp = timestamp;
}

const std::string& LogObject::getMessage() const {
    return message;
}

void LogObject::setMessage(const std::string& message) {
    this->message = message;
}

const std::string& LogObject::getMethod() const {
    return method;
}

void LogObject::setMethod(const std::string& method) {
    this->method = method;
}
