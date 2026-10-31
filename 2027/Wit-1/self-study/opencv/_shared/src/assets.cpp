#include "course.hpp"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace lab {
namespace {
std::vector<cv::Point> pixels(const std::vector<cv::Point2f>& points) {
  std::vector<cv::Point> result;
  for (const auto& point : points) result.emplace_back(cvRound(point.x), cvRound(point.y));
  return result;
}
void projectedPolygon(cv::Mat& image, const std::vector<cv::Point3f>& object,
                        const cv::Vec3d& rvec, const cv::Vec3d& tvec, cv::Scalar color) {
  std::vector<cv::Point2f> projected;
  cv::projectPoints(object, rvec, tvec, cameraMatrix(), cv::Mat(), projected);
  cv::fillConvexPoly(image, pixels(projected), color, cv::LINE_8);
}
}  // namespace
SyntheticFrame syntheticFrame(int index) {
  SyntheticFrame frame;
  frame.image = cv::Mat(480, 640, CV_8UC3);
  for (int y = 0; y < frame.image.rows; ++y)
    frame.image.row(y).setTo(cv::Scalar::all(18 + y / 50));
  const double phase = 2 * CV_PI * index / 180.0;
  frame.rvec = {0.025 * std::sin(phase), 0.07 * std::cos(phase), 0.12 * std::sin(phase)};
  frame.tvec = {0.18 * std::sin(phase), 0.065 * std::cos(phase), 1.65 + 0.25 * std::sin(phase)};
  cv::projectPoints(objectCorners(), frame.rvec, frame.tvec, cameraMatrix(), cv::Mat(), frame.corners);
  for (float x : {-0.075f, 0.075f}) {
    const std::vector<cv::Point3f> bar{{x - 0.007f, -0.06f, 0}, {x + 0.007f, -0.06f, 0},
                                      {x + 0.007f, 0.06f, 0}, {x - 0.007f, 0.06f, 0}};
    projectedPolygon(frame.image, bar, frame.rvec, frame.tvec, {20, 20, 240});
  }
  const auto center = (frame.corners[0] + frame.corners[1] + frame.corners[2] + frame.corners[3]) * 0.25f;
  cv::putText(frame.image, "7", center + cv::Point2f(-10, 12), cv::FONT_HERSHEY_SIMPLEX,
              0.9, {220, 220, 220}, 2, cv::LINE_AA);
  cv::rectangle(frame.image, {45, 60, 45, 45}, {25, 25, 180}, -1);
  cv::rectangle(frame.image, {540, 330, 16, 100}, {220, 30, 20}, -1);
  for (int noise = 0; noise < 12; ++noise)
    cv::rectangle(frame.image, {15 + noise * 47, 445 + noise % 3, 2, 3}, {20, 20, 240}, -1);
  frame.visible = !(index >= 80 && index < 95);
  if (!frame.visible) {
    const auto right = (frame.corners[1] + frame.corners[2]) * 0.5f;
    const cv::Rect cover(cvRound(right.x) - 13, cvRound(right.y) - 60, 27, 120);
    cv::rectangle(frame.image, cover & cv::Rect(0, 0, 640, 480), {20, 20, 20}, -1);
  }
  return frame;
}
void prepare(const Options& options) {
  mkdir(options.assets);
  const auto marker = options.assets + "/asset-version.txt";
  std::ifstream existing(marker);
  int version = 0; existing >> version;
  if (!options.force && version == 1 && fs::exists(options.assets + "/frames/frame-000.png") &&
      fs::exists(options.assets + "/boards/board-19.png")) return;
  mkdir(options.assets + "/frames"); mkdir(options.assets + "/boards");
  auto scene = syntheticFrame(0);
  cv::imwrite(options.assets + "/scene.png", scene.image);
  cv::Mat mask = cv::Mat::zeros(480, 640, CV_8UC1);
  cv::rectangle(mask, {100, 100, 24, 220}, {255}, -1);
  cv::rectangle(mask, {250, 100, 24, 220}, {255}, -1);
  mask(cv::Rect(250, 200, 24, 2)).setTo(0);
  mask.at<uchar>(50, 50) = 255;
  for (int i = 0; i < 20; ++i) mask.at<uchar>(400 + i % 5, 20 + i * 28) = 255;
  cv::imwrite(options.assets + "/broken-mask.png", mask);
  cv::Mat gradient(480, 640, CV_8UC1);
  for (int y = 0; y < 480; ++y) for (int x = 0; x < 640; ++x)
    gradient.at<uchar>(y, x) = static_cast<uchar>(55 + 160 * x / 639);
  for (int x = 70; x < 600; x += 100)
    cv::putText(gradient, "7", {x, 290}, cv::FONT_HERSHEY_SIMPLEX, 3, {20}, 8, cv::LINE_8);
  cv::imwrite(options.assets + "/gradient.png", gradient);
  cv::Mat pattern(80, 160, CV_8UC3, cv::Scalar(30, 30, 30));
  cv::rectangle(pattern, {5, 5, 8, 70}, {20, 20, 240}, -1);
  cv::rectangle(pattern, {147, 5, 8, 70}, {20, 20, 240}, -1);
  cv::putText(pattern, "7", {60, 65}, cv::FONT_HERSHEY_SIMPLEX, 2, {240, 240, 240}, 4);
  cv::imwrite(options.assets + "/pattern.png", pattern);

  cv::VideoWriter video(options.assets + "/motion.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                        30, {640, 480});
  const bool avi_available = video.isOpened();
  std::ofstream truth(options.assets + "/truth.csv");
  if (!truth) throw std::runtime_error("Cannot write truth.csv");
  truth << "frame,time_s,u,v,x_m,y_m,z_m,visible\n" << std::setprecision(10);
  for (int index = 0; index < 180; ++index) {
    const auto current = syntheticFrame(index);
    std::ostringstream name;
    name << options.assets << "/frames/frame-" << std::setw(3) << std::setfill('0') << index << ".png";
    if (!cv::imwrite(name.str(), current.image)) throw std::runtime_error("Cannot write video frame");
    if (avi_available) video.write(current.image);
    std::vector<cv::Point2f> center;
    cv::projectPoints(std::vector<cv::Point3f>{{0, 0, 0}}, current.rvec, current.tvec,
                       cameraMatrix(), cv::Mat(), center);
    truth << index << ',' << index / 30.0 << ',' << center[0].x << ',' << center[0].y << ','
          << current.tvec[0] << ',' << current.tvec[1] << ',' << current.tvec[2] << ','
          << (current.visible ? 1 : 0) << '\n';
  }
  video.release();
  cv::Mat board(280, 400, CV_8UC1);
  for (int row = 0; row < 7; ++row) for (int column = 0; column < 10; ++column)
    board(cv::Rect(column * 40, row * 40, 40, 40)).setTo((row + column) % 2 ? 255 : 0);
  const std::vector<cv::Point3f> outer{{-0.025f, -0.025f, 0}, {0.225f, -0.025f, 0},
                                     {0.225f, 0.15f, 0}, {-0.025f, 0.15f, 0}};
  const std::vector<cv::Point2f> source{{0, 0}, {399, 0}, {399, 279}, {0, 279}};
  for (int index = 0; index < 20; ++index) {
    const cv::Vec3d rotation(0.28 * std::sin(index * 0.7), 0.32 * std::cos(index * 0.5),
                             0.16 * std::sin(index * 0.4));
    const cv::Vec3d translation(-0.1 + 0.065 * std::sin(index * 1.1),
                                -0.0625 + 0.045 * std::cos(index * 0.8), 0.65 + 0.015 * index);
    std::vector<cv::Point2f> projected;
    cv::projectPoints(outer, rotation, translation, cameraMatrix(), cv::Mat(), projected);
    cv::Mat image;
    cv::warpPerspective(board, image, cv::getPerspectiveTransform(source, projected), {640, 480},
                         cv::INTER_LINEAR, cv::BORDER_CONSTANT, {127});
    std::ostringstream name;
    name << options.assets << "/boards/board-" << std::setw(2) << std::setfill('0') << index << ".png";
    cv::imwrite(name.str(), image);
  }
  cv::FileStorage camera(options.assets + "/camera-ground-truth.yaml", cv::FileStorage::WRITE);
  camera << "source" << "SYNTHETIC_PRACTICE" << "image_width" << 640 << "image_height" << 480
         << "camera_matrix" << cameraMatrix() << "distortion" << cv::Mat::zeros(1, 5, CV_64F)
         << "object_width_m" << kObjectWidth << "object_height_m" << kObjectHeight;
  std::ofstream marker_file(marker); marker_file << 1 << '\n';
  std::cout << "Prepared offline assets: 180 frames, 20 chessboards; MJPG=" << avi_available << std::endl;
}
}  // namespace lab
