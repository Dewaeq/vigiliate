#ifndef GUI_H
#define GUI_H

#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define DISPL_WIDTH 128
#define DISPL_HEIGHT 64
#define STATUS_HEIGHT 15

#define STATUS_SIZE 48
#define BODY_SIZE 144

class GuiClass {
    public:
        void begin();
        void clear();
        void show();
        void setStatus(const char *status);
        void setBody(const char *body);
        void appendStatus(const char *status, bool newLine = false);
        void appendStatus(const String &status, bool newLine = false);
        void appendBody(const char *body, bool newLine = false);
        void appendBody(const String &body, bool newLine = false);
        void showError(const char *error);
    private:
        Adafruit_SSD1306 display = Adafruit_SSD1306(DISPL_WIDTH, DISPL_HEIGHT, &Wire);
        char status[STATUS_SIZE];
        char body[BODY_SIZE];
        bool hasError = false;
        void showStatus();
        void showBody();
        void showStatusLine();
        void setDefaults();
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_GUI)
extern GuiClass Gui;
#endif

#endif