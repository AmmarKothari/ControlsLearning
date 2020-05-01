cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Unit_A VERSION 1.0)

set (SOURCES slam_01_a_plot_motor_ticks.cpp
            slam_01_b_print_motor_increments.cpp 
            slam_02_a_filter_motor_question.cpp 
            slam_02_b_filter_motor_question.cpp 
            slam_03_a_plot_scan.cpp
            slam_03_b_scan_derivative_question.cpp
            slam_03_c_find_cylinders_question.cpp
            slam_03_d_find_cylinders_cartesian_question.cpp)

foreach (sourcefile ${SOURCES})
    string(REPLACE ".cpp" "" exec_name ${sourcefile})
    add_executable(${exec_name} ${sourcefile})
    target_link_libraries(${exec_name} LINK_PUBLIC util)
endforeach()