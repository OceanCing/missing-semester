#include "contour_filter.hpp"
#include "hsv_filter.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {
constexpr int kWidth = 640;
constexpr int kHeight = 360;
constexpr int kTitle = 32;
constexpr int kPanelHeight = kHeight + kTitle;
const char* const kWindow = "OpenCV 03 - contours and light candidates";

int acceptedCount(const std::vector<ContourInfo>& contours) {
  return static_cast<int>(std::count_if(contours.begin(), contours.end(),
                                      [](const ContourInfo& info) { return info.accepted; }));
}

std::string formatRatio(int ratio_times10) {
  std::ostringstream text;
  text << std::fixed << std::setprecision(1) << ratio_times10 / 10.0;
  return text.str();
}

std::pair<cv::Mat, cv::Mat> drawResults(const cv::Mat& image,
                                      const std::vector<ContourInfo>& contours) {
  cv::Mat outlines = cv::Mat::zeros(image.size(), CV_8UC3);
  cv::Mat candidates = image.clone();
  for (std::size_t index = 0; index < contours.size(); ++index) {
    const auto& info = contours[index];
    const std::vector<std::vector<cv::Point>> one{info.points};
    cv::drawContours(outlines, one, -1, cv::Scalar(0, 220, 255), 2);
    auto label = cv::boundingRect(info.points).tl();
    label.x = std::min(label.x, kWidth - 50);
    label.y = std::max(14, label.y - 7);
    const std::string id = "#" + std::to_string(index);
    cv::putText(outlines, id, label, cv::FONT_HERSHEY_SIMPLEX, 0.45,
                cv::Scalar(230, 230, 230), 1, cv::LINE_AA);
    if (info.accepted) {
      cv::Point2f corners[4];
      info.rectangle.points(corners);
      for (int edge = 0; edge < 4; ++edge) {
        cv::line(candidates, corners[edge], corners[(edge + 1) % 4],
                  cv::Scalar(0, 255, 0), 2);
      }
      cv::putText(candidates, id, label, cv::FONT_HERSHEY_SIMPLEX, 0.45,
                  cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
    }
  }
  return {outlines, candidates};
}

void putPanel(cv::Mat& canvas, const cv::Mat& image, int column, int row,
              const std::string& title) {
  const int x = column * kWidth;
  const int y = row * kPanelHeight;
  cv::putText(canvas, title, cv::Point(x + 12, y + 23),
              cv::FONT_HERSHEY_SIMPLEX, 0.54, cv::Scalar(235, 235, 235), 1, cv::LINE_AA);
  cv::Mat display;
  if (image.channels() == 1) cv::cvtColor(image, display, cv::COLOR_GRAY2BGR);
  else display = image;
  display.copyTo(canvas(cv::Rect(x, y + kTitle, kWidth, kHeight)));
}

cv::Mat makeCanvas(const cv::Mat& image, const cv::Mat& mask,
                   const std::vector<ContourInfo>& contours,
                   int min_area, int ratio_times10) {
  cv::Mat canvas(2 * kPanelHeight + 60, 2 * kWidth, CV_8UC3, cv::Scalar(38, 38, 38));
  const auto views = drawResults(image, contours);
  putPanel(canvas, image, 0, 0, "1 Original | red bars, red blocks and tiny red noise");
  putPanel(canvas, mask, 1, 0, "2 Red mask | reuse HSV from lesson 02");
  putPanel(canvas, views.first, 0, 1, "3 All outer contours | yellow boundaries + IDs");
  putPanel(canvas, views.second, 1, 1, "4 Light candidates | green boxes passed filters");
  const int footer = 2 * kPanelHeight;
  const std::string stats = "Contours: " + std::to_string(contours.size()) +
      " | Candidates: " + std::to_string(acceptedCount(contours)) +
      " | Area >= " + std::to_string(min_area) + " | Long/short >= " + formatRatio(ratio_times10);
  cv::putText(canvas, stats, cv::Point(12, footer + 22),
              cv::FONT_HERSHEY_SIMPLEX, 0.53, cv::Scalar(235, 235, 235), 1, cv::LINE_AA);
  cv::putText(canvas, "Min ratio x10: 30 means 3.0 | S: save images + contours.csv | Q / Esc: quit",
              cv::Point(12, footer + 47), cv::FONT_HERSHEY_SIMPLEX, 0.50,
              cv::Scalar(235, 235, 235), 1, cv::LINE_AA);
  return canvas;
}

void logSettings(const std::vector<ContourInfo>& contours, int area, int ratio) {
  std::cout << "Area>=" << area << " ratio>=" << formatRatio(ratio)
            << " contours=" << contours.size() << " candidates=" << acceptedCount(contours)
            << std::endl;
}

void saveOutputs(const std::string& output, const cv::Mat& image, const cv::Mat& mask,
                  const std::vector<ContourInfo>& contours, const cv::Mat& canvas) {
  const auto views = drawResults(image, contours);
  const std::vector<std::pair<std::string, cv::Mat>> files = {
      {"mask.png", mask}, {"contours.png", views.first},
      {"candidates.png", views.second}, {"overview.png", canvas}};
  for (const auto& file : files) {
    if (!cv::imwrite(output + "/" + file.first, file.second)) {
      throw std::runtime_error("Could not save " + file.first);
    }
  }
  std::ofstream csv(output + "/contours.csv");
  if (!csv) throw std::runtime_error("Could not open contours.csv");
  csv << "id,center_x,center_y,area,short_side,long_side,ratio,accepted\n";
  csv << std::setprecision(10);
  for (std::size_t index = 0; index < contours.size(); ++index) {
    const auto& info = contours[index];
    csv << index << ',' << info.rectangle.center.x << ',' << info.rectangle.center.y << ','
        << info.area << ',' << info.short_side << ',' << info.long_side << ','
        << info.ratio << ',' << (info.accepted ? 1 : 0) << '\n';
  }
  csv.flush();
  if (!csv) throw std::runtime_error("Could not finish contours.csv");
  std::cout << "Saved outputs: " << output << std::endl;
}
}  // namespace

int main(int argc, char** argv) {
  cv::CommandLineParser parser(argc, argv,
      "{help h ? |      | Show help}"
      "{image    |      | Input 640x360 image}"
      "{output   |.     | Existing output directory}"
      "{min-area |200   | Minimum contour area, 0..5000}"
      "{ratio-times10|30| Minimum long/short ratio times 10, 0..100}"
      "{headless |false | Save outputs without opening a window}");
  if (parser.has("help")) { parser.printMessage(); return 0; }
  const auto input = parser.get<std::string>("image");
  const auto output = parser.get<std::string>("output");
  int min_area = parser.get<int>("min-area");
  int min_ratio_times10 = parser.get<int>("ratio-times10");
  const bool headless = parser.get<bool>("headless");
  if (!parser.check()) { parser.printErrors(); return 2; }
  if (input.empty() || min_area < 0 || min_area > 5000 ||
      min_ratio_times10 < 0 || min_ratio_times10 > 100) {
    std::cerr << "Invalid image or filter settings. Run with --help.\n";
    return 2;
  }
  try {
    const cv::Mat image = cv::imread(input, cv::IMREAD_COLOR);
    if (image.empty() || image.size() != cv::Size(kWidth, kHeight)) {
      std::cerr << "This lesson expects a readable 640x360 image.\n";
      return 2;
    }
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    const cv::Mat mask = selectHsv(hsv, HsvSettings{});
    auto contours = analyzeContours(mask, min_area, min_ratio_times10 / 10.0);
    auto canvas = makeCanvas(image, mask, contours, min_area, min_ratio_times10);
    std::cout << "OpenCV " << CV_VERSION << " | contours lesson" << std::endl;
    logSettings(contours, min_area, min_ratio_times10);
    saveOutputs(output, image, mask, contours, canvas);
    if (headless) return 0;

    cv::namedWindow(kWindow, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("Min area", kWindow, &min_area, 5000);
    cv::createTrackbar("Min ratio x10", kWindow, &min_ratio_times10, 100);
    std::cout << "CONTOUR GUI READY | Compare Min area 200 and 0. Q/Esc quits." << std::endl;
    int previous_area = min_area;
    int previous_ratio = min_ratio_times10;
    while (true) {
      min_area = cv::getTrackbarPos("Min area", kWindow);
      min_ratio_times10 = cv::getTrackbarPos("Min ratio x10", kWindow);
      if (min_area != previous_area || min_ratio_times10 != previous_ratio) {
        contours = analyzeContours(mask, min_area, min_ratio_times10 / 10.0);
        logSettings(contours, min_area, min_ratio_times10);
        previous_area = min_area;
        previous_ratio = min_ratio_times10;
      }
      canvas = makeCanvas(image, mask, contours, min_area, min_ratio_times10);
      cv::imshow(kWindow, canvas);
      const int key = cv::waitKey(30) & 0xff;
      if (key == 'q' || key == 'Q' || key == 27) break;
      if (key == 's' || key == 'S') saveOutputs(output, image, mask, contours, canvas);
      try {
        if (cv::getWindowProperty(kWindow, cv::WND_PROP_AUTOSIZE) < 0) break;
      } catch (const cv::Exception&) { break; }
    }
    cv::destroyAllWindows();
    return 0;
  } catch (const std::exception& error) {
    std::cerr << "Contour lesson failed: " << error.what() << std::endl;
    return 1;
  }
}
