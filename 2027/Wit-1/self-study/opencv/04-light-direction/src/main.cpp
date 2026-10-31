#include "hsv_filter.hpp"
#include "light_direction.hpp"

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
const char* const kWindow = "OpenCV 04 - light direction";

int countShapes(const std::vector<DirectionInfo>& objects) {
  return static_cast<int>(std::count_if(objects.begin(), objects.end(),
      [](const DirectionInfo& object) { return object.shape.accepted; }));
}

int countAccepted(const std::vector<DirectionInfo>& objects) {
  return static_cast<int>(std::count_if(objects.begin(), objects.end(),
      [](const DirectionInfo& object) { return object.accepted; }));
}

std::string angleText(double degrees) {
  std::ostringstream text;
  text << std::fixed << std::setprecision(1) << degrees;
  return text.str();
}

std::pair<cv::Mat, cv::Mat> drawResults(const cv::Mat& image,
                                      const std::vector<DirectionInfo>& objects) {
  cv::Mat axes = image.clone();
  cv::Mat accepted = image.clone();
  for (std::size_t index = 0; index < objects.size(); ++index) {
    const auto& info = objects[index];
    if (!info.shape.accepted) continue;
    const auto center = info.shape.rectangle.center;
    const auto half_axis = info.long_axis * static_cast<float>(info.shape.long_side / 2.0);
    cv::line(axes, center - half_axis, center + half_axis, cv::Scalar(0, 240, 255), 2);
    // A short vertical reference makes the angle's reference direction visible.
    cv::line(axes, center, center + cv::Point2f(0, -42), cv::Scalar(230, 230, 230), 1);
    const std::string label = "#" + std::to_string(index) + " " + angleText(info.tilt_degrees);
    const cv::Point label_at(std::max(2, static_cast<int>(center.x) - 37), 286);
    cv::putText(axes, label, label_at, cv::FONT_HERSHEY_SIMPLEX, 0.42,
                cv::Scalar(235, 235, 235), 1, cv::LINE_AA);
    if (!info.accepted) continue;
    cv::Point2f corners[4];
    info.shape.rectangle.points(corners);
    for (int edge = 0; edge < 4; ++edge) {
      cv::line(accepted, corners[edge], corners[(edge + 1) % 4], cv::Scalar(0, 255, 0), 2);
    }
    cv::putText(accepted, label, label_at, cv::FONT_HERSHEY_SIMPLEX, 0.42,
                cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
  }
  cv::putText(axes, "Yellow: long axis | White: vertical | Labels: ID + tilt (deg)",
              cv::Point(12, 330), cv::FONT_HERSHEY_SIMPLEX, 0.45,
              cv::Scalar(235, 235, 235), 1, cv::LINE_AA);
  return {axes, accepted};
}

void putPanel(cv::Mat& canvas, const cv::Mat& panel, int column, int row,
              const std::string& title) {
  const int x = column * kWidth;
  const int y = row * kPanelHeight;
  cv::putText(canvas, title, cv::Point(x + 12, y + 23), cv::FONT_HERSHEY_SIMPLEX,
              0.52, cv::Scalar(235, 235, 235), 1, cv::LINE_AA);
  cv::Mat display;
  if (panel.channels() == 1) cv::cvtColor(panel, display, cv::COLOR_GRAY2BGR);
  else display = panel;
  display.copyTo(canvas(cv::Rect(x, y + kTitle, kWidth, kHeight)));
}

cv::Mat makeCanvas(const cv::Mat& image, const cv::Mat& mask,
                   const std::vector<DirectionInfo>& objects, int max_tilt) {
  cv::Mat canvas(2 * kPanelHeight + 60, 2 * kWidth, CV_8UC3, cv::Scalar(38, 38, 38));
  const auto views = drawResults(image, objects);
  putPanel(canvas, image, 0, 0, "1 Original | vertical, tilted and horizontal red bars");
  putPanel(canvas, mask, 1, 0, "2 Red mask | same HSV settings as before");
  putPanel(canvas, views.first, 0, 1, "3 Shape candidates | measure long-axis tilt from vertical");
  putPanel(canvas, views.second, 1, 1, "4 Direction passed | green boxes");
  const int footer = 2 * kPanelHeight;
  const auto stats = "Shape candidates: " + std::to_string(countShapes(objects)) +
      " | Direction passed: " + std::to_string(countAccepted(objects)) +
      " | Max tilt: " + std::to_string(max_tilt) + " deg";
  cv::putText(canvas, stats, cv::Point(12, footer + 22), cv::FONT_HERSHEY_SIMPLEX,
              0.53, cv::Scalar(235, 235, 235), 1, cv::LINE_AA);
  cv::putText(canvas, "Fixed: area >= 200, long/short >= 3.0 | S: save | Q / Esc: quit",
              cv::Point(12, footer + 47), cv::FONT_HERSHEY_SIMPLEX, 0.50,
              cv::Scalar(235, 235, 235), 1, cv::LINE_AA);
  return canvas;
}

void logSettings(const std::vector<DirectionInfo>& objects, int max_tilt) {
  std::cout << "Max tilt=" << max_tilt << " deg | shapes=" << countShapes(objects)
            << " direction_passed=" << countAccepted(objects) << std::endl;
}

void saveOutputs(const std::string& output, const cv::Mat& image, const cv::Mat& mask,
                  const std::vector<DirectionInfo>& objects, const cv::Mat& canvas) {
  const auto views = drawResults(image, objects);
  const std::vector<std::pair<std::string, cv::Mat>> files = {
      {"mask.png", mask}, {"axes.png", views.first},
      {"candidates.png", views.second}, {"overview.png", canvas}};
  for (const auto& file : files) {
    if (!cv::imwrite(output + "/" + file.first, file.second)) {
      throw std::runtime_error("Could not save " + file.first);
    }
  }
  std::ofstream csv(output + "/directions.csv");
  if (!csv) throw std::runtime_error("Could not open directions.csv");
  csv << "id,center_x,center_y,area,ratio,tilt_degrees,shape_passed,accepted\n";
  csv << std::setprecision(10);
  for (std::size_t index = 0; index < objects.size(); ++index) {
    const auto& info = objects[index];
    csv << index << ',' << info.shape.rectangle.center.x << ','
        << info.shape.rectangle.center.y << ',' << info.shape.area << ','
        << info.shape.ratio << ',' << info.tilt_degrees << ','
        << (info.shape.accepted ? 1 : 0) << ',' << (info.accepted ? 1 : 0) << '\n';
  }
  csv.flush();
  if (!csv) throw std::runtime_error("Could not finish directions.csv");
  std::cout << "Saved outputs: " << output << std::endl;
}
}  // namespace

int main(int argc, char** argv) {
  cv::CommandLineParser parser(argc, argv,
      "{help h ? |      | Show help}"
      "{image    |      | Input 640x360 image}"
      "{output   |.     | Existing output directory}"
      "{max-tilt |15    | Maximum long-axis tilt from vertical, 0..90 degrees}"
      "{headless |false | Save outputs without opening a window}");
  if (parser.has("help")) { parser.printMessage(); return 0; }
  const auto input = parser.get<std::string>("image");
  const auto output = parser.get<std::string>("output");
  int max_tilt = parser.get<int>("max-tilt");
  const bool headless = parser.get<bool>("headless");
  if (!parser.check()) { parser.printErrors(); return 2; }
  if (input.empty() || max_tilt < 0 || max_tilt > 90) {
    std::cerr << "Invalid input or max-tilt. Run with --help.\n";
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
    auto objects = analyzeDirections(mask, max_tilt);
    auto canvas = makeCanvas(image, mask, objects, max_tilt);
    std::cout << "OpenCV " << CV_VERSION << " | light direction lesson" << std::endl;
    logSettings(objects, max_tilt);
    saveOutputs(output, image, mask, objects, canvas);
    if (headless) return 0;

    cv::namedWindow(kWindow, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("Max tilt deg", kWindow, &max_tilt, 90);
    std::cout << "DIRECTION GUI READY | Compare max tilt 15, 30, 65 and 90 degrees." << std::endl;
    int previous = max_tilt;
    while (true) {
      max_tilt = cv::getTrackbarPos("Max tilt deg", kWindow);
      if (max_tilt != previous) {
        objects = analyzeDirections(mask, max_tilt);
        logSettings(objects, max_tilt);
        previous = max_tilt;
      }
      canvas = makeCanvas(image, mask, objects, max_tilt);
      cv::imshow(kWindow, canvas);
      const int key = cv::waitKey(30) & 0xff;
      if (key == 'q' || key == 'Q' || key == 27) break;
      if (key == 's' || key == 'S') saveOutputs(output, image, mask, objects, canvas);
      try {
        if (cv::getWindowProperty(kWindow, cv::WND_PROP_AUTOSIZE) < 0) break;
      } catch (const cv::Exception&) { break; }
    }
    cv::destroyAllWindows();
    return 0;
  } catch (const std::exception& error) {
    std::cerr << "Direction lesson failed: " << error.what() << std::endl;
    return 1;
  }
}
