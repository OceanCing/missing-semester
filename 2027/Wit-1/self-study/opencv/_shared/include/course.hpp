#pragma once
#include "hsv_filter.hpp"
#include "light_pairing.hpp"
#include <opencv2/calib3d.hpp>
#include <opencv2/videoio.hpp>
#include <filesystem>
#include <cmath>
#include <functional>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace lab {
namespace fs = std::filesystem;
struct Options {
  std::string topic, assets, output, input, config, variant;
  bool headless = false, force = false;
  int value = -1, frames = 180;
  double square = 0.025;
};
using Panels = std::vector<std::pair<std::string, cv::Mat>>;
using Metrics = std::map<std::string, double>;
struct Result { Panels panels; Metrics metrics; };
struct DetectorConfig {
  HsvSettings hsv;
  int min_area = 200;
  double min_ratio = 3, max_tilt = 35, max_difference = 10;
  PairSettings pairing;
};
struct Detection {
  cv::Mat mask, annotated;
  std::vector<DirectionInfo> lights;
  std::vector<PairInfo> pairs;
  std::vector<std::vector<cv::Point2f>> corners;
  std::vector<cv::Point2f> centers;
};
struct SyntheticFrame {
  cv::Mat image;
  cv::Vec3d rvec, tvec;
  std::vector<cv::Point2f> corners;
  bool visible = true;
};
constexpr double kObjectWidth = 0.15;
constexpr double kObjectHeight = 0.12;
cv::Mat cameraMatrix();
std::vector<cv::Point3f> objectCorners(double width = kObjectWidth, double height = kObjectHeight);
DetectorConfig loadConfig(const std::string& path);
Detection detect(const cv::Mat& image, const DetectorConfig& config);
SyntheticFrame syntheticFrame(int index);
void prepare(const Options& options);
void mkdir(const std::string& path);
cv::Mat readImage(const std::string& path, int flags = cv::IMREAD_COLOR);
cv::Mat mosaic(const Panels& panels);
void saveResult(const Options& options, const Result& result);
void interactive(const Options& options, const std::string& title,
                 const std::string& slider, int initial, int maximum,
                 const std::function<Result(int)>& evaluate);
void staticResult(const Options& options, const std::string& title, const Result& result);
bool closed(const std::string& window);
int countLights(const Detection& detection);
int countPairs(const Detection& detection);
double reprojectionError(const std::vector<cv::Point3f>& object,
                         const std::vector<cv::Point2f>& image,
                         const cv::Mat& rvec, const cv::Mat& tvec,
                         const cv::Mat& K, const cv::Mat& distortion);
void runCameraModel(const Options&);
void runMat(const Options&);
void runPreprocess(const Options&);
void runGeometry(const Options&);
void runWarp(const Options&);
void runCalibration(const Options&);
void runPnp(const Options&);
void runVideo(const Options&, bool pipeline);
void runVideoLesson(const Options&);
void runPipeline(const Options&);
}  // namespace lab
