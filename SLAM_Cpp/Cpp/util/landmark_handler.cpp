#include "landmark_handler.h"
#include "constants.h"
#include <math.h>
#include <iostream>

#define FIX_SCALE true;  // Whether to scale the distance between the points or enforce rigid body transformations

class CylinderPairs{
    public:
        int measured, reference;
};

LandmarkHandler::LandmarkHandler(float in_max_radius){
    max_radius = in_max_radius;
}

std::vector<std::array<int, 2>> LandmarkHandler::find_cylinder_pairs(std::vector<point> measured_cylinders, std::vector<point> ref_cylinders){
    /*First index is the measured cylinder index, the second index is the reference cylinder index*/
    std::vector<std::array<int, 2>> pair_indexes;
    std::array<int, 2> index = {-1, -1};
    float dist, min_dist;
    for (int i1=0; i1<measured_cylinders.size(); i1++){
        min_dist = 10000;
        for (int i2=0; i2<ref_cylinders.size(); i2++){
            dist = hypot(measured_cylinders[i1].x - ref_cylinders[i2].x, measured_cylinders[i1].y - ref_cylinders[i2].y);
            if (dist < min_dist){
                min_dist = dist;
                index = {i1, i2};
            }
        }
        if (min_dist < max_radius){
            pair_indexes.push_back(index);
        }
    }
    return pair_indexes;
}

LandmarkTransform LandmarkHandler::get_estimated_transform(std::vector<point> measured_cylinders, std::vector<point> reference_cylinders){
    std::vector<std::array<int, 2>> index_pairs;
    index_pairs = find_cylinder_pairs(measured_cylinders, reference_cylinders);
    LandmarkTransform transform = {0.0, 0.0, 0.0, 0.0, 0.0, true};
    if (index_pairs.size() == 0){
        std::cout << "No matching pairs found" << std::endl;
        transform.valid = false;
        return transform;
    }

    std::vector<point> cylinders_in_world, cylinders_in_reference;
    for (auto pair: index_pairs){
        cylinders_in_world.push_back(measured_cylinders[pair[0]]);
        cylinders_in_reference.push_back(reference_cylinders[pair[1]]);
    }
    point measured_center = compute_center(cylinders_in_world);
    point reference_center = compute_center(cylinders_in_reference);

    // Convert to optimal coordinates
    std::vector<point> cylinders_in_oc, reference_cylinders_oc;
    cylinders_in_oc = compute_optimal_coordinates(measured_center, cylinders_in_world);
    reference_cylinders_oc = compute_optimal_coordinates(reference_center, cylinders_in_reference);

    int num_cylinders = index_pairs.size();
    float cs = 0, ss = 0, rr = 0, ll = 0;
    for (int i=0; i<num_cylinders; i++){
        cs += reference_cylinders_oc[i].x * cylinders_in_oc[i].x + reference_cylinders_oc[i].y * cylinders_in_oc[i].y;
        ss += -reference_cylinders_oc[i].x * cylinders_in_oc[i].y + reference_cylinders_oc[i].y * cylinders_in_oc[i].x;
        rr += reference_cylinders_oc[i].x * reference_cylinders_oc[i].x + reference_cylinders_oc[i].y * reference_cylinders_oc[i].y;
        ll += cylinders_in_oc[i].x * cylinders_in_oc[i].x + cylinders_in_oc[i].y * cylinders_in_oc[i].y;
    }
    if (ll == 0.0 or rr == 0.0){
        transform.valid = false;
    }
    if (true){
        transform.lambda = 1.0;
    }
    else{
        transform.lambda = sqrt(rr / ll);
    }
    float c, s, cs_norm;
    cs_norm = hypot(cs, ss);
    transform.c = cs / cs_norm;
    transform.s = ss / cs_norm;

    float tx, ty;
    transform.tx = reference_center.x - transform.lambda * (transform.c * measured_center.x - transform.s * measured_center.y);
    transform.ty = reference_center.y - transform.lambda * (transform.s * measured_center.x + transform.c * measured_center.y);
    return transform;
}

std::array<std::vector<point>, 2> get_corresponding_points_on_wall(std::vector<point> points, float arena_left, float arena_right, float arena_bottom, float arena_top, float epsilon){
    std::vector<point> scan_points, wall_points;
    point wall_pt;
    for (auto pt: points){
        if (abs(pt.x - arena_left) < epsilon){
            scan_points.push_back(pt);
            wall_pt = {arena_left, pt.y};
            wall_points.push_back(wall_pt);
        }
        else if (abs(pt.x - arena_right) < epsilon){
            scan_points.push_back(pt);
            wall_pt = {arena_right, pt.y};
            wall_points.push_back(wall_pt);
        }
        else if (abs(pt.y - arena_bottom) < epsilon){
            scan_points.push_back(pt);
            wall_pt = {pt.x, arena_bottom};
            wall_points.push_back(wall_pt);
        }
        else if (abs(pt.y - arena_top) < epsilon){
            scan_points.push_back(pt);
            wall_pt = {pt.x, arena_top};
            wall_points.push_back(wall_pt);
        }
    }
    std::array<std::vector<point>, 2>  return_array = {scan_points, wall_points};
    return return_array;
}



// **********************
// PRIVATE METHODS
// **********************

point LandmarkHandler::compute_center(std::vector<point> vecs){
    // Returns the point that is the average of all the points.
    point center = {0.0, 0.0};
    float num_points = vecs.size();
    for (auto p: vecs){
        center.x += p.x;
        center.y += p.y;
    }
    if (num_points > 0){
        center.x /= num_points;
        center.y /= num_points;
    }
    return center;
}

std::vector<point> LandmarkHandler::compute_optimal_coordinates(point center, std::vector<point> vec){
    std::vector<point> vec_oc;
    for (auto p: vec){
        point oc;
        oc.x = p.x - center.x;
        oc.y = p.y - center.y;
        vec_oc.push_back(oc);
    }
    return vec_oc;
}


// **********************
// DEBUG METHODS
// **********************