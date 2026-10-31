#include "course.hpp"
#include <cmath>
namespace lab {
void runPnp(const Options& options) {
  if (!options.variant.empty() && options.variant != "wrong-size") throw std::runtime_error("variant must be empty or wrong-size");
  interactive(options, "13 PnP and coordinates", "Point noise px", 0, 5, [&](int noise) {
    const auto truth_object = objectCorners();
    const auto model = objectCorners(options.variant == "wrong-size" ? 2 * kObjectWidth : kObjectWidth,
                                      options.variant == "wrong-size" ? 2 * kObjectHeight : kObjectHeight);
    const cv::Vec3d truth_r(0.12, -0.2, 0.08), truth_t(0.1, 0.03, 2.0);
    std::vector<cv::Point2f> image;
    cv::projectPoints(truth_object, truth_r, truth_t, cameraMatrix(), cv::Mat(), image);
    cv::RNG random(23);
    for (auto& point : image) {
      point.x += static_cast<float>(random.gaussian(noise));
      point.y += static_cast<float>(random.gaussian(noise));
    }
    cv::Mat rvec, tvec;
    if (!cv::solvePnP(model, image, cameraMatrix(), cv::Mat(), rvec, tvec, false, cv::SOLVEPNP_ITERATIVE))
      throw std::runtime_error("solvePnP did not find a pose");
    if (tvec.at<double>(2) <= 0) throw std::runtime_error("Rejected pose behind camera");
    cv::Mat display(480, 640, CV_8UC3, cv::Scalar(25, 25, 25));
    std::vector<cv::Point2f> reprojected;
    cv::projectPoints(model, rvec, tvec, cameraMatrix(), cv::Mat(), reprojected);
    for (int index = 0; index < 4; ++index) {
      cv::circle(display, image[index], 5, {0, 255, 0}, -1);
      cv::circle(display, reprojected[index], 8, {255, 220, 0}, 1);
      cv::putText(display, std::to_string(index), image[index] + cv::Point2f(7, -8), cv::FONT_HERSHEY_SIMPLEX, 0.6, {235, 235, 235}, 1);
    }
    cv::drawFrameAxes(display, cameraMatrix(), cv::Mat(), rvec, tvec, 0.06f);
    const cv::Vec3d camera_point(tvec.at<double>(0), tvec.at<double>(1), tvec.at<double>(2));
    const cv::Matx33d body_from_optical(0, 0, 1, -1, 0, 0, 0, -1, 0);
    const cv::Vec3d body_point = body_from_optical * camera_point + cv::Vec3d(0.2, 0, 0.1);
    const double rms = reprojectionError(model, image, rvec, tvec, cameraMatrix(), cv::Mat());
    cv::putText(display, "Z=" + std::to_string(camera_point[2]).substr(0, 5) + " m", {30, 40}, cv::FONT_HERSHEY_SIMPLEX, 0.8, {235, 235, 235}, 1);
    return Result{{{"Green: measured pixels; cyan: reprojected; XYZ axes", display}},
      {{"x_m", camera_point[0]}, {"y_m", camera_point[1]}, {"z_m", camera_point[2]},
       {"range_m", cv::norm(camera_point)}, {"reprojection_rms_px", rms},
       {"translation_error_m", cv::norm(camera_point - truth_t)},
       {"body_x_m", body_point[0]}, {"body_y_m", body_point[1]}, {"body_z_m", body_point[2]},
       {"model_scale", options.variant == "wrong-size" ? 2.0 : 1.0}}};
  });
}
}  // namespace lab
