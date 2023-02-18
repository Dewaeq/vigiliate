#ifndef TIMER_H
#define TIMER_H

class Timer {
    public:
        Timer (unsigned long interval) : interval(interval) {};
        void setInterval(unsigned long interval);
        bool isOver();
    private:
        unsigned long interval;
        unsigned long previousMillis;
};

#endif
