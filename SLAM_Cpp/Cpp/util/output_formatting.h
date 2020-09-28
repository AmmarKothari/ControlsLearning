#pragma once

#include <vector>
#include <ostream>

#include "spatial_classes.h"

std::string pose_to_txt(pose &p);

std::string point_to_txt(point &p);

void pose_file_format(std::ostream& out_file, pose p);

void points_file_format(std::ostream &out_file, std::vector<point> ps);

void relative_cylinders_file_format(std::ostream &out_file, std::vector<point> ps);

void world_cylinders_file_format(std::ostream &out_file, std::vector<point> ps);
