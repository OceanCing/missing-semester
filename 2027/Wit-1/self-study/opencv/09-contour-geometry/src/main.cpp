#include "course.hpp"
namespace lab {
void runGeometry(const Options& options) {
  cv::Mat mask = cv::Mat::zeros(480, 640, CV_8UC1);
  cv::rectangle(mask, {50, 90, 100, 180}, {255}, -1);
  cv::circle(mask, {320, 160}, 60, {255}, -1);
  cv::circle(mask, {320, 160}, 25, {0}, -1);
  cv::Point2f vertices[4]; cv::RotatedRect({500, 240}, {20, 180}, 25).points(vertices);
  std::vector<cv::Point> polygon;
  for (const auto& vertex : vertices) polygon.emplace_back(cvRound(vertex.x), cvRound(vertex.y));
  cv::fillConvexPoly(mask, polygon, {255});
  std::vector<std::vector<cv::Point>> outer, tree;
  std::vector<cv::Vec4i> hierarchy;
  cv::findContours(mask, outer, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  cv::findContours(mask, tree, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
  cv::Mat edges, shapes, nested;
  cv::Canny(mask, edges, 60, 120);
  cv::cvtColor(mask, shapes, cv::COLOR_GRAY2BGR); nested = shapes.clone();
  for (const auto& contour : outer) {
    cv::rectangle(shapes, cv::boundingRect(contour), {0, 0, 255}, 2);
    cv::Point2f corners[4]; cv::minAreaRect(contour).points(corners);
    for (int edge = 0; edge < 4; ++edge)
      cv::line(shapes, corners[edge], corners[(edge + 1) % 4], {0, 255, 0}, 2);
    const auto moments = cv::moments(contour);
    if (std::abs(moments.m00) > 1e-9)
      cv::circle(shapes, {cvRound(moments.m10 / moments.m00), cvRound(moments.m01 / moments.m00)}, 4, {255, 0, 0}, -1);
  }
  int children = 0;
  for (std::size_t index = 0; index < tree.size(); ++index) {
    const bool child = hierarchy[index][3] >= 0;
    if (child) ++children;
    cv::drawContours(nested, tree, static_cast<int>(index), child ? cv::Scalar(255, 0, 255) : cv::Scalar(0, 230, 255), 2);
  }
  staticResult(options, "09 Contour geometry", {
    {{"Binary regions: three objects, one hole", mask}, {"Canny edges: boundaries, not filled objects", edges},
     {"Red: boundingRect; green: minAreaRect", shapes}, {"RETR_TREE: inner boundary is a child", nested}},
    {{"external_contours", static_cast<double>(outer.size())}, {"tree_contours", static_cast<double>(tree.size())},
     {"child_contours", children}, {"edge_pixels", cv::countNonZero(edges)}}});
}
}  // namespace lab
