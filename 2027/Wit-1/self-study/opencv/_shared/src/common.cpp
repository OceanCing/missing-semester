#include "course.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace lab {
void mkdir(const std::string& path) { fs::create_directories(path); }
cv::Mat readImage(const std::string& path, int flags) {
  auto image = cv::imread(path, flags);
  if (image.empty()) throw std::runtime_error("Cannot read image: " + path);
  return image;
}
cv::Mat cameraMatrix() {
  return (cv::Mat_<double>(3, 3) << 800, 0, 320, 0, 800, 240, 0, 0, 1);
}
std::vector<cv::Point3f> objectCorners(double width, double height) {
  const float x = static_cast<float>(width / 2), y = static_cast<float>(height / 2);
  return {{-x, -y, 0}, {x, -y, 0}, {x, y, 0}, {-x, y, 0}};
}
cv::Mat mosaic(const Panels& panels) {
  if (panels.empty()) throw std::runtime_error("No panels");
  const int columns = panels.size() == 1 ? 1 : 2;
  const int rows = static_cast<int>((panels.size() + columns - 1) / columns);
  cv::Mat canvas(rows * 392, columns * 480, CV_8UC3, cv::Scalar(30, 30, 30));
  for (std::size_t index = 0; index < panels.size(); ++index) {
    const int x = static_cast<int>(index % columns) * 480;
    const int y = static_cast<int>(index / columns) * 392;
    cv::putText(canvas, panels[index].first, {x + 8, y + 23},
                cv::FONT_HERSHEY_SIMPLEX, 0.49, {235, 235, 235}, 1, cv::LINE_AA);
    cv::Mat bgr, resized;
    if (panels[index].second.channels() == 1)
      cv::cvtColor(panels[index].second, bgr, cv::COLOR_GRAY2BGR);
    else bgr = panels[index].second;
    const double scale = std::min(480.0 / bgr.cols, 360.0 / bgr.rows);
    cv::resize(bgr, resized, {}, scale, scale, cv::INTER_AREA);
    resized.copyTo(canvas(cv::Rect(x + (480 - resized.cols) / 2,
                                   y + 32 + (360 - resized.rows) / 2,
                                   resized.cols, resized.rows)));
  }
  return canvas;
}
void saveResult(const Options& options, const Result& result) {
  mkdir(options.output);
  cv::FileStorage metrics(options.output + "/metrics.json",
                         cv::FileStorage::WRITE | cv::FileStorage::FORMAT_JSON);
  if (!metrics.isOpened()) throw std::runtime_error("Cannot open metrics.json");
  for (const auto& entry : result.metrics) {
    if (!std::isfinite(entry.second)) throw std::runtime_error("Non-finite metric: " + entry.first);
    metrics << entry.first << entry.second;
  }
  for (std::size_t index = 0; index < result.panels.size(); ++index)
    if (!cv::imwrite(options.output + "/panel-" + std::to_string(index) + ".png",
                     result.panels[index].second)) throw std::runtime_error("Cannot write panel");
  if (!cv::imwrite(options.output + "/overview.png", mosaic(result.panels)))
    throw std::runtime_error("Cannot write overview");
  std::cout << "Saved: " << options.output << std::endl;
}
bool closed(const std::string& window) {
  try { return cv::getWindowProperty(window, cv::WND_PROP_AUTOSIZE) < 0; }
  catch (const cv::Exception&) { return true; }
}
void interactive(const Options& options, const std::string& title,
                 const std::string& slider, int initial, int maximum,
                 const std::function<Result(int)>& evaluate) {
  int value = options.value >= 0 ? options.value : initial;
  if (value < 0 || value > maximum) throw std::runtime_error("Slider value outside allowed range");
  auto result = evaluate(value);
  saveResult(options, result);
  if (options.headless) return;
  cv::namedWindow(title, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO);
  if (!slider.empty()) cv::createTrackbar(slider, title, &value, maximum);
  cv::resizeWindow(title, 960, result.panels.size() > 2 ? 780 : 460);
  int previous = value;
  std::cout << "GUI READY | Q/Esc: next/quit; S: save current results" << std::endl;
  while (true) {
    if (!slider.empty()) value = cv::getTrackbarPos(slider, title);
    if (value != previous) {
      result = evaluate(value);
      std::cout << slider << '=' << value << std::endl;
      previous = value;
    }
    cv::imshow(title, mosaic(result.panels));
    const int key = cv::waitKey(30) & 0xff;
    if (key == 'q' || key == 'Q' || key == 27 || closed(title)) break;
    if (key == 's' || key == 'S') saveResult(options, result);
  }
  cv::destroyWindow(title);
}
void staticResult(const Options& options, const std::string& title, const Result& result) {
  auto display_options = options;
  display_options.value = -1;  // A static view must not reuse a video delay as a slider value.
  interactive(display_options, title, "", 0, 0, [&](int) { return result; });
}
double reprojectionError(const std::vector<cv::Point3f>& object,
                         const std::vector<cv::Point2f>& image,
                         const cv::Mat& rvec, const cv::Mat& tvec,
                         const cv::Mat& K, const cv::Mat& distortion) {
  std::vector<cv::Point2f> projected;
  cv::projectPoints(object, rvec, tvec, K, distortion, projected);
  return cv::norm(projected, image, cv::NORM_L2) / std::sqrt(static_cast<double>(image.size()));
}
}  // namespace lab
