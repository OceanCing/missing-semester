#include "hsv_filter.hpp"
#include "light_pairing.hpp"

#include <algorithm>
#include <array>
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
const char* const kWindow = "OpenCV 05 - light pairing";

std::string number(double value, bool with_sign = false) {
  std::ostringstream text;
  if (with_sign) text << std::showpos;
  text << std::fixed << std::setprecision(1) << value;
  return text.str();
}

int lightCount(const std::vector<DirectionInfo>& lights) {
  return static_cast<int>(std::count_if(lights.begin(), lights.end(),
      [](const DirectionInfo& info) { return info.accepted; }));
}

int pairCount(const std::vector<PairInfo>& pairs, bool geometry_only = false) {
  return static_cast<int>(std::count_if(pairs.begin(), pairs.end(),
      [geometry_only](const PairInfo& info) {
        return geometry_only ? info.geometry_passed : info.accepted;
      }));
}

cv::Mat drawLights(const cv::Mat& image, const std::vector<DirectionInfo>& lights) {
  cv::Mat view = image.clone();
  for (std::size_t index = 0; index < lights.size(); ++index) {
    const auto& light = lights[index];
    if (!light.accepted) continue;
    cv::Point2f corners[4];
    light.shape.rectangle.points(corners);
    for (int edge = 0; edge < 4; ++edge) {
      cv::line(view, corners[edge], corners[(edge + 1) % 4], cv::Scalar(0, 255, 0), 2);
    }
    const auto center = light.shape.rectangle.center;
    const std::string label = "#" + std::to_string(index) + " " + number(signedTilt(light), true);
    cv::putText(view, label, cv::Point(static_cast<int>(center.x) - 43, 280),
                cv::FONT_HERSHEY_SIMPLEX, 0.46, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
  }
  return view;
}

void drawPair(cv::Mat& view, const PairInfo& pair, const std::vector<DirectionInfo>& lights,
               const cv::Scalar& color) {
  const auto& left = lights[pair.left];
  const auto& right = lights[pair.right];
  const auto lc = left.shape.rectangle.center;
  const auto rc = right.shape.rectangle.center;
  const auto la = upwardAxis(left) * static_cast<float>(left.shape.long_side / 2.0);
  const auto ra = upwardAxis(right) * static_cast<float>(right.shape.long_side / 2.0);
  const std::array<cv::Point2f, 4> corners{{lc + la, rc + ra, rc - ra, lc - la}};
  for (int edge = 0; edge < 4; ++edge) {
    cv::line(view, corners[edge], corners[(edge + 1) % 4], color, 2);
  }
  cv::line(view, lc, rc, color, 1);
  const auto center = (lc + rc) * 0.5f;
  cv::circle(view, center, 4, color, -1);
  const std::string label = std::to_string(pair.left) + "-" + std::to_string(pair.right) +
                            " diff " + number(pair.angle_difference);
  cv::putText(view, label, cv::Point(static_cast<int>(center.x) - 63, 300),
              cv::FONT_HERSHEY_SIMPLEX, 0.48, color, 1, cv::LINE_AA);
}

std::pair<cv::Mat, cv::Mat> drawPairs(const cv::Mat& image,
                                    const std::vector<DirectionInfo>& lights,
                                    const std::vector<PairInfo>& pairs) {
  cv::Mat nearby = image.clone();
  cv::Mat accepted = image.clone();
  for (const auto& pair : pairs) {
    if (pair.geometry_passed) drawPair(nearby, pair, lights, cv::Scalar(0, 230, 255));
    if (pair.accepted) drawPair(accepted, pair, lights, cv::Scalar(255, 220, 0));
  }
  return {nearby, accepted};
}

void putPanel(cv::Mat& canvas, const cv::Mat& panel, int column, int row,
              const std::string& title) {
  const int x = column * kWidth;
  const int y = row * kPanelHeight;
  cv::putText(canvas, title, cv::Point(x + 12, y + 23), cv::FONT_HERSHEY_SIMPLEX,
              0.52, cv::Scalar(235, 235, 235), 1, cv::LINE_AA);
  panel.copyTo(canvas(cv::Rect(x, y + kTitle, kWidth, kHeight)));
}

cv::Mat makeCanvas(const cv::Mat& image, const std::vector<DirectionInfo>& lights,
                   const std::vector<PairInfo>& pairs, int max_diff) {
  cv::Mat original = image.clone();
  cv::putText(original, "A: same tilt", cv::Point(40, 60), cv::FONT_HERSHEY_SIMPLEX,
              0.65, cv::Scalar(235, 235, 235), 1, cv::LINE_AA);
  cv::putText(original, "B: opposite tilts", cv::Point(365, 60), cv::FONT_HERSHEY_SIMPLEX,
              0.65, cv::Scalar(235, 235, 235), 1, cv::LINE_AA);
  cv::Mat canvas(2 * kPanelHeight + 60, 2 * kWidth, CV_8UC3, cv::Scalar(38, 38, 38));
  const auto views = drawPairs(image, lights, pairs);
  putPanel(canvas, original, 0, 0, "1 Original | two groups of red bars");
  putPanel(canvas, drawLights(image, lights), 1, 0, "2 Single lights | green boxes; signed tilt in degrees");
  putPanel(canvas, views.first, 0, 1, "3 Nearby pairs | length and position passed; angle unchecked");
  putPanel(canvas, views.second, 1, 1, "4 Accepted pairs | cyan outlines passed angle difference");
  const int footer = 2 * kPanelHeight;
  const std::string stats = "Lights: " + std::to_string(lightCount(lights)) +
      " | Nearby pairs: " + std::to_string(pairCount(pairs, true)) +
      " | Accepted pairs: " + std::to_string(pairCount(pairs)) +
      " | Max diff: " + std::to_string(max_diff) + " deg";
  cv::putText(canvas, stats, cv::Point(12, footer + 22), cv::FONT_HERSHEY_SIMPLEX,
              0.53, cv::Scalar(235, 235, 235), 1, cv::LINE_AA);
  cv::putText(canvas, "Single-light tilt <= 35 deg | Compare Max pair diff 10 and 55 | S: save | Q / Esc: quit",
              cv::Point(12, footer + 47), cv::FONT_HERSHEY_SIMPLEX, 0.48,
              cv::Scalar(235, 235, 235), 1, cv::LINE_AA);
  return canvas;
}

void logSettings(const std::vector<DirectionInfo>& lights,
                   const std::vector<PairInfo>& pairs, int max_diff) {
  std::cout << "Max pair diff=" << max_diff << " deg | lights=" << lightCount(lights)
            << " nearby_pairs=" << pairCount(pairs, true)
            << " accepted_pairs=" << pairCount(pairs) << std::endl;
}

void saveOutputs(const std::string& output, const cv::Mat& image,
                  const std::vector<DirectionInfo>& lights,
                  const std::vector<PairInfo>& pairs, const cv::Mat& canvas) {
  const auto views = drawPairs(image, lights, pairs);
  const std::vector<std::pair<std::string, cv::Mat>> files = {
      {"lights.png", drawLights(image, lights)}, {"nearby-pairs.png", views.first},
      {"accepted-pairs.png", views.second}, {"overview.png", canvas}};
  for (const auto& file : files) {
    if (!cv::imwrite(output + "/" + file.first, file.second)) {
      throw std::runtime_error("Could not save " + file.first);
    }
  }
  std::ofstream bars(output + "/lights.csv");
  std::ofstream pairs_file(output + "/pairs.csv");
  if (!bars || !pairs_file) throw std::runtime_error("Could not open CSV outputs");
  bars << "id,center_x,center_y,length,ratio,signed_tilt,tilt_degrees,accepted\n";
  bars << std::setprecision(10);
  for (std::size_t index = 0; index < lights.size(); ++index) {
    const auto& light = lights[index];
    bars << index << ',' << light.shape.rectangle.center.x << ','
         << light.shape.rectangle.center.y << ',' << light.shape.long_side << ','
         << light.shape.ratio << ',' << signedTilt(light) << ',' << light.tilt_degrees << ','
         << (light.accepted ? 1 : 0) << '\n';
  }
  pairs_file << "left,right,length_ratio,y_offset_ratio,separation_ratio,angle_difference,geometry_passed,accepted\n";
  pairs_file << std::setprecision(10);
  for (const auto& pair : pairs) {
    pairs_file << pair.left << ',' << pair.right << ',' << pair.length_ratio << ','
               << pair.y_offset_ratio << ',' << pair.separation_ratio << ','
               << pair.angle_difference << ',' << (pair.geometry_passed ? 1 : 0) << ','
               << (pair.accepted ? 1 : 0) << '\n';
  }
  bars.flush();
  pairs_file.flush();
  if (!bars || !pairs_file) throw std::runtime_error("Could not finish CSV outputs");
  std::cout << "Saved outputs: " << output << std::endl;
}
}  // namespace

int main(int argc, char** argv) {
  cv::CommandLineParser parser(argc, argv,
      "{help h ? |      | Show help}"
      "{image    |      | Input 640x360 image}"
      "{output   |.     | Existing output directory}"
      "{max-diff |10    | Maximum pair angle difference, 0..90 degrees}"
      "{headless |false | Save outputs without opening a window}");
  if (parser.has("help")) { parser.printMessage(); return 0; }
  const auto input = parser.get<std::string>("image");
  const auto output = parser.get<std::string>("output");
  int max_diff = parser.get<int>("max-diff");
  const bool headless = parser.get<bool>("headless");
  if (!parser.check()) { parser.printErrors(); return 2; }
  if (input.empty() || max_diff < 0 || max_diff > 90) {
    std::cerr << "Invalid input or max-diff. Run with --help.\n";
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
    const auto lights = analyzeDirections(selectHsv(hsv, HsvSettings{}), 35.0);
    auto pairs = pairLights(lights, max_diff);
    auto canvas = makeCanvas(image, lights, pairs, max_diff);
    std::cout << "OpenCV " << CV_VERSION << " | light pairing lesson" << std::endl;
    logSettings(lights, pairs, max_diff);
    saveOutputs(output, image, lights, pairs, canvas);
    if (headless) return 0;

    cv::namedWindow(kWindow, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("Max pair diff", kWindow, &max_diff, 90);
    std::cout << "PAIRING GUI READY | Compare Max pair diff 10 and 55 degrees." << std::endl;
    int previous = max_diff;
    while (true) {
      max_diff = cv::getTrackbarPos("Max pair diff", kWindow);
      if (max_diff != previous) {
        pairs = pairLights(lights, max_diff);
        logSettings(lights, pairs, max_diff);
        previous = max_diff;
      }
      canvas = makeCanvas(image, lights, pairs, max_diff);
      cv::imshow(kWindow, canvas);
      const int key = cv::waitKey(30) & 0xff;
      if (key == 'q' || key == 'Q' || key == 27) break;
      if (key == 's' || key == 'S') saveOutputs(output, image, lights, pairs, canvas);
      try {
        if (cv::getWindowProperty(kWindow, cv::WND_PROP_AUTOSIZE) < 0) break;
      } catch (const cv::Exception&) { break; }
    }
    cv::destroyAllWindows();
    return 0;
  } catch (const std::exception& error) {
    std::cerr << "Pairing lesson failed: " << error.what() << std::endl;
    return 1;
  }
}
