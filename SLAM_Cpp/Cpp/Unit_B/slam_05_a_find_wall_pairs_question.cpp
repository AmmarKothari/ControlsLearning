#include <fstream>
#include <string>
#include <math.h>
#include <array>
#include <iostream>
#include <iomanip>

#include "matplotlibcpp.h"
#include "lego_robot_log_file.h"
#include "lego_robot_model.h"
#include "constants.h"
#include "lego_robot_scan_handler.h"
#include "landmark_handler.h"
#include "spatial_classes.h"
#include "output_formatting.h"

#define DATA_PATH "../../Data/Unit_A/"
#define SCAN_DATA_FN "robot4_scan.txt"
#define MOTOR_DATA_FN "robot4_motors.txt"
#define LANDMARKS_FN "robot_arena_landmarks.txt"

// #define landmarks_file std::cout

int main(){
    float depth_jump = 100.0, cylinder_offset = 90.0;
    pose current_pose = {1850.0, 1897.0, 3.717551306747922};
    float rejection_radius = 300.0;

    char scan_data_file_path[100];
    strcpy(scan_data_file_path, DATA_PATH);
    strcat(scan_data_file_path, SCAN_DATA_FN);

    char motor_data_file_path[100];
    strcpy(motor_data_file_path, DATA_PATH);
    strcat(motor_data_file_path, MOTOR_DATA_FN);

    char arena_landmarks[100];
    strcpy(arena_landmarks, DATA_PATH);
    strcat(arena_landmarks, LANDMARKS_FN);
    
    LegoRobotLogFile lego_log_file;
    lego_log_file.read(scan_data_file_path);
    lego_log_file.read(motor_data_file_path);
    lego_log_file.read(arena_landmarks);

    std::ofstream landmarks_file("find_wall_pairs.txt");
    landmarks_file << std::setprecision(3) << std::fixed;

    LegoRobotModel lego_robot_model(TICKS_TO_MM, WHEEL_BASE, SCANNER_DISPLACEMENT);

    LegoRobotScanHandler lego_robot_scan_handler(MIN_VALID_SCAN_DISTANCE);
    LandmarkHandler landmark_handler(rejection_radius);
    std::vector<point> scanner_rel_cylinders, world_cylinders, transformed_world_cylinders, subsampled_points, world_points;
    std::vector<std::array<int, 2>> cylinder_pairs;
    float angle, x, y;
    std::vector<float> xs, ys;
    LandmarkTransform trafo;
    for (size_t i = 0; i<lego_log_file.scan_data.size(); i++){
        current_pose = lego_robot_model.scanner_step(current_pose, lego_log_file.rel_motor_ticks[i]);

        subsampled_points = get_subsampled_points(lego_log_file.scan_data[i]);
        world_points = lego_robot_model.convert_scanner_relative_to_world_xy(current_pose, subsampled_points);

        std::array<std::vector<point>, 2> correlated_points = landmark_handler.get_corresponding_points_on_wall(world_points);
        
        std::vector<point> all_points = correlated_points[0];
        all_points.insert(all_points.end(), correlated_points[1].begin(), correlated_points[1].end());
        
        pose_file_format(landmarks_file, current_pose);
        world_cylinders_file_format(landmarks_file, all_points);
    }
}