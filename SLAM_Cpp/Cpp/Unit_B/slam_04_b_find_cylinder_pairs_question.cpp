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

    std::ofstream landmarks_file("find_cylinder_pairs.txt");
    landmarks_file << std::setprecision(3) << std::fixed;

    LegoRobotModel lego_robot_model(TICKS_TO_MM, WHEEL_BASE, SCANNER_DISPLACEMENT);

    LegoRobotScanHandler lego_robot_scan_handler(MIN_VALID_SCAN_DISTANCE);
    LandmarkHandler landmark_handler(rejection_radius);
    std::vector<int> scan;
    std::vector<float> scan_der;
    std::vector<point> scanner_rel_cylinders, world_cylinders, paired_world_cylinders;
    std::vector<std::array<int, 2>> cylinder_pairs;
    std::vector<int> ticks;
    float angle, x, y;
    std::vector<float> xs, ys;
    for (size_t i = 0; i<lego_log_file.scan_data.size(); i++){
        current_pose = lego_robot_model.scanner_step(current_pose, lego_log_file.rel_motor_ticks[i]);

        scanner_rel_cylinders = lego_robot_scan_handler.find_cylinders_relative_xy(lego_log_file.scan_data[i], depth_jump, cylinder_offset);
        world_cylinders = lego_robot_model.convert_scanner_relative_to_world_xy(current_pose, scanner_rel_cylinders);
        cylinder_pairs = landmark_handler.find_cylinder_pairs(world_cylinders, lego_log_file.landmarks);
        pose_file_format(landmarks_file, current_pose);
        relative_cylinders_file_format(landmarks_file, scanner_rel_cylinders);
        paired_world_cylinders.clear();
        for (auto pair: cylinder_pairs){
            paired_world_cylinders.push_back(lego_log_file.landmarks[pair[1]]);
        }
        world_cylinders_file_format(landmarks_file, paired_world_cylinders);
    }
}