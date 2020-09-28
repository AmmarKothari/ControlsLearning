#pragma once
#include <vector>
#include <array>
#include "spatial_classes.h"

class LandmarkHandler{
    public:
        LandmarkHandler(float);
        std::vector<std::array<int, 2>> find_cylinder_pairs(std::vector<point>, std::vector<point>);
        LandmarkTransform get_estimated_transform(std::vector<point>, std::vector<point>);
        std::vector<point> apply_transform(LandmarkTransform, std::vector<point>);
        point apply_transform(LandmarkTransform, point);
        std::array<std::vector<point>, 2> get_corresponding_points_on_wall(std::vector<point> points, float arena_left = 0.0, float arena_right = 0.0, float arena_bottom = 2000.0, float arena_top = 2000.0, float epsilon = 150.0);
    private:
        float max_radius;
        point compute_center(std::vector<point>);
        std::vector<point> compute_optimal_coordinates(point, std::vector<point>);

};