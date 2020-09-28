#include <vector>
#include <iostream>
#include <array>
#include <math.h>

#include "lego_robot_scan_handler.h"
#include "constants.h"
#include "spatial_classes.h"

#define ZERO_SCAN_ANGLE_INDEX 330.0
#define ANGLE_PER_INDEX 0.006135923151543

arena::arena(){
    left = std::numeric_limits<float>::max();
    right = std::numeric_limits<float>::max();
    top = std::numeric_limits<float>::max();
    bottom = std::numeric_limits<float>::max();
};

LegoRobotScanHandler::LegoRobotScanHandler(){
    min_distance = 0.0;
}

LegoRobotScanHandler::LegoRobotScanHandler(float in_min_distance){
    min_distance = in_min_distance;
}

std::vector<float> LegoRobotScanHandler::scan_der(scan s){
    std::vector<float> scan_der;
    float prev_val, next_val;
    for (scan::iterator it = s.begin() + 1; it != s.end() - 1; it ++){
        prev_val = *(it-1);
        next_val = *(it+1);
        if (prev_val > min_distance and next_val > min_distance){
            scan_der.push_back((float)(next_val - prev_val) / 2.0);
        }
        else{
            // Could we eliminate the value from the scan data once we detect it as being erroneous?
            scan_der.push_back(0.0);
        }
    }
    // Want the output to have the same length as the input so add a 0 to start and end
    scan_der.insert(scan_der.begin(), 0.0);
    scan_der.push_back(0.0);
    return scan_der;
}

std::vector<CylinderScanDef> LegoRobotScanHandler::find_cylinders(scan s, std::vector<float> der, float depth_jump){
    std::vector<CylinderScanDef> cylinder_vec;
    CylinderScanDef current_cylinder;
    bool on_cylinder = false;
    float sum_ray = 0, sum_depth = 0, rays = 0;
    for (size_t i=0; i!=der.size(); i++){
        // if the depth jump is sufficiently large, then consider an object to be seen.
        if (der[i] < -depth_jump){
            on_cylinder = true;
            sum_ray = 0;
            sum_depth = 0;
            rays = 0;
        }
        if (der[i] > depth_jump){
            // If not on cylinder and see another indicator ending the end of the cylinder then update the last cylinder measurements.
            if (!on_cylinder and cylinder_vec.size() > 0){
                cylinder_vec.pop_back();
            }
            current_cylinder.angle = sum_ray / rays;
            current_cylinder.dist = sum_depth / rays;
            cylinder_vec.push_back(current_cylinder);
            on_cylinder = false;
        }
        
        // Zero indicates its an eroneous measurement
        if (s[i] >= min_distance){
            sum_ray += i;
            rays ++;
            sum_depth += s[i];
        }
    }
    return cylinder_vec;
}


std::vector<point> LegoRobotScanHandler::find_cylinders_relative_xy(scan s, std::vector<float> der, float depth_jump, float cylinder_offset){
    std::vector<CylinderScanDef> cylinder_vec;
    std::vector<point> cylinder_xy_vec;
    point xy;
    float angle;
    cylinder_vec = find_cylinders(s, der, depth_jump);
    for (auto c: cylinder_vec){
        angle = beam_index_to_angle(c.angle);
        xy = polar_to_cartesian(angle, c.dist + cylinder_offset);
        // xy.x = (c.dist + cylinder_offset) * cos(angle);
        // xy.y = (c.dist + cylinder_offset) * sin(angle);
        cylinder_xy_vec.push_back(xy);
    }
    return cylinder_xy_vec;
}

std::vector<point> LegoRobotScanHandler::find_cylinders_relative_xy(scan s, float depth_jump, float cylinder_offset){
    std::vector<CylinderScanDef> cylinder_vec;
    std::vector<point> cylinder_xy_vec;
    point xy;
    float angle;
    std::vector<float> der;
    der = scan_der(s);
    return find_cylinders_relative_xy(s, der, depth_jump, cylinder_offset);
    // cylinder_vec = find_cylinders(s, der, depth_jump);
    // for (auto c: cylinder_vec){
    //     angle = beam_index_to_angle(c.angle);
    //     xy = polar_to_cartesian(angle, c.dist + cylinder_offset);
        // xy.x = (c.dist + cylinder_offset) * cos(angle);
        // xy.y = (c.dist + cylinder_offset) * sin(angle);
    //     cylinder_xy_vec.push_back(xy);
    // }
    // return cylinder_xy_vec;
}

float beam_index_to_angle(float i, float mounting_angle){
    /* Convert a beam index to an angle, in radians. */
    return (i - ZERO_SCAN_ANGLE_INDEX) * ANGLE_PER_INDEX + mounting_angle;
}

std::vector<point> get_subsampled_points(scan s, unsigned int sampling){
    std::vector<point> subsample;
    float angle;
    for (size_t i=0; i<s.size(); i+= sampling){
        angle = beam_index_to_angle(i);
        subsample.push_back(polar_to_cartesian(angle, s[i]));
    }
    return subsample;
}

void get_corresponding_points_on_wall(std::vector<point> ps, arena a, float eps){

}
