cmake_minimum_required(VERSION 3.8)
project(light_direction_lesson LANGUAGES CXX)

find_package(OpenCV 4 REQUIRED COMPONENTS core imgcodecs imgproc highgui)
add_executable(light_direction src/main.cpp)
target_compile_features(light_direction PRIVATE cxx_std_14)
target_compile_options(light_direction PRIVATE -Wall -Wextra)
target_include_directories(light_direction PRIVATE ${OpenCV_INCLUDE_DIRS}
  "${CMAKE_CURRENT_SOURCE_DIR}/../02-hsv/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/../03-contours/src")
target_link_libraries(light_direction ${OpenCV_LIBS})
