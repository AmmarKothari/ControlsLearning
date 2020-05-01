
#include "lego_robot_scan_handler.h"
#include <vector>
#include <iostream>
#include <array>

#define MAX_SCAN_ANGLE 330.0
#define ANGLE_PER_INDEX 0.006135923151543

LegoRobotScanHandler::LegoRobotScanHandler(){
    min_distance = 0.0;
}

LegoRobotScanHandler::LegoRobotScanHandler(float in_min_distance){
    min_distance = in_min_distance;
}

std::vector<float> LegoRobotScanHandler::scan_der(std::vector<int> scan){
    std::vector<float> scan_der;
    float prev_val, next_val;
    for (std::vector<int>::iterator it = scan.begin() + 1; it != scan.end() - 1; it ++){
        prev_val = *(it-1);
        next_val = *(it+1);
        if (prev_val > min_distance and next_val > min_distance){
            scan_der.push_back((float)(next_val - prev_val) / 2.0);
        }
        else{
            // Could we eliminate the value from the scan data once we detect it as being erroneous?
            scan_der.push_back(0.0);
        }
    }
    // Want the output to have the same length as the input so add a 0 to start and end
    scan_der.insert(scan_der.begin(), 0.0);
    scan_der.push_back(0.0);
    return scan_der;
}

std::vector<std::array<float, 2>> LegoRobotScanHandler::find_cylinders(std::vector<int> scan, std::vector<float> der, float depth_jump){
    std::vector<std::array<float, 2>> cylinder_vec;
    std::array<float, 2> current_cylinder;
    bool on_cylinder = false;
    float sum_ray = 0, sum_depth = 0, rays = 0;
    for (size_t i=0; i!=der.size(); i++){
        // if the depth jump is sufficiently large, then consider an object to be seen.
        if (der[i] < -depth_jump){
            on_cylinder = true;
            sum_ray = 0;
            sum_depth = 0;
            rays = 0;
        }
        if (der[i] > depth_jump){
            // If not on cylinder and see another indicator ending the end of the cylinder then update the last cylinder measurements.
            if (!on_cylinder and cylinder_vec.size() > 0){
                cylinder_vec.pop_back();
            }
            current_cylinder[0] = sum_ray / rays;
            current_cylinder[1] = sum_depth / rays;
            cylinder_vec.push_back(current_cylinder);
            on_cylinder = false;

        }
        
        // Zero indicates its an eroneous measurement
        if (scan[i] >= min_distance){
            sum_ray += i;
            rays ++;
            sum_depth += scan[i];
        }
    }
    
    return cylinder_vec;
}

float beam_index_to_angle(float i, float mounting_angle){
    /* Convert a beam index to an angle, in radians. */
    return (i - MAX_SCAN_ANGLE) * ANGLE_PER_INDEX + mounting_angle;
}
