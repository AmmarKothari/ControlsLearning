#pragma once

#include <string>
#include <vector>
#include <array>
#include "spatial_classes.h"


#define float_vec std::vector<float>
#define int_vec std::vector<int>
#define int_mat std::vector<std::vector<int>>

class LegoRobotLogFile{
    public:
        float_vec reference_positions;
        int_mat scan_data;
        float_vec pole_indices;
        int_mat rel_motor_ticks;
        int_mat abs_motor_ticks;
        float_vec filtered_positions;
        std::vector<point> landmarks;
        float_vec detected_cylinders;
        float last_ticks;
        void read(std::string filename);
};