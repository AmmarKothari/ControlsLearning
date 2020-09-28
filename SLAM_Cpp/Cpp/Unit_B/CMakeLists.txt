cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Unit_B VERSION 1.0)

set (SOURCES slam_04_a_project_landmarks.cpp
             slam_04_b_find_cylinder_pairs_question.cpp
             slam_04_c_estimate_transform_question.cpp
             slam_04_d_apply_transform_question.cpp
             slam_05_a_find_wall_pairs_question.cpp
             )

foreach (sourcefile ${SOURCES})
    string(REPLACE ".cpp" "" exec_name ${sourcefile})
    add_executable(${exec_name} ${sourcefile})
    target_link_libraries(${exec_name} LINK_PUBLIC util)
endforeach()