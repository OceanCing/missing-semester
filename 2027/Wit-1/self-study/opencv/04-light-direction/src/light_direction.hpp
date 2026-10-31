#pragma once

#include "contour_filter.hpp"

#include <cmath>
#include <vector>

struct DirectionInfo {
  ContourInfo shape;
  cv::Point2f long_axis;
  double tilt_degrees = 0;
  bool accepted = false;
};

inline std::vector<DirectionInfo> analyzeDirections(const cv::Mat& mask,
                                                   double max_tilt_degrees,
                                                   int min_area = 200,
                                                   double min_ratio = 3.0) {
  const auto shapes = analyzeContours(mask, min_area, min_ratio);
  std::vector<DirectionInfo> result;
  for (const auto& shape : shapes) {
    DirectionInfo info;
    info.shape = shape;
    cv::Point2f corners[4];
    shape.rectangle.points(corners);
    double longest_squared = 0;
    for (int index = 0; index < 4; ++index) {
      const auto edge = corners[(index + 1) % 4] - corners[index];
      const double squared = edge.dot(edge);
      if (squared > longest_squared) {
        longest_squared = squared;
        info.long_axis = edge;
      }
    }
    if (shape.accepted && longest_squared > 0) {
      info.long_axis *= static_cast<float>(1.0 / std::sqrt(longest_squared));
      // Use the acute angle from vertical, independent of the edge's sign.
      const double dx = std::abs(static_cast<double>(info.long_axis.x));
      const double dy = std::abs(static_cast<double>(info.long_axis.y));
      info.tilt_degrees = std::atan2(dx, dy) * 180.0 / CV_PI;
      info.accepted = info.tilt_degrees <= max_tilt_degrees;
    }
    result.push_back(info);
  }
  return result;
}
