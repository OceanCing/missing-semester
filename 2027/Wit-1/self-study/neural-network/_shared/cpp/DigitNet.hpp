#pragma once
#include "course.hpp"
#include <opencv2/dnn.hpp>
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <string>
#include <cstdio>
#include <stdexcept>

namespace nncourse {
struct Classification {
  int id=-1; float score=0; bool accepted=false;
  std::string name; std::vector<float> logits;
};
class DigitNet {
  cv::dnn::Net network_;
  std::vector<std::string> names_;
public:
  explicit DigitNet(const std::string& folder) {
    cv::FileStorage metadata(folder+"/metadata.json",cv::FileStorage::READ);
    if (!metadata.isOpened()) throw std::runtime_error("Cannot read model metadata");
    for (const auto& item:metadata["classes"]) names_.push_back(static_cast<std::string>(item));
    if (names_.size()!=11 || names_.back()!="negative") throw std::runtime_error("Unexpected class map");
    network_=cv::dnn::readNetFromONNX(folder+"/baseline.onnx");
    network_.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    network_.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
  }
  Classification infer(const cv::Mat& image,bool roi=false,float threshold=.65f,const std::string& mode="normal") {
    if (image.empty()) throw std::runtime_error("No input pixels");
    cv::Mat view=image, gray;
    if (roi) view=image(cv::Rect(image.cols/4,0,image.cols/2,image.rows));
    if (view.channels()==3) cv::cvtColor(view,gray,cv::COLOR_BGR2GRAY); else gray=view.clone();
    cv::resize(gray,gray,{32,32},0,0,cv::INTER_AREA);
    if (mode=="invert") cv::bitwise_not(gray,gray);
    network_.setInput(cv::dnn::blobFromImage(gray,mode=="raw"?1.:1./255.,{32,32}));
    cv::Mat output=network_.forward().reshape(1,1);
    if (output.total()!=names_.size()) throw std::runtime_error("Model output/class map mismatch");
    Classification result;
    result.logits.assign(output.ptr<float>(),output.ptr<float>()+output.total());
    for(float value:result.logits) if(!std::isfinite(value)) throw std::runtime_error("Non-finite model output");
    result.id=static_cast<int>(std::max_element(result.logits.begin(),result.logits.end())-result.logits.begin());
    const float maximum=result.logits[result.id];
    float denominator=0;
    for(float value:result.logits) denominator+=std::exp(value-maximum);
    result.score=1.f/denominator;
    result.name=names_[result.id];
    result.accepted=result.id!=10 && result.score>=threshold;
    return result;
  }
};
struct Prediction {
  bool roi_valid=false,has_pose=false;
  Classification classification;
  cv::Mat roi,annotated,tvec;
};
inline Prediction process(const cv::Mat& image,DigitNet& net,const lab::DetectorConfig& config,float threshold=.65f) {
  Prediction output;
  const auto detection=lab::detect(image,config);
  output.annotated=detection.annotated;
  output.roi=cv::Mat(80,160,CV_8UC3,cv::Scalar(20,20,20));
  output.roi_valid=lab::countPairs(detection)==1;
  if (output.roi_valid) {
    const std::vector<cv::Point2f> flat{{0,0},{159,0},{159,79},{0,79}};
    cv::warpPerspective(image,output.roi,cv::getPerspectiveTransform(detection.corners[0],flat),{160,80});
    output.classification=net.infer(output.roi,true,threshold);
    cv::Mat rotation;
    output.has_pose=cv::solvePnP(lab::objectCorners(),detection.corners[0],lab::cameraMatrix(),cv::Mat(),rotation,output.tvec);
    output.has_pose=output.has_pose && cv::checkRange(output.tvec) && output.tvec.at<double>(2)>0;
    const auto& c=output.classification;
    std::string text=c.accepted?"digit="+c.name:"CLASS REJECTED";
    text+=" score="+std::to_string(c.score).substr(0,4);
    cv::putText(output.annotated,text,{20,32},cv::FONT_HERSHEY_SIMPLEX,.65,{230,230,230},1);
    if(output.has_pose) cv::putText(output.annotated,"Z="+std::to_string(output.tvec.at<double>(2)).substr(0,5)+"m",
                                  {20,62},cv::FONT_HERSHEY_SIMPLEX,.6,{230,230,230},1);
  } else cv::putText(output.annotated,"NO UNIQUE ROI",{20,32},cv::FONT_HERSHEY_SIMPLEX,.65,{0,230,255},1);
  return output;
}
}
