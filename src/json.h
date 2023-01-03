#ifndef JSON_H
#define JSON_H

#include <Arduino.h>

enum Operation {
    None,
    OpenObject,
    CloseObject,
    AddKeyValue,
};

class Json {
    public:
        String content;
        void open();
        void close();
        void addKeyValue(String key, String value);
        void addKeyValue(String key, const char *value);
        void addKeyValue(String key, float value);
        void addKeyValue(String key, bool value);
        String build();
    private:
        void addKeyValuePair(String key, String value);
        Operation lastOperation = Operation::None;
};

#endif
