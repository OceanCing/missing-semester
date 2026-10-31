#include "course.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {
  cv::CommandLineParser parser(argc, argv,
      "{help h ? | | Show help}"
      "{assets | | Generated assets directory}"
      "{output | | Output directory}"
      "{input | | Optional local image/video/board image pattern}"
      "{config | | Detector YAML}"
      "{variant | | Experiment variant}"
      "{value |-1| Slider value for headless experiments}"
      "{frames |180| Maximum video frames}"
      "{square |0.025| Calibration square side in meters}"
      "{headless |false| Save outputs without GUI}");
  if (parser.has("help")) { parser.printMessage(); return 0; }
  lab::Options options;
  options.assets = parser.get<std::string>("assets");
  options.output = parser.get<std::string>("output");
  options.input = parser.get<std::string>("input");
  options.config = parser.get<std::string>("config");
  options.variant = parser.get<std::string>("variant");
  options.value = parser.get<int>("value");
  options.frames = parser.get<int>("frames");
  options.square = parser.get<double>("square");
  options.headless = parser.get<bool>("headless");
  if (!parser.check()) { parser.printErrors(); return 2; }
  if (options.assets.empty() || options.output.empty() || options.frames < 1 ||
      options.value < -1 || !std::isfinite(options.square) || options.square <= 0) {
    std::cerr << "Invalid paths, frames, slider value or square size. Use run.sh.\n";
    return 2;
  }
  try {
    lab::mkdir(options.output);
    std::cout << "OpenCV " << CV_VERSION << std::endl;
    lab::COURSE_ENTRY(options);
    return 0;
  } catch (const std::exception& error) {
    std::cerr << "Lesson failed: " << error.what() << std::endl;
    return 1;
  }
}
