#include <fstream>
#include <string>
#include <math.h>
#include <array>
#include <iostream>

#include "lego_robot_log_file.h"
#include "lego_robot_model.h"
#include "constants.h"
#include "spatial_classes.h"
#include "output_formatting.h"

#define DATA_PATH "../../Data/Unit_A/"
#define MOTOR_DATA_FN "robot4_motors.txt"




int main(){
    char motor_data_file_path[100];
    strcpy(motor_data_file_path, DATA_PATH);
    strcat(motor_data_file_path, MOTOR_DATA_FN);

    std::ofstream output("robot_scanner_pose.csv");
    
    LegoRobotLogFile lego_log_file;
    lego_log_file.read(motor_data_file_path);

    LegoRobotModel lego_robot_model(TICKS_TO_MM, WHEEL_BASE, SCANNER_DISPLACEMENT);

    pose pre_pose = {1850.0, 1897.0, 213.0 / 180.0 * M_PI};
    pose post_pose = {0.0, 0.0, 0.0};
    std::vector<pose> filtered;
    for (size_t i=0; i< lego_log_file.rel_motor_ticks.size(); i++){
        post_pose = lego_robot_model.track_center_step(pre_pose, lego_log_file.rel_motor_ticks[i]);
        filtered.push_back(post_pose);
        pose_file_format(output, post_pose);
        pre_pose = post_pose;
    }
}