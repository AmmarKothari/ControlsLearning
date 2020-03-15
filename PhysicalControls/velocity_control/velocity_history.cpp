#include "velocity_history.h"

//VelocityHistory::VelocityHistory(){
//    _dims = 0;
//    _history_length = 0;
//}

VelocityHistory::VelocityHistory(int dims, int length){
    _dims = dims;
    _history_length = length;
}

void VelocityHistory::update(std::vector<int> _input_data){
    // remove values if too long
    if (_history.size() > _dims) {
        _history.pop_back();
    }
    // append new value
    _history.insert(_history.begin(), _input_data);
}

int VelocityHistory::get_length(){
    return _history_length;
}

float VelocityHistory::avg_velocity_over_time(int look_back_time){
    // For each value in history starting from the front
        // if the time difference is greater than the look back time
            // break
        // else update the position difference and time to compare to
    // return velocity difference
    noInterrupts();
    std::vector<std::vector<int>> _history_copy = _history;
    interrupts();

    int current_time = millis();
    int t0;  // The time to compare against for measurement.
    int pos0;  // The position to compare against for measurement.

    for (auto x: _history_copy){
        // how to deal with roll over?
        if ((current_time - x[0]) < look_back_time) {
            pos0 = x[1];
            t0 = x[0];
        }
        else {
            break;
        }
    }
    std::vector<int> latest_measurement = _history_copy.back();
    float velocity_estimate;
    float dt = latest_measurement[0] - t0;
    if (dt == 0){
        velocity_estimate = 0.0;
    }
    else {
        velocity_estimate = (latest_measurement[1] - pos0) / dt;
    }
    return velocity_estimate;
}
