#ifndef VelocityHistory_h
#define VelocityHistory_h

#include "Arduino.h"
#include "ArduinoSTL.h"
#include <vector>

class VelocityHistory{
    public:
        VelocityHistory(int, int);
        void update(std::vector<int>);
        float avg_velocity_over_time(int);
        int get_length();

    private:
        unsigned int _dims;
        int _history_length;
        std::vector<std::vector<int>> _history;  // vector<vector<time, pos>>; Should this be a deque?
};


#endif // VelocityHistory_h
