
#include "json.h"

void Json::open() {
    if (this->lastOperation == Operation::CloseObject) {
        this->content += ",";
    }

    this->content += "{";
    this->lastOperation = Operation::OpenObject;
}

void Json::close() {
    this->content += "}";
    this->lastOperation = Operation::CloseObject;
}

void Json::addKeyValue(String key, String value) {
    String newValue = '"' + value + '"';
    this->addKeyValuePair(key, newValue);
}

void Json::addKeyValue(String key, float value) {
    String newValue = String(value);
    this->addKeyValuePair(key, newValue);
}

void Json::addKeyValue(String key, bool value) {
    String newValue = value ? "true" : "false";
    this->addKeyValuePair(key, newValue);
}

void Json::addKeyValuePair(String key, String value) {
    if (this->lastOperation == Operation::AddKeyValue) {
        this->content += ",";
    }

    this->content += "\"" + key + "\": " + value;
    this->lastOperation = Operation::AddKeyValue;
}

String Json::build() {
    return this->content;
}