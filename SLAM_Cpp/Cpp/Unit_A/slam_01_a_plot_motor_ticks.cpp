#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define DATA_PATH "../../../Data/Unit_A/"
#define MOTOR_DATA_FN "robot4_motors.txt"

int main(){
    char motor_data_file_path[100];
    strcpy(motor_data_file_path, DATA_PATH);
    strcat(motor_data_file_path, MOTOR_DATA_FN);
    std::cout << motor_data_file_path << std::endl;
    std::ifstream robot_data(motor_data_file_path);
    std::ofstream tick_data("motor_ticks.csv");
    std::string line, delimiter, token;
    std::vector<std::string> split_line;
    delimiter = " ";
    std::size_t start_pos;
    if (robot_data.is_open()){
        while (std::getline(robot_data, line)){
            start_pos = 0;
            split_line.clear();
            start_pos = 0;
            while ( (start_pos=line.find(delimiter)) != std::string::npos){
                token = line.substr(0, line.find(delimiter));
                split_line.push_back(token);
                line.erase(0, line.find(delimiter) + delimiter.length());
            }
            tick_data << split_line[2] << ", " << split_line[6] << std::endl;
        }
    }
    else{
        std::cout << "Could not open file" << std::endl;
    }
}