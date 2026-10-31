cmake_minimum_required(VERSION 3.8)
project(turtle_control)

find_package(catkin REQUIRED COMPONENTS geometry_msgs roscpp turtlesim tf2 tf2_ros tf2_geometry_msgs)
catkin_package(CATKIN_DEPENDS geometry_msgs roscpp turtlesim tf2 tf2_ros tf2_geometry_msgs)

add_executable(turtle_motion src/turtle_motion.cpp)
target_compile_features(turtle_motion PRIVATE cxx_std_14)
target_compile_options(turtle_motion PRIVATE -Wall -Wextra)
target_include_directories(turtle_motion PRIVATE ${catkin_INCLUDE_DIRS})
target_link_libraries(turtle_motion ${catkin_LIBRARIES})

add_executable(turtle_pose_listener src/turtle_pose_listener.cpp)
target_compile_features(turtle_pose_listener PRIVATE cxx_std_14)
target_compile_options(turtle_pose_listener PRIVATE -Wall -Wextra)
target_include_directories(turtle_pose_listener PRIVATE ${catkin_INCLUDE_DIRS})
target_link_libraries(turtle_pose_listener ${catkin_LIBRARIES})

add_executable(turtle_go_to_x src/turtle_go_to_x.cpp)
target_compile_features(turtle_go_to_x PRIVATE cxx_std_14)
target_compile_options(turtle_go_to_x PRIVATE -Wall -Wextra)
target_include_directories(turtle_go_to_x PRIVATE ${catkin_INCLUDE_DIRS})
target_link_libraries(turtle_go_to_x ${catkin_LIBRARIES})

add_executable(point_to_world src/point_to_world.cpp)
target_compile_features(point_to_world PRIVATE cxx_std_14)
target_compile_options(point_to_world PRIVATE -Wall -Wextra)
target_include_directories(point_to_world PRIVATE ${catkin_INCLUDE_DIRS})
target_link_libraries(point_to_world ${catkin_LIBRARIES})

add_executable(turtle_tf_broadcaster src/turtle_tf_broadcaster.cpp)
target_compile_features(turtle_tf_broadcaster PRIVATE cxx_std_14)
target_compile_options(turtle_tf_broadcaster PRIVATE -Wall -Wextra)
target_include_directories(turtle_tf_broadcaster PRIVATE ${catkin_INCLUDE_DIRS})
target_link_libraries(turtle_tf_broadcaster ${catkin_LIBRARIES})

install(TARGETS turtle_motion turtle_pose_listener turtle_go_to_x point_to_world turtle_tf_broadcaster
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch)
install(DIRECTORY rviz/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/rviz)
install(DIRECTORY urdf/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/urdf)
install(DIRECTORY config/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/config)
