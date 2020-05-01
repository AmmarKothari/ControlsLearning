#include "lego_robot_model.h"
#include <array>
#include <vector>
#include <math.h>
#include <iostream>


#define EPSILON 0.00001


void print_pose(std::array<float, 3> pose){
    std::printf("XY: [%.3f, %.3f], Theta: %.3f\n", pose[0], pose[1], pose[2]);
}


LegoRobotModel::LegoRobotModel(float ticks_to_mm_in, float wheel_base_in){
    ticks_to_mm = ticks_to_mm_in;
    wheel_base = wheel_base_in;
    scanner_displacement = 0.0;
}

LegoRobotModel::LegoRobotModel(float ticks_to_mm_in, float wheel_base_in, float scanner_displacement_in){
    ticks_to_mm = ticks_to_mm_in;
    wheel_base = wheel_base_in;
    scanner_displacement = scanner_displacement_in;
}

std::array<float, 3> LegoRobotModel::track_center_step(std::array<float, 3> in_pose, std::vector<int> delta_ticks){
    std::array<float, 3> out_pose;
    float r, l, alpha, cx, cy, xp, yp, R;
    r = (float)delta_ticks[0] * ticks_to_mm;
    l = (float)delta_ticks[1] * ticks_to_mm;
    alpha = (r - l) / wheel_base;
    if (fabs(alpha) < EPSILON){
        out_pose[2] = in_pose[2];
        out_pose[0] = in_pose[0] + cos(in_pose[2])*l;
        out_pose[1] = in_pose[1] + sin(in_pose[2])*l;
    }
    else{
        R = l / alpha;
        cx = in_pose[0] - (R + wheel_base / 2.0) * sin(in_pose[2]);
        cy = in_pose[1] - (R + wheel_base / 2.0) * -cos(in_pose[2]);
        out_pose[2] = in_pose[2] + alpha;
        out_pose[0] = cx + (R + wheel_base / 2.0) * sin(out_pose[2]);
        out_pose[1] = cy + (R + wheel_base / 2.0) * -cos(out_pose[2]);
    }
    return out_pose;
}

std::array<float, 3> LegoRobotModel::scanner_step(std::array<float, 3> in_pose, std::vector<int> delta_ticks){
    std::array<float, 3> out_pose, in_track_center_pose, out_track_center_pose;
    // Transform to track center
    in_track_center_pose[2] = in_pose[2];
    in_track_center_pose[0] = in_pose[0] + cos(in_pose[2]) * -scanner_displacement;
    in_track_center_pose[1] = in_pose[1] + sin(in_pose[2]) * -scanner_displacement;
    
    out_track_center_pose = track_center_step(in_track_center_pose, delta_ticks);

    // Transform back to scanner frame
    out_pose[2] = out_track_center_pose[2];
    out_pose[0] = out_track_center_pose[0] + cos(out_track_center_pose[2]) * scanner_displacement;
    out_pose[1] = out_track_center_pose[1] + cos(out_track_center_pose[2]) * scanner_displacement;
    return out_pose;
}




