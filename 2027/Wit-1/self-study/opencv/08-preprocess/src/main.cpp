#include "course.hpp"
namespace lab {
void runPreprocess(const Options& options) {
  const auto variant = options.variant.empty() ? "morph" : options.variant;
  if (variant == "threshold") {
    const auto gray = readImage(options.assets + "/gradient.png", cv::IMREAD_GRAYSCALE);
    interactive(options, "08 Thresholds", "Global threshold", 127, 255, [&](int value) {
      cv::Mat global, otsu, adaptive;
      cv::threshold(gray, global, value, 255, cv::THRESH_BINARY);
      const auto automatic = cv::threshold(gray, otsu, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
      cv::adaptiveThreshold(gray, adaptive, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 31, 7);
      return Result{{{"Uneven illumination", gray}, {"Global threshold", global},
                      {"Otsu: one global threshold", otsu}, {"Adaptive: local thresholds", adaptive}},
                     {{"otsu_threshold", automatic}, {"global_white", cv::countNonZero(global)}}};
    });
  } else if (variant == "blur") {
    auto clean = readImage(options.assets + "/scene.png", cv::IMREAD_GRAYSCALE);
    auto noisy = clean.clone();
    cv::RNG rng(42);
    for (int index = 0; index < 6000; ++index)
      noisy.at<uchar>(rng.uniform(0, noisy.rows), rng.uniform(0, noisy.cols)) = index % 2 ? 255 : 0;
    interactive(options, "08 Filters", "Kernel radius", 1, 7, [&](int radius) {
      const int size = 2 * radius + 1;
      cv::Mat gaussian, median;
      cv::GaussianBlur(noisy, gaussian, {size, size}, 0);
      if (size == 1) median = noisy.clone(); else cv::medianBlur(noisy, median, size);
      return Result{{{"Clean reference", clean}, {"Salt and pepper noise", noisy},
                      {"Gaussian blur", gaussian}, {"Median blur", median}},
                     {{"kernel_size", size}, {"noisy_L1", cv::norm(noisy, clean, cv::NORM_L1)},
                      {"median_L1", cv::norm(median, clean, cv::NORM_L1)}}};
    });
  } else if (variant == "morph") {
    const auto raw = readImage(options.assets + "/broken-mask.png", cv::IMREAD_GRAYSCALE);
    interactive(options, "08 Morphology", "Kernel radius", 1, 16, [&](int radius) {
      const auto kernel = cv::getStructuringElement(cv::MORPH_RECT, {2 * radius + 1, 2 * radius + 1});
      cv::Mat opened, closed_mask, combined;
      cv::morphologyEx(raw, opened, cv::MORPH_OPEN, kernel);
      cv::morphologyEx(raw, closed_mask, cv::MORPH_CLOSE, kernel);
      cv::morphologyEx(opened, combined, cv::MORPH_CLOSE, kernel);
      cv::Mat labels;
      const int raw_components = cv::connectedComponents(raw, labels) - 1;
      const int cleaned_components = cv::connectedComponents(combined, labels) - 1;
      return Result{{{"Raw: isolated noise and a broken bar", raw}, {"Open: remove small white regions", opened},
                      {"Close: bridge narrow dark gaps", closed_mask}, {"Open then close", combined}},
                     {{"kernel_size", 2 * radius + 1}, {"raw_components", raw_components},
                      {"cleaned_components", cleaned_components},
                      {"noise_after_open", opened.at<uchar>(50, 50)},
                      {"gap_after_close", closed_mask.at<uchar>(201, 260)}}};
    });
  } else throw std::runtime_error("variant must be morph, blur or threshold");
}
}  // namespace lab
