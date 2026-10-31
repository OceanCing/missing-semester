#pragma once

#include <opencv2/opencv.hpp>
#include <utility>
#include <vector>

struct HsvSettings {
  int hue_center = 0;
  int hue_radius = 10;
  int saturation_min = 100;
  int value_min = 50;
};

// Hue is circular: red near 0 and red near 179 are neighbors.
// Each pair is an inclusive interval, matching cv::inRange's boundaries.
inline std::vector<std::pair<int, int>> hueRanges(const HsvSettings& settings) {
  const int low = settings.hue_center - settings.hue_radius;
  const int high = settings.hue_center + settings.hue_radius;
  if (low < 0) return {{0, high}, {180 + low, 179}};
  if (high >= 180) return {{0, high - 180}, {low, 179}};
  return {{low, high}};
}

inline cv::Mat selectHsv(const cv::Mat& hsv, const HsvSettings& settings) {
  cv::Mat mask = cv::Mat::zeros(hsv.size(), CV_8UC1);
  for (const auto& range : hueRanges(settings)) {
    cv::Mat part;
    cv::inRange(hsv,
                cv::Scalar(range.first, settings.saturation_min, settings.value_min),
                cv::Scalar(range.second, 255, 255), part);
    cv::bitwise_or(mask, part, mask);
  }
  return mask;
}
