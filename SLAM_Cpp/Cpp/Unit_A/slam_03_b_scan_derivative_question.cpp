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
    char scan_data_file_path[100];
    strcpy(scan_data_file_path, DATA_PATH);
    strcat(scan_data_file_path, SCAN_DATA_FN);
    
    LegoRobotLogFile lego_log_file;
    lego_log_file.read(scan_data_file_path);
    std::vector<int> scan = lego_log_file.scan_data[7];
    LegoRobotScanHandler lego_robot_scan_handler(MIN_VALID_SCAN_DISTANCE);
    std::vector<float> scan_der = lego_robot_scan_handler.scan_der(scan);
    std::cout << "scan_der size: " << scan_der.size() << std::endl;
    matplotlibcpp::plot(scan_der, "b-");
    matplotlibcpp::plot(scan, "r-");
    matplotlibcpp::show();
}