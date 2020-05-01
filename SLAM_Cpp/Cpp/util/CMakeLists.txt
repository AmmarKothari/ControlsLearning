cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(util VERSION 1.0)

find_package(Python3 COMPONENTS Development NumPy)

add_library(util STATIC lego_robot_log_file.cpp lego_robot_model.cpp lego_robot_scan_handler.cpp constants.h ../matplotlibcpp.h)

target_include_directories(util PUBLIC $(CMAKE_CURRENT_SOURCE_DIR) PUBLIC .. PRIVATE ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})

target_link_libraries(util Python3::Python Python3::NumPy)