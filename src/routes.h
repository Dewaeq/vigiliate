#ifndef ROUTES_H
#define ROUTES_H

namespace routes {
    void begin();

    void handleRoot();
    void handleConnect();
    void handleDisconnect();

    namespace api {
        void handleReadings();
        void handleStatus();
    } // namespace api
    
} // namespace routes


#endif