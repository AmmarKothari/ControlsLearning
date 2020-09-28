#include "lego_robot_log_file.h"
#include <iostream>
#include <fstream>
#include <string>

std::vector<std::string> split_line(std::string line, std::string delimiter=" "){
    std::string token;
    std::vector<std::string> split_line;
    int start_pos = 0;
    while ( (start_pos=line.find(delimiter)) != std::string::npos){
        token = line.substr(0, line.find(delimiter));
        split_line.push_back(token);
        line.erase(0, line.find(delimiter) + delimiter.length());
    }
    return split_line;
}

std::vector<int> get_ticks_from_split_line(std::vector<std::string> split){
    std::vector<int> tick;
    tick.push_back(std::stoi(split[2]));
    tick.push_back(std::stoi(split[6]));
    return tick;
}

std::vector<int> get_scan_from_split_line(std::vector<std::string> split){
    std::vector<int> scan;
    // First value is "S", Second is timestep, third is count, 4th is first scan element
    for (std::vector<std::string>::iterator it = split.begin() + 3; it != split.end(); it++){
        scan.push_back(std::stoi(*it));
    }
    return scan;
}

point get_landmark_from_split_line(std::vector<std::string> split){
    point landmark;
    // First value is "C" for cylinder but just ignoring that for now
    landmark = {std::stof(split[2]), std::stof(split[3])};
    return landmark;
}


void LegoRobotLogFile::read(std::string filename){
    std::cout << "Loading file: " << filename << std::endl;
    std::ifstream data_file(filename);
    std::string line;
    std::vector<std::string> split;

    bool first_ticks = true, first_scan_data = true, first_landmarks = true;
    std::vector<std::vector<int>> ticks;
    std::vector<int> tick, single_scan;
    point landmark;
    if (data_file.is_open()){
        int lines_read = 0;
        while (getline(data_file, line)){
            lines_read++;
            split = split_line(line);
            if (split[0] == "M"){
                if (first_ticks){
                    abs_motor_ticks.clear();
                    first_ticks = false;
                }
                tick = get_ticks_from_split_line(split);
                abs_motor_ticks.push_back(tick);
            }

            if (split[0] == "S"){
                if (first_scan_data){
                    first_scan_data = false;
                    scan_data.clear();
                }
                single_scan = get_scan_from_split_line(split);
                scan_data.push_back(single_scan);
            }

            if (split[0] == "L"){
                if (first_landmarks){
                    first_landmarks = false;
                    landmarks.clear();
                }
                landmark = get_landmark_from_split_line(split);
                landmarks.push_back(landmark);
            }
        }
        std::cout << "Lines read: " << lines_read << std::endl;
        if (scan_data.size() > 0){
            std::cout << "Scans: " << scan_data.size() << std::endl;
        }
        if (landmarks.size() > 0){
            std::cout << "Landmarks: " << landmarks.size() << std::endl;
        }

        // Calculate relative motor ticks
        if (abs_motor_ticks.size() > 0) {
            std::vector<int> previous_tick, rel_tick;
            previous_tick = abs_motor_ticks[0];
            for (auto tick: abs_motor_ticks){
                rel_tick.clear();
                rel_tick.push_back(tick[0] - previous_tick[0]);
                rel_tick.push_back(tick[1] - previous_tick[1]);
                rel_motor_ticks.push_back(rel_tick);
                previous_tick = tick;
            }
        }
        std::cout << "Finished reading file" << std::endl;
    }
    else{
        std::cout << "Could not open data file: " << filename << std::endl;
    }

}