cmake_minimum_required(VERSION 3.8)
project(contour_lesson LANGUAGES CXX)

find_package(OpenCV 4 REQUIRED COMPONENTS core imgcodecs imgproc highgui)
add_executable(image_contours src/main.cpp)
target_compile_features(image_contours PRIVATE cxx_std_14)
target_compile_options(image_contours PRIVATE -Wall -Wextra)
target_include_directories(image_contours PRIVATE ${OpenCV_INCLUDE_DIRS}
  "${CMAKE_CURRENT_SOURCE_DIR}/../02-hsv/src")
target_link_libraries(image_contours ${OpenCV_LIBS})
