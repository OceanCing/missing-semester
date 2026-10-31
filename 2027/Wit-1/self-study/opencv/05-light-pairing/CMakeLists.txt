cmake_minimum_required(VERSION 3.8)
project(light_pairing_lesson LANGUAGES CXX)

find_package(OpenCV 4 REQUIRED COMPONENTS core imgcodecs imgproc highgui)
add_executable(light_pairing src/main.cpp)
target_compile_features(light_pairing PRIVATE cxx_std_14)
target_compile_options(light_pairing PRIVATE -Wall -Wextra)
target_include_directories(light_pairing PRIVATE ${OpenCV_INCLUDE_DIRS}
  "${CMAKE_CURRENT_SOURCE_DIR}/../02-hsv/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/../03-contours/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/../04-light-direction/src")
target_link_libraries(light_pairing ${OpenCV_LIBS})
