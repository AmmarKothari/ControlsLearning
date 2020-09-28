#include "lego_robot_model.h"
#include "constants.h"
#include <array>
#include <vector>
#include <math.h>
#include <iostream>


#define EPSILON 0.00001

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

pose LegoRobotModel::track_center_step(pose in_pose, std::vector<int> delta_ticks){
    pose out_pose;
    float r, l, alpha, cx, cy, xp, yp, R;
    r = (float)delta_ticks[0] * ticks_to_mm;
    l = (float)delta_ticks[1] * ticks_to_mm;
    // Do i need to deal with this more cleverly?
    // alpha = (r - l) / wheel_base;
    alpha = (l - r) / wheel_base;
    if (fabs(alpha) < EPSILON){
        out_pose.theta = in_pose.theta;
        out_pose.x = in_pose.x + cos(in_pose.theta)*l;
        out_pose.y = in_pose.y + sin(in_pose.theta)*l;
    }
    else{
        R = l / alpha;
        cx = in_pose.x - (R + wheel_base / 2.0) * sin(in_pose.theta);
        cy = in_pose.y - (R + wheel_base / 2.0) * -cos(in_pose.theta);
        out_pose.theta = in_pose.theta + alpha;
        out_pose.x = cx + (R + wheel_base / 2.0) * sin(out_pose.theta);
        out_pose.y = cy + (R + wheel_base / 2.0) * -cos(out_pose.theta);
    }
    return out_pose;
}

pose LegoRobotModel::scanner_step(pose in_pose, std::vector<int> delta_ticks){
    pose out_pose, in_track_center_pose, out_track_center_pose;
    // Transform to track center
    in_track_center_pose.theta = in_pose.theta;
    in_track_center_pose.x = in_pose.x + cos(in_pose.theta) * -scanner_displacement;
    in_track_center_pose.y = in_pose.y + sin(in_pose.theta) * -scanner_displacement;
    
    out_track_center_pose = track_center_step(in_track_center_pose, delta_ticks);

    // Transform back to scanner frame
    out_pose.theta = out_track_center_pose.theta;
    out_pose.x = out_track_center_pose.x + cos(out_track_center_pose.theta) * scanner_displacement;
    out_pose.y = out_track_center_pose.y + sin(out_track_center_pose.theta) * scanner_displacement;
    return out_pose;
}

point LegoRobotModel::convert_scanner_relative_to_world_xy(pose p, point scanner_relative_pos){
    /*p_world = b_world + R(theta_world) * [x, y]_relative */
    point world_pos;
    world_pos.x = p.x + scanner_relative_pos.x * cos(p.theta) - scanner_relative_pos.y * sin(p.theta);
    world_pos.y = p.y + scanner_relative_pos.x * sin(p.theta) + scanner_relative_pos.y * cos(p.theta);
    return world_pos;
}

std::vector<point> LegoRobotModel::convert_scanner_relative_to_world_xy(pose p, std::vector<point> scanner_relative_positions){
    point world_pos;
    std::vector<point> world_poses;
    if (scanner_relative_positions.size() > 0){
        for (auto scanner_relative_pos: scanner_relative_positions){
            world_pos = convert_scanner_relative_to_world_xy(p, scanner_relative_pos);
            world_poses.push_back(world_pos);
        }
    }
    return world_poses;
}




