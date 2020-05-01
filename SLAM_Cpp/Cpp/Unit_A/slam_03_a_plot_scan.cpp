#include "lego_robot_log_file.h"
#include "lego_robot_model.h"
#include "constants.h"
#include <fstream>
#include <string>
#include <math.h>
#include <array>
#include <iostream>
#include "matplotlibcpp.h"

#define DATA_PATH "../../Data/Unit_A/"
#define SCAN_DATA_FN "robot4_scan.txt"




int main(){
    char scan_data_file_path[100];
    strcpy(scan_data_file_path, DATA_PATH);
    strcat(scan_data_file_path, SCAN_DATA_FN);

    std::ofstream output("robot_scan_data.csv");
    
    LegoRobotLogFile lego_log_file;
    std::cout << "Scans: " << lego_log_file.scan_data.size() << std::endl;
    lego_log_file.read(scan_data_file_path);
    for (auto v: lego_log_file.scan_data[0]){
        output << v << ", ";
    }
    output << std::endl;
    matplotlibcpp::plot(lego_log_file.scan_data[0], "b-");
    matplotlibcpp::xlabel("Scan Measurement Index");
    matplotlibcpp::ylabel("Distance (scan units)");
    matplotlibcpp::title("Single Scan from Lidar");
    matplotlibcpp::show(true);

}