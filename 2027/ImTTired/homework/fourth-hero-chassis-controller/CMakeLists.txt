
cmake_minimum_required(VERSION 3.10)
project(hero_chassis_controller)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

find_package(catkin REQUIRED COMPONENTS
        roscpp
        controller_interface
        hardware_interface
        pluginlib
        control_toolbox
        geometry_msgs
        realtime_tools
        nav_msgs
        tf2_ros
        tf
)

catkin_package(
        INCLUDE_DIRS include
        LIBRARIES ${PROJECT_NAME}
        CATKIN_DEPENDS
        roscpp
        controller_interface
        hardware_interface
        pluginlib
        control_toolbox
        geometry_msgs
        realtime_tools
        nav_msgs
        tf2_ros
        tf
)

include_directories(
        include
        ${catkin_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}
        src/hero_chassis_controller.cpp
)

target_link_libraries(${PROJECT_NAME}
        ${catkin_LIBRARIES}
)
