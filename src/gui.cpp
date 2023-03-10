#include "gui.h"
#include <Arduino.h>

void GuiClass::begin() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
        Serial.println(F("Failed to connect to display!"));
        for (;;);
    }

    setDefaults();
    clear();
    display.display();
}

void GuiClass::setDefaults() {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setTextWrap(true);
}

void GuiClass::clear() {
    display.clearDisplay();
    setDefaults();
}

void GuiClass::setStatus(const char *status) {
    if (hasError) {
        return;
    }

    strcpy(this->status, status);
}

void GuiClass::setBody(const char *body) {
    if (hasError) {
        return;
    }

    strcpy(this->body, body);
}

void GuiClass::appendStatus(const char *status, bool newLine) {
    if (hasError) {
        return;
    }

    if (newLine) {
        strcat(this->status, "\n");
    }
    strcat(this->status, status);
}

void GuiClass::appendStatus(const String &status, bool newLine) {
    int len = status.length() + 1;
    char buffer[len];
    status.toCharArray(buffer, len);

    appendStatus(buffer, newLine);
}

void GuiClass::appendBody(const char *body, bool newLine) {
    if (hasError) {
        return;
    }

    if (newLine) {
        strcat(this->body, "\n");
    }
    strcat(this->body, body);
}

void GuiClass::appendBody(const String &body, bool newLine) {
    int len = body.length() + 1;
    char buffer[len];
    body.toCharArray(buffer, len);

    appendBody(buffer, newLine);
}

void GuiClass::show() {
    clear();
    showBody();
    showStatusLine();
    showStatus();

    display.display();
}

void GuiClass::showStatus() {
    if (hasError) {
        display.setTextSize(2);
    }
    display.setCursor(0, 0);
    display.print(status);
}

void GuiClass::showStatusLine() {
    display.drawLine(0, STATUS_HEIGHT, DISPL_WIDTH, STATUS_HEIGHT, WHITE);
}

void GuiClass::showBody() {
    display.setCursor(0, STATUS_HEIGHT + 5);
    display.print(body);
}

void GuiClass::showError(const char *error) {
    setStatus("ERROR");
    setBody(error);
    hasError = true;
    show();
}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_GUI)
GuiClass Gui;
#endif