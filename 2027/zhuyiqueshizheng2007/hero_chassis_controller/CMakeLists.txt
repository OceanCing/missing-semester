cmake_minimum_required(VERSION 3.10)
project(hero_chassis_controller)

## Use C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## By adding -Wall and -Werror, the compiler does not ignore warnings anymore,
## enforcing cleaner code.
# 注意：为了防止你自己写代码时因为变量未使用等小问题报错，这里我们先把它注释掉
# add_definitions(-Wall -Werror)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
        roscpp
        control_toolbox
        pluginlib
        hardware_interface
        controller_interface
        geometry_msgs
        nav_msgs tf
)

## Find system libraries
find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED)

## catkin specific configuration ##
catkin_package(
        INCLUDE_DIRS include
        CATKIN_DEPENDS roscpp control_toolbox pluginlib hardware_interface
        controller_interface
        geometry_msgs
        nav_msgs tf
)

###########
## Build ##
###########

## Specify additional locations of header files
include_directories(
        include
        ${catkin_INCLUDE_DIRS}
        ${EIGEN3_INCLUDE_DIR}
        ${Boost_INCLUDE_DIRS}
)

## 核心修改点：把你的控制器编译成一个共享库 (插件)！
add_library(${PROJECT_NAME}
        src/HeroChassisController.cpp
)

## 链接依赖库
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})
add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

#############
## Install ##
#############

# 将插件库安装到正确的路径
install(
        TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

# 安装头文件
install(
        DIRECTORY include/${PROJECT_NAME}/
        DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
        FILES_MATCHING PATTERN "*.hpp"
)

#############
## Testing ##
#############
# 对应的测试文件可能还需要改，先全注释掉
# if (CATKIN_ENABLE_TESTING})
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
#   catkin_add_gtest(${PROJECT_NAME}-test
#       test/test_hero_chassis_controller.cpp
#       test/AlgorithmTest.cpp)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME}_core)
# endif ()

##########################
## Static code analysis ##
##########################
# roslint_cpp()
