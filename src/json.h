#ifndef JSON_H
#define JSON_H

#include <Arduino.h>

#define JSON_BUFFER_SIZE 512
#define MAX_PAIR_SIZE 128
#define MAX_FLOAT_SIZE 6

const char TRUE_STR[]  = "true";
const char FALSE_STR[]  = "false";
const char COMMA_STR[]  = ",";
const char BRACKET_OPEN_STR[]  = "{";
const char BRACKET_CLOSE_STR[]  = "}";
const char QUOTE_STR[]  = "\"";
const char KEY_END_STR[]  = "\": ";

enum Operation {
    None,
    OpenObject,
    CloseObject,
    AddKeyValue,
};

class Json {
    public:
        void open();
        void close();
        void addKeyValue(const char *key, const String &value);
        void addKeyValue(const char *key, const char *value);
        void addKeyValue(const char *key, float value);
        void addKeyValue(const char *key, bool value);
        void addKeyValue(const char *key);
        const char *build();
    private:
        char content[JSON_BUFFER_SIZE];
        Operation lastOperation = Operation::None;
        void addKeyValuePair(const char *key, const char *value);
        void append(const char *value);
};

#endif
