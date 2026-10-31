#pragma once

#include "light_direction.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <vector>

struct PairInfo {
  std::size_t left = 0;
  std::size_t right = 0;
  double length_ratio = 0;
  double y_offset_ratio = 0;
  double separation_ratio = 0;
  double angle_difference = 0;
  bool geometry_passed = false;
  bool accepted = false;
};

struct PairSettings {
  double min_length_ratio = 0.7;
  double max_y_offset_ratio = 0.3;
  double min_separation_ratio = 0.5;
  double max_separation_ratio = 1.5;
};

inline cv::Point2f upwardAxis(const DirectionInfo& light) {
  auto axis = light.long_axis;
  if (axis.y > 0 || (axis.y == 0 && axis.x < 0)) axis *= -1.0f;
  return axis;
}

inline double signedTilt(const DirectionInfo& light) {
  const auto axis = upwardAxis(light);
  return std::atan2(static_cast<double>(axis.x),
                    -static_cast<double>(axis.y)) * 180.0 / CV_PI;
}

inline double axisDifference(const DirectionInfo& first, const DirectionInfo& second) {
  const auto a = upwardAxis(first);
  const auto b = upwardAxis(second);
  const double cross = static_cast<double>(a.x) * b.y - static_cast<double>(a.y) * b.x;
  const double dot = static_cast<double>(a.x) * b.x + static_cast<double>(a.y) * b.y;
  // Undirected axes: opposite vectors represent the same line orientation.
  return std::atan2(std::abs(cross), std::abs(dot)) * 180.0 / CV_PI;
}

inline std::vector<PairInfo> pairLights(const std::vector<DirectionInfo>& lights,
                                       double max_angle_difference,
                                       const PairSettings& settings = PairSettings{}) {
  std::vector<PairInfo> result;
  for (std::size_t first = 0; first < lights.size(); ++first) {
    if (!lights[first].accepted) continue;
    for (std::size_t second = first + 1; second < lights.size(); ++second) {
      if (!lights[second].accepted) continue;
      PairInfo info;
      info.left = first;
      info.right = second;
      if (lights[info.left].shape.rectangle.center.x >
          lights[info.right].shape.rectangle.center.x) std::swap(info.left, info.right);
      const auto& left = lights[info.left];
      const auto& right = lights[info.right];
      const double left_length = left.shape.long_side;
      const double right_length = right.shape.long_side;
      const double average = (left_length + right_length) / 2.0;
      const auto delta = right.shape.rectangle.center - left.shape.rectangle.center;
      info.length_ratio = std::min(left_length, right_length) /
                          std::max(left_length, right_length);
      info.y_offset_ratio = std::abs(static_cast<double>(delta.y)) / average;
      info.separation_ratio = std::abs(static_cast<double>(delta.x)) / average;
      info.angle_difference = axisDifference(left, right);
      info.geometry_passed = info.length_ratio >= settings.min_length_ratio &&
                             info.y_offset_ratio <= settings.max_y_offset_ratio &&
                             info.separation_ratio >= settings.min_separation_ratio &&
                             info.separation_ratio <= settings.max_separation_ratio;
      info.accepted = info.geometry_passed && info.angle_difference <= max_angle_difference;
      result.push_back(info);
    }
  }
  return result;
}
