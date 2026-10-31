#include "course.hpp"
namespace lab {
void runWarp(const Options& options) {
  const auto original = readImage(options.assets + "/pattern.png");
  const std::vector<cv::Point2f> flat{{0, 0}, {159, 0}, {159, 79}, {0, 79}};
  const std::vector<cv::Point2f> quad{{180, 110}, {430, 150}, {450, 340}, {120, 290}};
  cv::Mat scene, restored, wrong;
  cv::warpPerspective(original, scene, cv::getPerspectiveTransform(flat, quad), {640, 480});
  cv::warpPerspective(scene, restored, cv::getPerspectiveTransform(quad, flat), original.size());
  auto incorrect = quad; std::swap(incorrect[1], incorrect[2]);
  cv::warpPerspective(scene, wrong, cv::getPerspectiveTransform(incorrect, flat), original.size());
  const double denominator = static_cast<double>(original.total() * original.channels());
  const double correct_error = cv::norm(original, restored, cv::NORM_L1) / denominator;
  const double wrong_error = cv::norm(original, wrong, cv::NORM_L1) / denominator;
  auto labelled = scene.clone();
  for (std::size_t index = 0; index < quad.size(); ++index) {
    cv::circle(labelled, quad[index], 5, {0, 255, 0}, -1);
    cv::putText(labelled, std::to_string(index), quad[index] + cv::Point2f(5, -8),
                cv::FONT_HERSHEY_SIMPLEX, 0.7, {0, 255, 0}, 2);
  }
  staticResult(options, "10 Perspective ROI", {
    {{"Known order: TL, TR, BR, BL", labelled}, {"Restored 160x80 ROI", restored},
     {"Incorrect order: TR and BR swapped", wrong}, {"Original rectangular pattern", original}},
    {{"correct_mean_absolute_error", correct_error}, {"wrong_mean_absolute_error", wrong_error},
     {"roi_width", restored.cols}, {"roi_height", restored.rows}}});
}
}  // namespace lab
