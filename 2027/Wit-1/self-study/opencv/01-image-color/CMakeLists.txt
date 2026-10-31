cmake_minimum_required(VERSION 3.8)
project(image_color_lesson LANGUAGES CXX)

find_package(OpenCV 4 REQUIRED COMPONENTS core imgcodecs imgproc highgui)

add_executable(image_color src/image_color.cpp)
target_compile_features(image_color PRIVATE cxx_std_14)
target_compile_options(image_color PRIVATE -Wall -Wextra)
target_include_directories(image_color PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(image_color ${OpenCV_LIBS})
