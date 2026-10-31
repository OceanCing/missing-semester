#include "course.hpp"
#include <cmath>
#include <stdexcept>

namespace lab {
DetectorConfig loadConfig(const std::string& path) {
  DetectorConfig config;
  if (path.empty()) return config;
  cv::FileStorage file(path, cv::FileStorage::READ);
  if (!file.isOpened()) throw std::runtime_error("Cannot open detector config: " + path);
  auto read = [&](const char* key, auto& value) { if (!file[key].empty()) file[key] >> value; };
  read("hue_center", config.hsv.hue_center); read("hue_radius", config.hsv.hue_radius);
  read("saturation_min", config.hsv.saturation_min); read("value_min", config.hsv.value_min);
  read("min_area", config.min_area); read("min_ratio", config.min_ratio);
  read("max_tilt", config.max_tilt); read("max_difference", config.max_difference);
  read("min_length_ratio", config.pairing.min_length_ratio);
  read("max_y_offset_ratio", config.pairing.max_y_offset_ratio);
  read("min_separation_ratio", config.pairing.min_separation_ratio);
  read("max_separation_ratio", config.pairing.max_separation_ratio);
  const auto finiteRange = [](double x, double low, double high) {
    return std::isfinite(x) && x >= low && x <= high;
  };
  if (!finiteRange(config.hsv.hue_center, 0, 179) || !finiteRange(config.hsv.hue_radius, 0, 89) ||
      !finiteRange(config.hsv.saturation_min, 0, 255) || !finiteRange(config.hsv.value_min, 0, 255) ||
      config.min_area < 0 || !finiteRange(config.min_ratio, 1, 100) ||
      !finiteRange(config.max_tilt, 0, 90) || !finiteRange(config.max_difference, 0, 90) ||
      !finiteRange(config.pairing.min_length_ratio, 0, 1) ||
      !finiteRange(config.pairing.max_y_offset_ratio, 0, 10) ||
      !finiteRange(config.pairing.min_separation_ratio, 0, 100) ||
      !finiteRange(config.pairing.max_separation_ratio, config.pairing.min_separation_ratio, 100))
    throw std::runtime_error("Invalid detector config range");
  return config;
}
Detection detect(const cv::Mat& image, const DetectorConfig& config) {
  Detection result;
  cv::Mat hsv;
  cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
  result.mask = selectHsv(hsv, config.hsv);
  result.lights = analyzeDirections(result.mask, config.max_tilt, config.min_area, config.min_ratio);
  result.pairs = pairLights(result.lights, config.max_difference, config.pairing);
  result.annotated = image.clone();
  for (const auto& light : result.lights) {
    if (!light.accepted) continue;
    cv::Point2f points[4]; light.shape.rectangle.points(points);
    for (int edge = 0; edge < 4; ++edge)
      cv::line(result.annotated, points[edge], points[(edge + 1) % 4], {0, 255, 0}, 1);
  }
  for (const auto& pair : result.pairs) {
    if (!pair.accepted) continue;
    const auto& left = result.lights[pair.left]; const auto& right = result.lights[pair.right];
    const auto lc = left.shape.rectangle.center, rc = right.shape.rectangle.center;
    const auto la = upwardAxis(left) * static_cast<float>(left.shape.long_side / 2);
    const auto ra = upwardAxis(right) * static_cast<float>(right.shape.long_side / 2);
    const std::vector<cv::Point2f> points{lc + la, rc + ra, rc - ra, lc - la};
    const auto center = (lc + rc) * 0.5f;
    result.corners.push_back(points); result.centers.push_back(center);
    for (int edge = 0; edge < 4; ++edge)
      cv::line(result.annotated, points[edge], points[(edge + 1) % 4], {255, 220, 0}, 2);
    cv::circle(result.annotated, center, 3, {255, 220, 0}, -1);
  }
  return result;
}
int countLights(const Detection& detection) {
  return static_cast<int>(std::count_if(detection.lights.begin(), detection.lights.end(),
      [](const DirectionInfo& light) { return light.accepted; }));
}
int countPairs(const Detection& detection) { return static_cast<int>(detection.corners.size()); }
}  // namespace lab
