#include <iostream>
#include <math.h>

#include "spatial_classes.h"


void print_pose(pose p){
    std::printf("XY: [%.3f, %.3f], Theta: %.3f\n", p.x, p.y, p.theta);
}

std::string pose_to_csv(pose p){
    char buff[100];
    std::snprintf(buff, sizeof(buff), "%.3f, %.3f, %.3f", p.x, p.y, p.theta);
    std::string out = buff;
    return out;
}

std::string point_to_csv(point p){
    char buff[100];
    std::snprintf(buff, sizeof(buff), "%.3f, %.3f", p.x, p.y);
    std::string out = buff;
    return out;
}

// Make this a template so works for both points and poses!
point apply_transform(LandmarkTransform t, point p){
    float lac, las;
    point transformed_point;
    lac = t.lambda * t.c;
    las = t.lambda * t.s;
    transformed_point.x = lac * p.x - las * p.y + t.tx;
    transformed_point.y = las * p.x + lac * p.y + t.ty;
    return transformed_point;
}


pose apply_transform(LandmarkTransform t, pose p){
    point p_transformed = apply_transform(t, (point) p);
    pose pose_transformed;
    pose_transformed.x = p_transformed.x;
    pose_transformed.y = p_transformed.y;
    pose_transformed.theta = p.theta + atan2(t.s, t.c);
    return pose_transformed;
}

std::vector<point> apply_transform(LandmarkTransform t, std::vector<point> vec){
    std::vector<point> transformed_points;
    for (auto p: vec){
        transformed_points.push_back(apply_transform(t, p));
    }
    return transformed_points;
}

point polar_to_cartesian(float angle, float dist){
    point xy;
    xy.x = dist * cos(angle);
    xy.y = dist * sin(angle);
    return xy;
}