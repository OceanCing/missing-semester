cmake_minimum_required(VERSION 3.10)
project(hero_chassis_controller)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic)
find_package(catkin REQUIRED COMPONENTS controller_interface control_toolbox control_msgs
  dynamic_reconfigure geometry_msgs hardware_interface nav_msgs pluginlib realtime_tools roscpp tf tf2_msgs)
generate_dynamic_reconfigure_options(cfg/Chassis.cfg)
catkin_package(INCLUDE_DIRS include LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS controller_interface control_toolbox control_msgs dynamic_reconfigure geometry_msgs
  hardware_interface nav_msgs pluginlib realtime_tools roscpp tf tf2_msgs)
include_directories(include ${catkin_INCLUDE_DIRS})
add_library(${PROJECT_NAME} src/chassis_controller.cpp)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_gencfg ${catkin_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
install(DIRECTORY include/${PROJECT_NAME}/ DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})
install(DIRECTORY config launch urdf docs DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
install(FILES hero_chassis_controller_plugins.xml README.md LICENSE
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
if(CATKIN_ENABLE_TESTING)
  catkin_add_gtest(test_kinematics test/test_kinematics.cpp)
  if(TARGET test_kinematics)
    target_link_libraries(test_kinematics ${catkin_LIBRARIES})
  endif()
endif()
