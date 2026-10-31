#include <cmath>
#include <geometry_msgs/PointStamped.h>
#include <ros/ros.h>
#include <tf2/exceptions.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>

int main(int argc, char** argv) {
  ros::init(argc, argv, "point_to_world");
  ros::NodeHandle private_nh("~");

  // The listener receives TF messages and stores frame relationships in buffer.
  tf2_ros::Buffer buffer;
  tf2_ros::TransformListener listener(buffer);

  geometry_msgs::PointStamped point;
  point.header.frame_id = "lesson_robot";  // The input coordinates belong to this frame.
  point.header.stamp = ros::Time(0);        // Use the latest available transform.
  private_nh.param("x", point.point.x, 2.0);
  private_nh.param("y", point.point.y, 0.0);
  private_nh.param("z", point.point.z, 0.0);
  if (!std::isfinite(point.point.x) || !std::isfinite(point.point.y) ||
      !std::isfinite(point.point.z)) {
    ROS_ERROR("Point coordinates must be finite.");
    return 1;
  }

  ROS_INFO("Robot point: frame=%s x=%.3f y=%.3f z=%.3f",
           point.header.frame_id.c_str(), point.point.x, point.point.y, point.point.z);
  try {
    // TF2 applies the known rotation and translation to this point.
    const auto world_point = buffer.transform(point, "lesson_world", ros::Duration(5.0));
    ROS_INFO("World point: frame=%s x=%.3f y=%.3f z=%.3f",
             world_point.header.frame_id.c_str(), world_point.point.x,
             world_point.point.y, world_point.point.z);
  } catch (const tf2::TransformException& error) {
    ROS_ERROR("Transform unavailable. Start frames.launch first. Details: %s", error.what());
    return 1;
  }
  return 0;
}
