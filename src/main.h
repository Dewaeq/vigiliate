#ifndef MAIN_H
#define MAIN_H

void setupWebServer();

// routes
void handleRoot();
void handleDisconnect();
void handleReadings();
void handleStatus();

// UI methods
void setStatus();

#endif
