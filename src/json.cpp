#include "json.h"

void Json::open() {
    if (lastOperation == Operation::None) {
        strcpy(content, BRACKET_OPEN_STR);
    }
    else if (lastOperation == Operation::CloseObject) {
        append(COMMA_STR);
        append(BRACKET_OPEN_STR);
    }

    lastOperation = Operation::OpenObject;
}

void Json::close() {
    append(BRACKET_CLOSE_STR);
    lastOperation = Operation::CloseObject;
}

void Json::addKeyValue(const char *key, const String &value) {
    char buffer[MAX_PAIR_SIZE];
    value.toCharArray(buffer, MAX_PAIR_SIZE);

    addKeyValue(key, buffer);
}

void Json::addKeyValue(const char *key, float value) {
    char buffer[MAX_FLOAT_SIZE];
    dtostrf(value, MAX_FLOAT_SIZE, 2, buffer);

    addKeyValuePair(key, buffer);
}

void Json::addKeyValue(const char *key, bool value) {
    const char *newValue = value ? TRUE_STR : FALSE_STR;
    addKeyValuePair(key, newValue);
}

void Json::addKeyValue(const char *key) {
    addKeyValuePair(key, "null");
}

void Json::addKeyValue(const char *key, const char *value) {
    char buffer[MAX_PAIR_SIZE];

    strcpy(buffer, QUOTE_STR);
    strcat(buffer, value);
    strcat(buffer, QUOTE_STR);

    addKeyValuePair(key, buffer);
}

void Json::addKeyValuePair(const char *key, const char *value) {
    if (lastOperation == Operation::AddKeyValue) {
        append(COMMA_STR);
    }

    append(QUOTE_STR);
    append(key);
    append(KEY_END_STR);
    append(value);

    lastOperation = Operation::AddKeyValue;
}

void Json::append(const char *value) {
    strcat(content, value);
}

const char *Json::build() {
    return content;
}