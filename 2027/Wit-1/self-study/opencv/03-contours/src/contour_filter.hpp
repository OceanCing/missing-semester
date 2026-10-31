#pragma once

#include <opencv2/opencv.hpp>
#include <algorithm>
#include <vector>

struct ContourInfo {
  std::vector<cv::Point> points;
  cv::RotatedRect rectangle;
  double area = 0;
  double short_side = 0;
  double long_side = 0;
  double ratio = 0;
  bool accepted = false;
};

inline std::vector<ContourInfo> analyzeContours(const cv::Mat& mask,
                                               int min_area, double min_ratio) {
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  // Stable left-to-right IDs make the example's labels easier to follow.
  std::sort(contours.begin(), contours.end(), [](const std::vector<cv::Point>& a,
                                                const std::vector<cv::Point>& b) {
    const auto ra = cv::boundingRect(a);
    const auto rb = cv::boundingRect(b);
    return ra.x == rb.x ? ra.y < rb.y : ra.x < rb.x;
  });
  std::vector<ContourInfo> result;
  for (const auto& points : contours) {
    ContourInfo info;
    info.points = points;
    info.area = cv::contourArea(points);
    info.rectangle = cv::minAreaRect(points);
    info.short_side = std::min(info.rectangle.size.width, info.rectangle.size.height);
    info.long_side = std::max(info.rectangle.size.width, info.rectangle.size.height);
    if (info.short_side > 0) info.ratio = info.long_side / info.short_side;
    info.accepted = info.short_side > 0 && info.area >= min_area && info.ratio >= min_ratio;
    result.push_back(info);
  }
  return result;
}
