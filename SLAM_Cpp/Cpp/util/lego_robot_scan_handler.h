#pragma once
#include <vector>
#include <array>

#define DEFAULT_MOUNTING_ANGLE -0.06981317007977318

class LegoRobotScanHandler{
    public:
        LegoRobotScanHandler();
        LegoRobotScanHandler(float in_min_distance);
        std::vector<float> scan_der(std::vector<int> scan);
        std::vector<std::array<float, 2>> find_cylinders(std::vector<int> scan, std::vector<float> der, float depth_jump);

    private:
        float min_distance;
};

float beam_index_to_angle(float, float = DEFAULT_MOUNTING_ANGLE);
