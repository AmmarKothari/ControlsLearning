#include "matplotlibcpp.h"
#include "lego_robot_log_file.h"
#include "lego_robot_model.h"
#include "constants.h"
#include <fstream>
#include <string>
#include <math.h>
#include <array>
#include <iostream>
#include "lego_robot_scan_handler.h"

#define DATA_PATH "../../Data/Unit_A/"
#define SCAN_DATA_FN "robot4_scan.txt"

int main(){
    float depth_jump = 100.0;

    char scan_data_file_path[100];
    strcpy(scan_data_file_path, DATA_PATH);
    strcat(scan_data_file_path, SCAN_DATA_FN);
    
    LegoRobotLogFile lego_log_file;
    lego_log_file.read(scan_data_file_path);
    std::vector<int> scan = lego_log_file.scan_data[8];
    LegoRobotScanHandler lego_robot_scan_handler(MIN_VALID_SCAN_DISTANCE);
    std::vector<float> scan_der = lego_robot_scan_handler.scan_der(scan);
    std::vector<std::array<float, 2>> cylinders = lego_robot_scan_handler.find_cylinders(scan, scan_der, depth_jump);
    std::cout << "scan_der size: " << scan_der.size() << std::endl;
    matplotlibcpp::plot(scan, "r-");
    std::vector<float> ray_positions, depth_positions;
    for (auto c: cylinders){
        ray_positions.push_back(c[0]);
        depth_positions.push_back(c[1]);
    }
    matplotlibcpp::plot(ray_positions, depth_positions, "bs");
    matplotlibcpp::show();
}