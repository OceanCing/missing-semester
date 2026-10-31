#include "course.hpp"
#include <iostream>
int main(int argc, char** argv) {
  cv::CommandLineParser parser(argc, argv,
      "{assets | | Generated assets directory}{force |false| Regenerate teaching assets}");
  lab::Options options;
  options.assets = parser.get<std::string>("assets");
  options.force = parser.get<bool>("force");
  if (!parser.check() || options.assets.empty()) return 2;
  try { lab::prepare(options); return 0; }
  catch (const std::exception& error) { std::cerr << error.what() << std::endl; return 1; }
}
