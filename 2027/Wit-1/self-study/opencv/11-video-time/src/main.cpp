#include "course.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
namespace lab {
void runVideo(const Options& options, bool pipeline) {
  if (pipeline && !options.input.empty())
    throw std::runtime_error("This pipeline uses the known synthetic camera; use lesson 11 for arbitrary videos");
  cv::VideoCapture capture;
  bool sequence = false;
  if (!options.input.empty()) {
    if (!capture.open(options.input)) throw std::runtime_error("Cannot open local video: " + options.input);
  } else if (!capture.open(options.assets + "/motion.avi")) {
    if (!capture.open(options.assets + "/frames/frame-%03d.png", cv::CAP_IMAGES))
      throw std::runtime_error("Cannot open generated video or PNG sequence");
    sequence = true;
  }
  double fps = sequence ? 30.0 : capture.get(cv::CAP_PROP_FPS);
  const bool fallback = !std::isfinite(fps) || fps <= 0 || fps > 240;
  if (fallback) fps = 30;
  const auto config = loadConfig(options.config);
  cv::VideoWriter writer;
  std::ofstream table(options.output + "/frames.csv");
  if (!table) throw std::runtime_error("Cannot write frames.csv");
  table << "frame,time_s,lights,pairs,center_u,center_v,has_pose,x_m,y_m,z_m,processing_ms\n" << std::setprecision(10);
  int frames = 0, detections = 0, poses = 0;
  double total_ms = 0;
  bool quit = false, writer_started = false, writer_open = false;
  Result last;
  const std::string window = pipeline ? "14 Offline vision pipeline" : "11 Local video and timing";
  if (!options.headless) {
    cv::namedWindow(window, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO);
    cv::resizeWindow(window, 960, pipeline ? 780 : 460);
  }
  cv::Mat frame;
  while (frames < options.frames && capture.read(frame)) {
    if (frame.empty()) break;
    if (!writer_started) {
      writer.open(options.output + "/annotated.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, frame.size());
      writer_open = writer.isOpened(); writer_started = true;
      if (!writer_open) mkdir(options.output + "/annotated-frames");
    }
    const auto start = std::chrono::steady_clock::now();
    const auto detection = detect(frame, config);
    cv::Mat annotated = detection.annotated.clone();
    const bool found = countPairs(detection) == 1;
    if (found) ++detections;
    bool has_pose = false;
    cv::Mat rvec, tvec, roi(80, 160, CV_8UC3, cv::Scalar(20, 20, 20));
    if (pipeline && found) {
      if (frame.size() != cv::Size(640, 480)) throw std::runtime_error("Synthetic K expects 640x480 images");
      const std::vector<cv::Point2f> flat{{0, 0}, {159, 0}, {159, 79}, {0, 79}};
      cv::warpPerspective(frame, roi, cv::getPerspectiveTransform(detection.corners[0], flat), {160, 80});
      has_pose = cv::solvePnP(objectCorners(), detection.corners[0], cameraMatrix(), cv::Mat(),
                               rvec, tvec, false, cv::SOLVEPNP_ITERATIVE);
      has_pose = has_pose && cv::checkRange(tvec) && tvec.at<double>(2) > 0;
      if (has_pose) {
        ++poses;
        cv::drawFrameAxes(annotated, cameraMatrix(), cv::Mat(), rvec, tvec, 0.05f);
        cv::putText(annotated, "Z=" + std::to_string(tvec.at<double>(2)).substr(0, 5) + " m (demo model)",
                     {20, 65}, cv::FONT_HERSHEY_SIMPLEX, 0.6, {235, 235, 235}, 1);
      }
    }
    if (!found) cv::putText(annotated, "NO UNIQUE TARGET", {20, 65}, cv::FONT_HERSHEY_SIMPLEX, 0.7, {0, 200, 255}, 2);
    if (options.value > 0) std::this_thread::sleep_for(std::chrono::milliseconds(options.value));
    const double elapsed = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();
    total_ms += elapsed;
    table << frames << ',' << frames / fps << ',' << countLights(detection) << ',' << countPairs(detection) << ',';
    if (found) table << detection.centers[0].x << ',' << detection.centers[0].y << ',';
    else table << ",,";
    table << (has_pose ? 1 : 0) << ',';
    if (has_pose) table << tvec.at<double>(0) << ',' << tvec.at<double>(1) << ',' << tvec.at<double>(2) << ',';
    else table << ",,,";
    table << elapsed << '\n';
    cv::putText(annotated, "frame=" + std::to_string(frames) + "  pairs=" + std::to_string(countPairs(detection)),
                 {20, 30}, cv::FONT_HERSHEY_SIMPLEX, 0.65, {235, 235, 235}, 1);
    if (writer_open) writer.write(annotated);
    else {
      std::ostringstream name;
      name << options.output << "/annotated-frames/frame-" << std::setw(3) << std::setfill('0') << frames << ".png";
      cv::imwrite(name.str(), annotated);
    }
    last.panels = pipeline ? Panels{{"Original offline frame", frame.clone()}, {"HSV mask", detection.mask},
                                     {"Lights, pairs, pose (no stale pose on loss)", annotated}, {"Perspective ROI, empty on loss", roi}} :
                             Panels{{"Original offline frame", frame.clone()}, {"Detected pairs; Space: pause; Q: quit", annotated}};
    ++frames;
    if (!options.headless) {
      cv::imshow(window, mosaic(last.panels));
      int key = cv::waitKey(std::max(1, cvRound(1000 / fps - elapsed))) & 0xff;
      if (key == ' ') do { key = cv::waitKey(30) & 0xff; } while (key != ' ' && key != 'q' && key != 27 && !closed(window));
      if (key == 'q' || key == 'Q' || key == 27 || closed(window)) { quit = true; break; }
    }
  }
  capture.release(); writer.release(); table.flush();
  if (!table || frames == 0) throw std::runtime_error("No frames processed or output CSV failed");
  const double mean = total_ms / frames;
  last.metrics = {{"frames", frames}, {"detected_frames", detections}, {"pose_frames", poses},
                  {"source_fps", fps}, {"mean_processing_ms", mean},
                  {"processing_capacity_fps", mean > 0 ? 1000 / mean : 0},
                  {"writer_open", writer_open ? 1.0 : 0.0}, {"fps_fallback", fallback ? 1.0 : 0.0}};
  if (options.headless || quit) {
    saveResult(options, last);
    if (!options.headless && !closed(window)) cv::destroyWindow(window);
  } else staticResult(options, window, last);
}
void runVideoLesson(const Options& options) { runVideo(options, false); }
}  // namespace lab
