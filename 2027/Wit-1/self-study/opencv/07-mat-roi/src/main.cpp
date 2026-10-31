#include "course.hpp"
namespace lab {
void runMat(const Options& options) {
  auto image = readImage(options.input.empty() ? options.assets + "/scene.png" : options.input);
  if (image.cols < 160 || image.rows < 120) throw std::runtime_error("Image must be at least 160x120");
  const auto before = image.clone();
  cv::Mat shallow = image;
  cv::Mat deep = image.clone();
  const cv::Rect rectangle(20, 20, 100, 80);
  cv::Mat roi = image(rectangle);
  roi.setTo(cv::Scalar(0, 220, 0));
  const bool shared = shallow.data == image.data;
  const bool independent = deep.data != image.data;
  const double changed = cv::norm(image, before, cv::NORM_L1);
  staticResult(options, "07 Mat and ROI", {
    {{"Before modification", before}, {"Parent after ROI.setTo", image},
     {"Shallow copy: shares pixels", shallow}, {"clone: independent pixels", deep}},
    {{"shallow_shares_data", shared ? 1.0 : 0.0}, {"clone_independent", independent ? 1.0 : 0.0},
     {"parent_changed_L1", changed}, {"clone_changed_L1", cv::norm(deep, before, cv::NORM_L1)},
     {"roi_continuous", roi.isContinuous() ? 1.0 : 0.0}, {"width", image.cols}, {"height", image.rows}}});
}
}  // namespace lab
