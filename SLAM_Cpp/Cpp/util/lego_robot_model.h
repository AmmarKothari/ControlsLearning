#pragma once

#include <array>
#include <vector>
#include "spatial_classes.h"


class LegoRobotModel{
    public:
        LegoRobotModel(float, float);
        LegoRobotModel(float, float, float);
        pose track_center_step(pose in_pose, std::vector<int> delta_ticks);
        pose scanner_step(pose in_pose, std::vector<int> delta_ticks);
        point convert_scanner_relative_to_world_xy(pose pose, point scanner_relative_pos);
        std::vector<point> convert_scanner_relative_to_world_xy(pose poses, std::vector<point> scanner_relative_pos);

    private:
        float ticks_to_mm, wheel_base, scanner_displacement;
};