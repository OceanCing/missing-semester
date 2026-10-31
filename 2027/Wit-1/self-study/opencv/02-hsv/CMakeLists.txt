cmake_minimum_required(VERSION 3.8)
project(hsv_lesson LANGUAGES CXX)

find_package(OpenCV 4 REQUIRED COMPONENTS core imgcodecs imgproc highgui)
add_executable(image_hsv src/main.cpp)
target_compile_features(image_hsv PRIVATE cxx_std_14)
target_compile_options(image_hsv PRIVATE -Wall -Wextra)
target_include_directories(image_hsv PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(image_hsv ${OpenCV_LIBS})
