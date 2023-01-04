#ifndef API_H
#define API_H

class ApiClass {
    public:
        void getReadings();
        void getStatus();
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_API)
extern ApiClass Api;
#endif

#endif