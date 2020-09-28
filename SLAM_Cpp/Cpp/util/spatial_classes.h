#pragma once

#include <vector>



struct point{
    float x = 0.0, y = 0.0;
};

struct pose : point{
    float theta = 0.0;
};

struct LandmarkTransform{
    float lambda, c, s, tx, ty;
    bool valid = true;
};

void print_pose(pose p);
std::string pose_to_csv(pose p);
std::string point_to_csv(point p);


point apply_transform(LandmarkTransform t, point p);
pose apply_transform(LandmarkTransform t, pose p);
std::vector<point> apply_transform(LandmarkTransform t, std::vector<point> vec);
point polar_to_cartesian(float angle, float dist);