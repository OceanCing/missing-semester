#include "hsv_filter.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {
constexpr int kWidth = 640;
constexpr int kHeight = 360;
constexpr int kTitle = 32;
constexpr int kPanelHeight = kHeight + kTitle;
const char* const kWindow = "OpenCV 02 - HSV color selection";

struct Selection {
  const cv::Mat* image = nullptr;
  const cv::Mat* hsv = nullptr;
  std::string text = "Click Original to inspect a pixel's BGR and HSV values.";
};

void onMouse(int event, int x, int y, int, void* data) {
  if (event != cv::EVENT_LBUTTONDOWN) return;
  y -= kTitle;
  if (x < 0 || x >= kWidth || y < 0 || y >= kHeight) return;
  auto& selection = *static_cast<Selection*>(data);
  const auto bgr = selection.image->at<cv::Vec3b>(y, x);
  const auto hsv = selection.hsv->at<cv::Vec3b>(y, x);
  selection.text = "x=" + std::to_string(x) + " y=" + std::to_string(y) +
      "  B=" + std::to_string(bgr[0]) + " G=" + std::to_string(bgr[1]) +
      " R=" + std::to_string(bgr[2]) + " | H=" + std::to_string(hsv[0]) +
      " S=" + std::to_string(hsv[1]) + " V=" + std::to_string(hsv[2]);
  std::cout << selection.text << std::endl;
}

std::string describeHue(const HsvSettings& settings) {
  std::string text;
  for (const auto& range : hueRanges(settings)) {
    if (!text.empty()) text += " OR ";
    text += "[" + std::to_string(range.first) + "," +
            std::to_string(range.second) + "]";
  }
  return text;
}

cv::Mat makeGuide(const HsvSettings& settings) {
  cv::Mat guide(kHeight, kWidth, CV_8UC3, cv::Scalar(32, 32, 32));
  cv::Mat hue_line(1, 180, CV_8UC3);
  for (int h = 0; h < 180; ++h) {
    hue_line.at<cv::Vec3b>(0, h) = cv::Vec3b(static_cast<unsigned char>(h), 255, 255);
  }
  cv::Mat strip;
  cv::cvtColor(hue_line, strip, cv::COLOR_HSV2BGR);
  cv::resize(strip, strip, cv::Size(600, 48), 0, 0, cv::INTER_NEAREST);
  strip.copyTo(guide(cv::Rect(20, 40, 600, 48)));
  for (const auto& range : hueRanges(settings)) {
    const int left = 20 + range.first * 600 / 180;
    const int right = 20 + (range.second + 1) * 600 / 180;
    cv::rectangle(guide, cv::Rect(left, 38, right - left, 52),
                   cv::Scalar(255, 255, 255), 2);
  }
  const cv::Scalar text_color(230, 230, 230);
  for (const int hue : {0, 30, 60, 90, 120, 150, 179}) {
    const int x = 20 + hue * 600 / 180 - (hue == 179 ? 28 : 0);
    cv::putText(guide, std::to_string(hue), cv::Point(x, 115),
                cv::FONT_HERSHEY_SIMPLEX, 0.48, text_color, 1, cv::LINE_AA);
  }
  const std::vector<std::string> lines = {
      "White outline: allowed hue ranges",
      "H in " + describeHue(settings),
      "S >= " + std::to_string(settings.saturation_min) + "  (reject gray / white)",
      "V >= " + std::to_string(settings.value_min) + "  (reject dark pixels)",
      "Try H center: red 0, yellow 30, blue 120",
      "Both ends of this hue strip are close to red."};
  int y = 150;
  for (const auto& line : lines) {
    cv::putText(guide, line, cv::Point(20, y), cv::FONT_HERSHEY_SIMPLEX,
                0.52, text_color, 1, cv::LINE_AA);
    y += 32;
  }
  return guide;
}

void putPanel(cv::Mat& canvas, const cv::Mat& image, int column, int row,
              const std::string& title) {
  const int x = column * kWidth;
  const int y = row * kPanelHeight;
  cv::putText(canvas, title, cv::Point(x + 12, y + 23),
              cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(235, 235, 235), 1,
              cv::LINE_AA);
  cv::Mat display;
  if (image.channels() == 1) cv::cvtColor(image, display, cv::COLOR_GRAY2BGR);
  else display = image;
  display.copyTo(canvas(cv::Rect(x, y + kTitle, kWidth, kHeight)));
}

cv::Mat makeCanvas(const cv::Mat& image, const cv::Mat& mask,
                   const HsvSettings& settings, const Selection& selection) {
  cv::Mat canvas(2 * kPanelHeight + 60, 2 * kWidth, CV_8UC3, cv::Scalar(38, 38, 38));
  cv::Mat selected = cv::Mat::zeros(image.size(), image.type());
  image.copyTo(selected, mask);
  putPanel(canvas, image, 0, 0, "1 Original | click for BGR and HSV");
  putPanel(canvas, makeGuide(settings), 1, 0, "2 HSV guide | hue is circular");
  putPanel(canvas, mask, 0, 1, "3 Mask | hue range AND S/V limits");
  putPanel(canvas, selected, 1, 1, "4 Selected pixels | original colors");
  const int footer = 2 * kPanelHeight;
  cv::putText(canvas, selection.text, cv::Point(12, footer + 22),
              cv::FONT_HERSHEY_SIMPLEX, 0.50, cv::Scalar(235, 235, 235), 1,
              cv::LINE_AA);
  cv::putText(canvas, "Selected: " + std::to_string(cv::countNonZero(mask)) +
              " pixels | Drag HSV sliders | S: save | Q / Esc: quit",
              cv::Point(12, footer + 47), cv::FONT_HERSHEY_SIMPLEX, 0.50,
              cv::Scalar(235, 235, 235), 1, cv::LINE_AA);
  return canvas;
}

void saveOutputs(const std::string& output, const cv::Mat& image,
                  const cv::Mat& hsv, const cv::Mat& mask, const cv::Mat& canvas) {
  std::vector<cv::Mat> channels;
  cv::split(hsv, channels);
  cv::Mat selected = cv::Mat::zeros(image.size(), image.type());
  image.copyTo(selected, mask);
  const std::vector<std::pair<std::string, cv::Mat>> files = {
      {"hue.png", channels[0]}, {"saturation.png", channels[1]},
      {"value.png", channels[2]}, {"mask.png", mask},
      {"selected.png", selected}, {"overview.png", canvas}};
  for (const auto& file : files) {
    if (!cv::imwrite(output + "/" + file.first, file.second)) {
      throw std::runtime_error("Could not save " + file.first);
    }
  }
  std::cout << "Saved outputs: " << output << std::endl;
}

void logSettings(const HsvSettings& settings, const cv::Mat& mask) {
  std::cout << "H=" << settings.hue_center << " radius=" << settings.hue_radius
            << " S>=" << settings.saturation_min << " V>=" << settings.value_min
            << " selected=" << cv::countNonZero(mask) << std::endl;
}
}  // namespace

int main(int argc, char** argv) {
  cv::CommandLineParser parser(argc, argv,
      "{help h ? |      | Show help}"
      "{image    |      | Input 640x360 color image}"
      "{output   |.     | Existing output directory}"
      "{h-center |0     | Hue center, 0..179}"
      "{h-radius |10    | Hue radius, 0..40}"
      "{s-min    |100   | Minimum saturation, inclusive, 0..255}"
      "{v-min    |50    | Minimum value, inclusive, 0..255}"
      "{headless |false | Save outputs without opening a window}");
  if (parser.has("help")) { parser.printMessage(); return 0; }
  const auto input = parser.get<std::string>("image");
  const auto output = parser.get<std::string>("output");
  const bool headless = parser.get<bool>("headless");
  HsvSettings settings;
  settings.hue_center = parser.get<int>("h-center");
  settings.hue_radius = parser.get<int>("h-radius");
  settings.saturation_min = parser.get<int>("s-min");
  settings.value_min = parser.get<int>("v-min");
  if (!parser.check()) { parser.printErrors(); return 2; }
  if (input.empty() || settings.hue_center < 0 || settings.hue_center > 179 ||
      settings.hue_radius < 0 || settings.hue_radius > 40 ||
      settings.saturation_min < 0 || settings.saturation_min > 255 ||
      settings.value_min < 0 || settings.value_min > 255) {
    std::cerr << "Invalid image or HSV settings. Run with --help for ranges.\n";
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
    Selection selection;
    selection.image = &image;
    selection.hsv = &hsv;
    auto mask = selectHsv(hsv, settings);
    auto canvas = makeCanvas(image, mask, settings, selection);
    std::cout << "OpenCV " << CV_VERSION << " | HSV lesson" << std::endl;
    logSettings(settings, mask);
    saveOutputs(output, image, hsv, mask, canvas);
    if (headless) return 0;

    cv::namedWindow(kWindow, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("H center", kWindow, &settings.hue_center, 179);
    cv::createTrackbar("H radius", kWindow, &settings.hue_radius, 40);
    cv::createTrackbar("S min", kWindow, &settings.saturation_min, 255);
    cv::createTrackbar("V min", kWindow, &settings.value_min, 255);
    cv::setMouseCallback(kWindow, onMouse, &selection);
    std::cout << "HSV GUI READY | Click Original, then vary V min. Q/Esc quits."
              << std::endl;
    auto previous = settings;
    while (true) {
      settings.hue_center = cv::getTrackbarPos("H center", kWindow);
      settings.hue_radius = cv::getTrackbarPos("H radius", kWindow);
      settings.saturation_min = cv::getTrackbarPos("S min", kWindow);
      settings.value_min = cv::getTrackbarPos("V min", kWindow);
      if (settings.hue_center != previous.hue_center ||
          settings.hue_radius != previous.hue_radius ||
          settings.saturation_min != previous.saturation_min ||
          settings.value_min != previous.value_min) {
        mask = selectHsv(hsv, settings);
        logSettings(settings, mask);
        previous = settings;
      }
      canvas = makeCanvas(image, mask, settings, selection);
      cv::imshow(kWindow, canvas);
      const int key = cv::waitKey(30) & 0xff;
      if (key == 'q' || key == 'Q' || key == 27) break;
      if (key == 's' || key == 'S') saveOutputs(output, image, hsv, mask, canvas);
      try {
        if (cv::getWindowProperty(kWindow, cv::WND_PROP_AUTOSIZE) < 0) break;
      } catch (const cv::Exception&) {
        break;
      }
    }
    cv::destroyAllWindows();
    return 0;
  } catch (const std::exception& error) {
    std::cerr << "HSV lesson failed: " << error.what() << std::endl;
    return 1;
  }
}
