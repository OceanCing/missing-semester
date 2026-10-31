#include "course.hpp"
#include <cmath>
namespace lab {
void runCameraModel(const Options& options) {
  interactive(options, "06 Camera model", "Depth cm", 100, 500, [&](int value) {
    const double depth = std::max(50, value) / 100.0;
    const double height = 800 * 0.1 / depth;
    cv::Mat image(480, 640, CV_8UC3, cv::Scalar(25, 25, 25));
    cv::line(image, {320, 0}, {320, 479}, {80, 80, 80});
    cv::line(image, {0, 240}, {639, 240}, {80, 80, 80});
    const int half = cvRound(height / 2), spacing = cvRound(800 * 0.135 / depth / 2);
    for (int sign : {-1, 1}) cv::rectangle(image, {320 + sign * spacing - 3, 240 - half, 6, 2 * half}, {20, 20, 240}, -1);
    cv::putText(image, "Z=" + std::to_string(depth).substr(0, 4) + " m; h=" +
                 std::to_string(cvRound(height)) + " px", {30, 40}, cv::FONT_HERSHEY_SIMPLEX, 0.7, {235, 235, 235}, 1);
    cv::Mat camera = cameraMatrix();
    const double u = 400, v = 200;
    const double x = (u - camera.at<double>(0, 2)) * depth / camera.at<double>(0, 0);
    const double y = (v - camera.at<double>(1, 2)) * depth / camera.at<double>(1, 1);
    return Result{{{"Fixed H=0.10m, fy=800px; slider below 50 is clamped", image}},
                  {{"depth_m", depth}, {"height_px", height}, {"x_at_u400_m", x}, {"y_at_v200_m", y}}};
  });
}
}  // namespace lab
