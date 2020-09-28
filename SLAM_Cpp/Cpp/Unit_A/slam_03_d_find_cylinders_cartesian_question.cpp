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
#include "output_formatting.h"
#include "spatial_classes.h"

#define DATA_PATH "../../Data/Unit_A/"
#define SCAN_DATA_FN "robot4_scan.txt"

int main(){
    float depth_jump = 100.0, cylinder_offset = 90.0;

    char scan_data_file_path[100];
    strcpy(scan_data_file_path, DATA_PATH);
    strcat(scan_data_file_path, SCAN_DATA_FN);
    
    LegoRobotLogFile lego_log_file;
    lego_log_file.read(scan_data_file_path);

    std::ofstream all_cylinders("cylinders.txt");
    all_cylinders << std::setprecision(3) << std::fixed;

    LegoRobotModel lego_robot_model(TICKS_TO_MM, WHEEL_BASE, SCANNER_DISPLACEMENT);

    LegoRobotScanHandler lego_robot_scan_handler(MIN_VALID_SCAN_DISTANCE);
    std::vector<int> scan;
    std::vector<float> scan_der;
    std::vector<point> cylinders;
    point cylinders_xy;
    float angle, x, y;
    for (auto scan: lego_log_file.scan_data){
        scan_der = lego_robot_scan_handler.scan_der(scan);
        cylinders = lego_robot_scan_handler.find_cylinders_relative_xy(scan, scan_der, depth_jump, cylinder_offset);
        relative_cylinders_file_format(all_cylinders, cylinders);
    }
}