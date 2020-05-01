#pragma once
#include <array>
#include <vector>

class LegoRobotModel{
    public:
        LegoRobotModel(float, float);
        LegoRobotModel(float, float, float);
        std::array<float, 3> track_center_step(std::array<float, 3> in_pose, std::vector<int> delta_ticks);
        std::array<float, 3> scanner_step(std::array<float, 3> in_pose, std::vector<int> delta_ticks);

    private:
        float ticks_to_mm, wheel_base, scanner_displacement;
};