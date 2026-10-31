#include "course.hpp"
#include <cv_bridge/cv_bridge.h>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <iostream>

int main(int argc, char** argv) {
  ros::init(argc, argv, "offline_image_demo", ros::init_options::AnonymousName);
  cv::CommandLineParser parser(argc, argv,
      "{mode |publish| publish or process}{assets | | Generated assets}{output | | Output directory}"
      "{config | | Detector YAML}{headless |false| No GUI}{frames |10| Headless receive count}");
  const auto mode = parser.get<std::string>("mode");
  const auto assets = parser.get<std::string>("assets");
  const auto output = parser.get<std::string>("output");
  const auto config_path = parser.get<std::string>("config");
  const bool headless = parser.get<bool>("headless");
  const int limit = parser.get<int>("frames");
  if (!parser.check() || assets.empty() || output.empty() || limit < 1) return 2;
  try {
    lab::mkdir(output);
    ros::NodeHandle node;
    if (mode == "publish") {
      auto publisher = node.advertise<sensor_msgs::Image>("/vision_course/image_raw", 1);
      cv::VideoCapture capture(assets + "/frames/frame-%03d.png", cv::CAP_IMAGES);
      if (!capture.isOpened()) throw std::runtime_error("Cannot open offline PNG sequence");
      ros::Rate rate(20);
      unsigned int sequence = 0;
      while (ros::ok()) {
        cv::Mat frame;
        if (!capture.read(frame)) { capture.set(cv::CAP_PROP_POS_FRAMES, 0); continue; }
        std_msgs::Header header;
        header.seq = sequence++;
        header.stamp = ros::Time::now();  // Simulated acquisition at the offline publisher.
        header.frame_id = "camera_optical_frame";
        publisher.publish(cv_bridge::CvImage(header, sensor_msgs::image_encodings::BGR8, frame).toImageMsg());
        ros::spinOnce(); rate.sleep();
      }
    } else if (mode == "process") {
      const auto config = lab::loadConfig(config_path);
      auto publisher = node.advertise<sensor_msgs::Image>("/vision_course/image_debug", 1);
      int received = 0;
      bool callback_failed = false;
      const std::string window = "15 ROS1 offline image callback";
      auto subscriber = node.subscribe<sensor_msgs::Image>("/vision_course/image_raw", 1,
        [&](const sensor_msgs::ImageConstPtr& message) {
          try {
            const auto shared = cv_bridge::toCvShare(message, sensor_msgs::image_encodings::BGR8);
            const auto detection = lab::detect(shared->image, config);  // detect draws on clone().
            const auto debug = cv_bridge::CvImage(message->header, sensor_msgs::image_encodings::BGR8,
                                                  detection.annotated).toImageMsg();
            publisher.publish(debug);
            ++received;
            ROS_INFO_THROTTLE(1.0, "received=%d pixels=%ux%u pairs=%d frame=%s", received,
                              message->width, message->height, lab::countPairs(detection), message->header.frame_id.c_str());
            if (!headless) {
              cv::imshow(window, detection.annotated);
              const int key = cv::waitKey(1) & 0xff;
              if (key == 'q' || key == 27 || lab::closed(window)) ros::shutdown();
            }
            if (headless && received >= limit) {
              lab::Options options; options.output = output;
              lab::saveResult(options, {{{"ROS callback processed offline image", detection.annotated}},
                {{"received", received}, {"width", message->width}, {"height", message->height},
                 {"stamp_preserved", debug->header.stamp == message->header.stamp ? 1.0 : 0.0},
                 {"frame_preserved", debug->header.frame_id == message->header.frame_id ? 1.0 : 0.0},
                 {"encoding_bgr8", debug->encoding == "bgr8" ? 1.0 : 0.0}}});
              ros::shutdown();
            }
          } catch (const std::exception& error) {
            callback_failed = true; ROS_ERROR("Image callback: %s", error.what()); ros::shutdown();
          }
        });
      if (!headless) {
        cv::namedWindow(window, cv::WINDOW_NORMAL); cv::resizeWindow(window, 800, 600);
      }
      ros::spin();
      if (callback_failed) return 1;
      if (headless && received < limit) return 1;
    } else throw std::runtime_error("mode must be publish or process");
  } catch (const std::exception& error) { std::cerr << error.what() << std::endl; return 1; }
  return 0;
}
