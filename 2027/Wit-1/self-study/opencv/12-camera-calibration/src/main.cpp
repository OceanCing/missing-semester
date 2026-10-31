#include "course.hpp"
#include <fstream>
#include <iomanip>
namespace lab {
namespace {
void distortionDemo(const Options& options) {
  cv::Mat ideal(480, 640, CV_8UC3, cv::Scalar(25, 25, 25));
  for (int x = 20; x < 640; x += 40) cv::line(ideal, {x, 0}, {x, 479}, {230, 230, 230});
  for (int y = 20; y < 480; y += 40) cv::line(ideal, {0, y}, {639, y}, {230, 230, 230});
  const auto K = cameraMatrix();
  const cv::Mat dist = (cv::Mat_<double>(1, 5) << -0.25, 0.08, 0, 0, 0);
  std::vector<cv::Point2f> destination, undistorted;
  destination.reserve(640 * 480);
  for (int y = 0; y < 480; ++y) for (int x = 0; x < 640; ++x)
    destination.emplace_back(static_cast<float>(x), static_cast<float>(y));
  cv::undistortPoints(destination, undistorted, K, dist, cv::noArray(), K);
  cv::Mat mapping(480, 640, CV_32FC2, undistorted.data());
  cv::Mat distorted, corrected;
  cv::remap(ideal, distorted, mapping, cv::Mat(), cv::INTER_LINEAR);
  cv::undistort(distorted, corrected, K, dist, K);
  const cv::Rect middle(40, 40, 560, 400);
  const double divisor = static_cast<double>(middle.area() * 3);
  staticResult(options, "12 Distortion", {
    {{"Ideal straight grid", ideal}, {"Synthetic radial distortion", distorted},
     {"Correct K and distortion", corrected}, {"Wrong zero distortion: unchanged input", distorted}},
    {{"distorted_MAE", cv::norm(ideal(middle), distorted(middle), cv::NORM_L1) / divisor},
     {"corrected_MAE", cv::norm(ideal(middle), corrected(middle), cv::NORM_L1) / divisor}}});
}
}  // namespace
void runCalibration(const Options& options) {
  if (options.variant == "distortion") { distortionDemo(options); return; }
  if (!options.variant.empty()) throw std::runtime_error("Unknown calibration variant");
  const std::string pattern = options.input.empty() ? options.assets + "/boards/*.png" :
      (fs::is_directory(options.input) ? options.input + "/*.png" : options.input);
  std::vector<cv::String> paths;
  cv::glob(pattern, paths);
  std::vector<cv::Point3f> grid;
  for (int row = 0; row < 6; ++row) for (int column = 0; column < 9; ++column)
    grid.emplace_back(static_cast<float>(column * options.square), static_cast<float>(row * options.square), 0);
  std::vector<std::vector<cv::Point2f>> image_points;
  std::vector<std::vector<cv::Point3f>> object_points;
  std::vector<std::string> accepted_paths;
  cv::Mat first_image, first_overlay;
  cv::Size size;
  for (const auto& path : paths) {
    const auto gray = readImage(path, cv::IMREAD_GRAYSCALE);
    if (size.empty()) size = gray.size();
    if (gray.size() != size) throw std::runtime_error("Calibration images must share one resolution");
    std::vector<cv::Point2f> corners;
    if (!cv::findChessboardCorners(gray, {9, 6}, corners,
           cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE)) continue;
    cv::cornerSubPix(gray, corners, {5, 5}, {-1, -1},
                      {cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 40, 0.001});
    object_points.push_back(grid); image_points.push_back(corners); accepted_paths.push_back(path);
    if (first_image.empty()) {
      first_image = gray.clone(); cv::cvtColor(gray, first_overlay, cv::COLOR_GRAY2BGR);
      cv::drawChessboardCorners(first_overlay, {9, 6}, corners, true);
    }
  }
  if (image_points.size() < 8) throw std::runtime_error("Need at least eight valid 9x6 chessboard views");
  cv::Mat K, distortion;
  std::vector<cv::Mat> rotations, translations;
  const double rms = cv::calibrateCamera(object_points, image_points, size, K, distortion,
                                         rotations, translations);
  cv::Mat undistorted;
  cv::undistort(first_image, undistorted, K, distortion, K);
  cv::FileStorage file(options.output + "/calibration.yaml", cv::FileStorage::WRITE);
  file << "source" << (options.input.empty() ? "SYNTHETIC_CHESSBOARD_IMAGES" : "USER_PROVIDED_IMAGES")
       << "image_width" << size.width << "image_height" << size.height
       << "square_size_m" << options.square << "camera_matrix" << K << "distortion" << distortion
       << "rms_px" << rms << "accepted_views" << static_cast<int>(image_points.size());
  std::ofstream per_view(options.output + "/per-view.csv");
  per_view << "view,rms_px\n";
  for (std::size_t index = 0; index < image_points.size(); ++index)
    per_view << index << ',' << reprojectionError(grid, image_points[index], rotations[index], translations[index], K, distortion) << '\n';
  staticResult(options, "12 Camera calibration", {
    {{"Generated chessboard view (or supplied input)", first_image}, {"Detected 9x6 inner corners", first_overlay},
     {"Undistorted with fitted parameters", undistorted}},
    {{"views", static_cast<double>(image_points.size())}, {"rms_px", rms},
     {"fx", K.at<double>(0, 0)}, {"fy", K.at<double>(1, 1)},
     {"cx", K.at<double>(0, 2)}, {"cy", K.at<double>(1, 2)},
     {"synthetic_input", options.input.empty() ? 1.0 : 0.0}}});
}
}  // namespace lab
