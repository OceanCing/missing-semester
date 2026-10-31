#include <ros/ros.h>
#include <turtlesim/Pose.h>

// ROS calls this function when a new position message is processed.
void poseCallback(const turtlesim::Pose::ConstPtr& message) {
  // Messages arrive frequently; print at most once per second.
  ROS_INFO_THROTTLE(1.0, "Pose: x=%.2f y=%.2f theta=%.2f rad",
                    message->x, message->y, message->theta);
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "turtle_pose_listener");
  ros::NodeHandle nh;

  // Register the topic and the function that will process its messages.
  auto subscriber = nh.subscribe("turtle1/pose", 10, poseCallback);
  ROS_INFO("Listening to %s. Press Ctrl+C to stop this listener.", subscriber.getTopic().c_str());

  // Keep processing callbacks until this node is stopped.
  ros::spin();
  return 0;
}
