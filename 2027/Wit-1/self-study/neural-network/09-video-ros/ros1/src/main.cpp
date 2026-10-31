#include "DigitNet.hpp"
#include <digit_classifier_demo/DigitPrediction.h>
#include <cv_bridge/cv_bridge.h>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <fstream>
#include <iostream>

int main(int argc,char** argv){
  ros::init(argc,argv,"digit_demo",ros::init_options::AnonymousName);
  cv::setNumThreads(1);
  cv::CommandLineParser parser(argc,argv,"{mode|process|}{models||}{assets||}{output||}{config||}{headless|false|}");
  if(!parser.check())return 2;
  try{
    ros::NodeHandle nh;
    if(parser.get<std::string>("mode")=="publish"){
      auto pub=nh.advertise<sensor_msgs::Image>("/digit_course/image_raw",1);
      ros::Rate rate(20);unsigned int sequence=0;
      while(ros::ok()){
        char name[40];std::snprintf(name,sizeof(name),"/frames/frame-%03u.png",sequence%180);
        const auto image=cv::imread(parser.get<std::string>("assets")+name);
        if(image.empty())throw std::runtime_error("Missing generated frame");
        std_msgs::Header header;header.seq=sequence++;header.stamp=ros::Time::now();header.frame_id="camera_optical_frame";
        pub.publish(cv_bridge::CvImage(header,"bgr8",image).toImageMsg());ros::spinOnce();rate.sleep();
      }
      return 0;
    }
    nncourse::DigitNet network(parser.get<std::string>("models"));
    const auto config=lab::loadConfig(parser.get<std::string>("config"));
    const bool headless=parser.get<bool>("headless");
    auto pub=nh.advertise<digit_classifier_demo::DigitPrediction>("/digit_course/prediction",1);
    auto debug=nh.advertise<sensor_msgs::Image>("/digit_course/image_debug",1);
    int received=0;bool failed=false;
    auto sub=nh.subscribe<sensor_msgs::Image>("/digit_course/image_raw",1,[&](const sensor_msgs::ImageConstPtr& message){
      try{
        const auto view=cv_bridge::toCvShare(message,"bgr8");
        const auto result=nncourse::process(view->image,network,config);
        digit_classifier_demo::DigitPrediction prediction;
        prediction.header=message->header;prediction.detected=result.roi_valid;
        prediction.classified=result.roi_valid&&result.classification.accepted;
        prediction.class_id=prediction.classified?result.classification.id:-1;
        prediction.class_name=prediction.classified?result.classification.name:"";
        prediction.confidence=result.classification.score;prediction.has_pose=result.has_pose;
        if(result.has_pose){prediction.position.x=result.tvec.at<double>(0);prediction.position.y=result.tvec.at<double>(1);prediction.position.z=result.tvec.at<double>(2);}
        pub.publish(prediction);debug.publish(cv_bridge::CvImage(message->header,"bgr8",result.annotated).toImageMsg());
        ++received;ROS_INFO_THROTTLE(1,"received=%d detected=%d class=%s score=%.3f",received,result.roi_valid,prediction.class_name.c_str(),prediction.confidence);
        if(!headless){cv::imshow("09 ROS1 digit callback",result.annotated);int key=cv::waitKey(1)&255;if(key=='q'||key==27||lab::closed("09 ROS1 digit callback"))ros::shutdown();}
        if(headless&&received>=210)ros::shutdown();
      }catch(const std::exception& e){failed=true;ROS_ERROR("%s",e.what());ros::shutdown();}
    });
    if(!headless)cv::namedWindow("09 ROS1 digit callback",cv::WINDOW_NORMAL);
    ros::spin();
    lab::mkdir(parser.get<std::string>("output"));
    cv::FileStorage output(parser.get<std::string>("output")+"/processor.json",cv::FileStorage::WRITE|cv::FileStorage::FORMAT_JSON);
    output<<"received"<<received<<"callback_failed"<<int(failed);
    return failed?1:0;
  }catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}
}
