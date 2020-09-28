#pragma once
#include <vector>
#include <array>
#include "spatial_classes.h"

#define DEFAULT_MOUNTING_ANGLE -0.06981317007977318
#define DEFAULT_SCAN_SUBSAMPLING 10

typedef std::vector<int> scan;

class CylinderScanDef{
    public:
        float dist, angle;
};


class arena{
    public:
        arena();
        float left, right, top, bottom;
};

class LegoRobotScanHandler{
    public:
        LegoRobotScanHandler();
        LegoRobotScanHandler(float in_min_distance);
        std::vector<float> scan_der(std::vector<int> scan);
        std::vector<CylinderScanDef> find_cylinders(std::vector<int> scan, std::vector<float> der, float depth_jump);
        std::vector<point> find_cylinders_relative_xy(std::vector<int> scan, std::vector<float> der, float depth_jump, float cylinder_offset);
        std::vector<point> find_cylinders_relative_xy(std::vector<int> scan, float depth_jump, float cylinder_offset);

    private:
        float min_distance;
};

float beam_index_to_angle(float, float = DEFAULT_MOUNTING_ANGLE);
std::vector<point> get_subsampled_points(scan s, unsigned int sampling=DEFAULT_SCAN_SUBSAMPLING);