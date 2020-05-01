#include <iostream>
#include "lego_robot_log_file.h"
#include <string>
#include <fstream>

#define DATA_PATH "../../Data/Unit_A/"
#define MOTOR_DATA_FN "robot4_motors.txt"



int main(){
    char motor_data_file_path[100];
    strcpy(motor_data_file_path, DATA_PATH);
    strcat(motor_data_file_path, MOTOR_DATA_FN);

    std::ofstream output("relative_motor_ticks.csv");
    
    LegoRobotLogFile lego_robot_log_file;
    lego_robot_log_file.read(motor_data_file_path);
    for (size_t i; i< lego_robot_log_file.rel_motor_ticks.size(); i++){
        output << lego_robot_log_file.rel_motor_ticks[i][0] << ", " << lego_robot_log_file.rel_motor_ticks[i][1] << std::endl;
    }
}