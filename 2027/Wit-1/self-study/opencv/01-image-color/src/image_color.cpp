#include <opencv2/opencv.hpp>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {
constexpr int kImageWidth = 640;
constexpr int kImageHeight = 360;
constexpr int kTitleHeight = 32;
constexpr int kPanelHeight = kImageHeight + kTitleHeight;
const char* const kWindow = "OpenCV 01 - pixels and color";

struct FilterResult {
  cv::Mat red_minus_blue;
  cv::Mat red_minus_green;
  cv::Mat mask_rb;
  cv::Mat mask_rg;
  cv::Mat mask;
  cv::Mat selected;
};

// Compare the original R-B rule with the intersection of two color rules.
FilterResult filterRed(const cv::Mat& image, int threshold, int green_threshold,
                       bool use_green) {
  std::vector<cv::Mat> channels;
  cv::split(image, channels);  // B = 0, G = 1, R = 2.

  FilterResult result;
  cv::subtract(channels[2], channels[0], result.red_minus_blue);
  cv::subtract(channels[2], channels[1], result.red_minus_green);
  cv::threshold(result.red_minus_blue, result.mask_rb, threshold, 255,
                cv::THRESH_BINARY);
  cv::threshold(result.red_minus_green, result.mask_rg, green_threshold, 255,
                cv::THRESH_BINARY);
  if (use_green) {
    cv::bitwise_and(result.mask_rb, result.mask_rg, result.mask);
  } else {
    result.mask = result.mask_rb.clone();
  }
  result.selected = cv::Mat::zeros(image.size(), image.type());
  image.copyTo(result.selected, result.mask);
  return result;
}

struct Selection {
  const cv::Mat* image = nullptr;
  int x = -1;
  int y = -1;
  std::string text = "Click a colored area in the Original panel to inspect BGR.";
};

void onMouse(int event, int x, int y, int, void* data) {
  if (event != cv::EVENT_LBUTTONDOWN) return;
  auto& selection = *static_cast<Selection*>(data);
  const int image_y = y - kTitleHeight;
  if (x < 0 || x >= kImageWidth || image_y < 0 || image_y >= kImageHeight) return;

  selection.x = x;
  selection.y = image_y;
  const auto pixel = selection.image->at<cv::Vec3b>(image_y, x);
  const int difference = std::max(0, static_cast<int>(pixel[2]) - pixel[0]);
  const int green_difference = std::max(0, static_cast<int>(pixel[2]) - pixel[1]);
  selection.text = "x=" + std::to_string(x) + " y=" + std::to_string(image_y) +
                   "  B=" + std::to_string(pixel[0]) +
                   " G=" + std::to_string(pixel[1]) +
                   " R=" + std::to_string(pixel[2]) +
                   "  max(R-B,0)=" + std::to_string(difference) +
                   " max(R-G,0)=" + std::to_string(green_difference);
  std::cout << selection.text << std::endl;
}

void putPanel(cv::Mat& canvas, const cv::Mat& image, int column, int row,
              const std::string& title) {
  const int x = column * kImageWidth;
  const int y = row * kPanelHeight;
  cv::putText(canvas, title, cv::Point(x + 12, y + 23),
              cv::FONT_HERSHEY_SIMPLEX, 0.57, cv::Scalar(235, 235, 235), 1,
              cv::LINE_AA);
  const cv::Rect destination(x, y + kTitleHeight, kImageWidth, kImageHeight);
  if (image.channels() == 1) {
    cv::Mat color;
    cv::cvtColor(image, color, cv::COLOR_GRAY2BGR);
    color.copyTo(canvas(destination));
  } else {
    image.copyTo(canvas(destination));
  }
}

cv::Mat makeCanvas(const cv::Mat& image, const FilterResult& result,
                   int threshold, int green_threshold, bool use_green,
                   const Selection& selection) {
  cv::Mat canvas(2 * kPanelHeight + 60, 2 * kImageWidth, CV_8UC3,
                 cv::Scalar(38, 38, 38));
  putPanel(canvas, image, 0, 0, "1 Original | color order: B, G, R");
  putPanel(canvas, use_green ? result.red_minus_green : result.red_minus_blue, 1, 0,
           use_green ? "2 Difference | max(R - G, 0)" : "2 Difference | max(R - B, 0)");
  std::string rule = "R-B > " + std::to_string(threshold);
  if (use_green) rule += " AND R-G > " + std::to_string(green_threshold);
  putPanel(canvas, result.mask, 0, 1,
           "3 Mask | " + rule);
  putPanel(canvas, result.selected, 1, 1, "4 Selected pixels | original colors");
  if (selection.x >= 0) {
    cv::drawMarker(canvas, cv::Point(selection.x, selection.y + kTitleHeight),
                   cv::Scalar(255, 255, 255), cv::MARKER_CROSS, 12, 1);
  }
  const int footer_y = 2 * kPanelHeight;
  cv::putText(canvas, selection.text, cv::Point(12, footer_y + 22),
              cv::FONT_HERSHEY_SIMPLEX, 0.52, cv::Scalar(235, 235, 235), 1,
              cv::LINE_AA);
  const std::string status =
      "Selected: " + std::to_string(cv::countNonZero(result.mask)) +
      " pixels | Use G=" + std::to_string(use_green ? 1 : 0) +
      " | G: toggle green rule | S: save | Q / Esc: quit";
  cv::putText(canvas, status, cv::Point(12, footer_y + 47),
              cv::FONT_HERSHEY_SIMPLEX, 0.52, cv::Scalar(235, 235, 235), 1,
              cv::LINE_AA);
  return canvas;
}

void saveOutputs(const std::string& directory, const FilterResult& result,
                 const cv::Mat& canvas) {
  const std::vector<std::pair<std::string, cv::Mat>> files = {
      {"difference.png", result.red_minus_blue},
      {"difference_rg.png", result.red_minus_green},
      {"mask_rb.png", result.mask_rb}, {"mask_rg.png", result.mask_rg},
      {"mask.png", result.mask},
      {"selected.png", result.selected}, {"overview.png", canvas}};
  for (const auto& file : files) {
    if (!cv::imwrite(directory + "/" + file.first, file.second)) {
      throw std::runtime_error("Could not save " + file.first);
    }
  }
  std::cout << "Saved outputs: " << directory << std::endl;
}
}  // namespace

int main(int argc, char** argv) {
  const char* keys =
      "{help h ? |      | Show help}"
      "{image    |      | Input color image}"
      "{output   |.     | Existing output directory}"
      "{threshold|80    | Difference threshold, from 0 to 255}"
      "{green-threshold|50 | Threshold for R-G, from 0 to 255}"
      "{use-green|false | Also require R-G to exceed its threshold}"
      "{headless |false | Save outputs without opening a window}"
      "{preview-ms|0    | Close the GUI after this many milliseconds; 0 waits}"
      ;
  cv::CommandLineParser parser(argc, argv, keys);
  if (parser.has("help")) {
    parser.printMessage();
    return 0;
  }
  const auto image_path = parser.get<std::string>("image");
  const auto output = parser.get<std::string>("output");
  int threshold = parser.get<int>("threshold");
  int green_threshold = parser.get<int>("green-threshold");
  int use_green = parser.get<bool>("use-green") ? 1 : 0;
  const bool headless = parser.get<bool>("headless");
  const int preview_ms = parser.get<int>("preview-ms");
  if (!parser.check()) {
    parser.printErrors();
    return 2;
  }
  if (image_path.empty() || threshold < 0 || threshold > 255 ||
      green_threshold < 0 || green_threshold > 255 || preview_ms < 0) {
    std::cerr << "Provide --image=FILE; thresholds must be 0..255 and preview-ms >= 0.\n";
    return 2;
  }

  try {
    const cv::Mat image = cv::imread(image_path, cv::IMREAD_COLOR);
    if (image.empty()) {
      std::cerr << "Could not read image: " << image_path << std::endl;
      return 1;
    }
    // Fixed-size teaching panels keep image coordinates equal to mouse coordinates.
    if (image.cols != kImageWidth || image.rows != kImageHeight) {
      std::cerr << "This first lesson expects a 640x360 image.\n";
      return 2;
    }
    std::cout << "OpenCV " << CV_VERSION << " | image: " << image.cols << "x"
              << image.rows << " | channels: " << image.channels() << std::endl;
    std::cout << "Image coordinates: x goes right, y goes down; BGR values are 0..255."
              << std::endl;
    Selection selection;
    selection.image = &image;
    auto result = filterRed(image, threshold, green_threshold, use_green != 0);
    auto canvas = makeCanvas(image, result, threshold, green_threshold,
                             use_green != 0, selection);
    std::cout << "Threshold=" << threshold << " green-threshold=" << green_threshold
              << " use-green=" << use_green << " selected="
              << cv::countNonZero(result.mask) << std::endl;
    saveOutputs(output, result, canvas);
    if (headless) return 0;

    cv::namedWindow(kWindow, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("Min R-B", kWindow, &threshold, 255);
    cv::createTrackbar("Min R-G", kWindow, &green_threshold, 255);
    cv::createTrackbar("Use G (0/1)", kWindow, &use_green, 1);
    cv::setMouseCallback(kWindow, onMouse, &selection);
    std::cout << "GUI READY | Drag Min R-B / Min R-G. Use G=0: original; 1: both rules."
              << std::endl;
    const auto start = std::chrono::steady_clock::now();
    int previous_threshold = -1;
    int previous_green_threshold = -1;
    int previous_use_green = -1;
    while (true) {
      threshold = cv::getTrackbarPos("Min R-B", kWindow);
      green_threshold = cv::getTrackbarPos("Min R-G", kWindow);
      use_green = cv::getTrackbarPos("Use G (0/1)", kWindow);
      if (threshold != previous_threshold || green_threshold != previous_green_threshold ||
          use_green != previous_use_green) {
        result = filterRed(image, threshold, green_threshold, use_green != 0);
        std::cout << "Threshold=" << threshold << " green-threshold=" << green_threshold
                  << " use-green=" << use_green << " selected="
                  << cv::countNonZero(result.mask) << std::endl;
        previous_threshold = threshold;
        previous_green_threshold = green_threshold;
        previous_use_green = use_green;
      }
      canvas = makeCanvas(image, result, threshold, green_threshold,
                           use_green != 0, selection);
      cv::imshow(kWindow, canvas);
      const int key = cv::waitKey(30) & 0xff;
      if (key == 'q' || key == 'Q' || key == 27) break;
      if (key == 's' || key == 'S') saveOutputs(output, result, canvas);
      if (key == 'g' || key == 'G') {
        cv::setTrackbarPos("Use G (0/1)", kWindow, use_green == 0 ? 1 : 0);
      }
      if (preview_ms > 0 &&
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::steady_clock::now() - start).count() >= preview_ms) break;
      try {
        // OpenCV 4.2's GTK backend supports AUTOSIZE but not VISIBLE.
        if (cv::getWindowProperty(kWindow, cv::WND_PROP_AUTOSIZE) < 0) break;
      } catch (const cv::Exception&) {
        break;  // Some GUI backends remove the window immediately on close.
      }
    }
    cv::destroyAllWindows();
    return 0;
  } catch (const std::exception& error) {
    std::cerr << "OpenCV lesson failed: " << error.what() << std::endl;
    return 1;
  }
}
