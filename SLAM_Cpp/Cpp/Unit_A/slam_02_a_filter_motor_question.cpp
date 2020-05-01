
#include "lego_robot_log_file.h"
#include "lego_robot_model.h"
#include "constants.h"
#include <fstream>
#include <string>
#include <math.h>
#include <array>
#include <iostream>

#define DATA_PATH "../../Data/Unit_A/"
#define MOTOR_DATA_FN "robot4_motors.txt"

int main(){
    char motor_data_file_path[100];
    strcpy(motor_data_file_path, DATA_PATH);
    strcat(motor_data_file_path, MOTOR_DATA_FN);

    std::ofstream output("robot_pose.csv");
    
    LegoRobotLogFile lego_log_file;
    lego_log_file.read(motor_data_file_path);

    LegoRobotModel lego_robot_model(TICKS_TO_MM, WHEEL_BASE, SCANNER_DISPLACEMENT);

    std::array<float, 3> pre_pose = {0.0, 0.0, 0.0};
    std::array<float, 3> post_pose = {0.0, 0.0, 0.0};
    std::vector<std::array<float, 3>> filtered;
    for (size_t i=0; i< lego_log_file.rel_motor_ticks.size(); i++){
        post_pose = lego_robot_model.track_center_step(pre_pose, lego_log_file.rel_motor_ticks[i]);
        filtered.push_back(post_pose);
        output << post_pose[0] << ", " << post_pose[1] << ", "<< post_pose[2] << std::endl;
        pre_pose = post_pose;
    }
}