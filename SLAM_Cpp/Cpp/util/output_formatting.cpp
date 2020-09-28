#include <string>
#include <iostream>
#include <ostream>

#include "output_formatting.h"
#include "spatial_classes.h"

std::string pose_to_txt(pose &p){
    char buff[100];
    std::snprintf(buff, sizeof(buff), "%.3f %.3f %.3f", p.x, p.y, p.theta);
    std::string out = buff;
    return out;
}

std::string point_to_txt(point &p){
    char buff[100];
    std::snprintf(buff, sizeof(buff), "%.3f %.3f ", p.x, p.y);
    std::string out = buff;
    return out;
}

void pose_file_format(std::ostream& out_file, pose p){
    out_file << "F " << pose_to_txt(p) << std::endl;
}

void points_file_format(std::ostream &out_file, std::vector<point> ps){
    for (auto p: ps){
        out_file << point_to_txt(p);
    }
}

void relative_cylinders_file_format(std::ostream &out_file, std::vector<point> ps){
    out_file << "D C ";
    points_file_format(out_file, ps);
    out_file << std::endl;
}

void world_cylinders_file_format(std::ostream &out_file, std::vector<point> ps){
    out_file << "W C ";
    points_file_format(out_file, ps);
    out_file << std::endl;
}