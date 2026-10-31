#include <geometry_msgs/TransformStamped.h>
#include <ros/ros.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2_ros/transform_broadcaster.h>
#include <turtlesim/Pose.h>

class TurtleFrameBroadcaster {
 public:
  explicit TurtleFrameBroadcaster(ros::NodeHandle& nh) {
    subscriber_ = nh.subscribe("turtle1/pose", 10,
                               &TurtleFrameBroadcaster::poseCallback, this);
    ROS_INFO("Listening to %s; broadcasting turtle_world -> turtle_body on /tf.",
             subscriber_.getTopic().c_str());
  }

 private:
  void poseCallback(const turtlesim::Pose::ConstPtr& pose) {
    geometry_msgs::TransformStamped transform;
    // turtlesim/Pose has no Header: use callback time for this live-data lesson.
    // For stamped sensor data, use its measurement time instead.
    transform.header.stamp = ros::Time::now();
    transform.header.frame_id = "turtle_world";
    transform.child_frame_id = "turtle_body";
    transform.transform.translation.x = pose->x;
    transform.transform.translation.y = pose->y;
    transform.transform.translation.z = 0.0;

    tf2::Quaternion rotation;
    rotation.setRPY(0.0, 0.0, pose->theta);
    transform.transform.rotation = tf2::toMsg(rotation);
    broadcaster_.sendTransform(transform);
  }

  tf2_ros::TransformBroadcaster broadcaster_;
  ros::Subscriber subscriber_;
};

int main(int argc, char** argv) {
  ros::init(argc, argv, "turtle_tf_broadcaster");
  ros::NodeHandle nh;
  TurtleFrameBroadcaster node(nh);
  ros::spin();
  return 0;
}
