#include <algorithm>
#include <cmath>
#include <csignal>
#include <geometry_msgs/Twist.h>
#include <ros/ros.h>
#include <turtlesim/Pose.h>

namespace {
ros::Publisher velocity_publisher;
double target_x = 8.0;
double max_speed = 1.0;
constexpr double tolerance = 0.05;
double current_x = 0.0;
bool received_pose = false;
bool reached = false;
bool heading_ok = true;
ros::WallTime last_pose_time;
volatile std::sig_atomic_t stop_requested = 0;

void requestStop(int) { stop_requested = 1; }

void poseCallback(const turtlesim::Pose::ConstPtr& message) {
  received_pose = true;
  current_x = message->x;
  last_pose_time = ros::WallTime::now();

  // This lesson controls only x; the turtle must face right after a reset.
  if (std::abs(message->theta) > 0.05) {
    heading_ok = false;
    return;
  }

  // Feedback control: position error determines the next velocity command.
  const double error = target_x - message->x;
  geometry_msgs::Twist command;
  if (std::abs(error) <= tolerance) {
    reached = true;  // Leave the default zero velocity in command.
  } else {
    // Gain = 1/s: approach slowly near the goal, with a configurable speed cap.
    command.linear.x = std::max(-max_speed, std::min(max_speed, error));
  }
  velocity_publisher.publish(command);
  ROS_INFO_THROTTLE(0.5, "x=%.3f target=%.3f error=%.3f speed=%.3f",
                    message->x, target_x, error, command.linear.x);
}
}  // namespace

int main(int argc, char** argv) {
  ros::init(argc, argv, "turtle_go_to_x", ros::init_options::NoSigintHandler);
  std::signal(SIGINT, requestStop);
  std::signal(SIGTERM, requestStop);
  ros::NodeHandle nh;
  ros::NodeHandle private_nh("~");
  private_nh.param("target_x", target_x, 8.0);
  private_nh.param("max_speed", max_speed, 1.0);
  if (!std::isfinite(target_x) || target_x < 0.5 || target_x > 10.5 ||
      !std::isfinite(max_speed) || max_speed <= 0.0 || max_speed > 5.0) {
    ROS_ERROR("Use target_x in [0.5, 10.5] and max_speed in (0, 5].");
    return 1;
  }

  velocity_publisher = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1);
  auto subscriber = nh.subscribe("turtle1/pose", 1, poseCallback);
  ros::WallRate rate(20);
  const auto start = ros::WallTime::now();
  ROS_INFO("Go to x=%.2f with speed cap %.2f. Reset turtlesim first.", target_x, max_speed);
  while (ros::ok() && !stop_requested && !reached && heading_ok) {
    ros::spinOnce();
    const auto now = ros::WallTime::now();
    if (!received_pose && (now - start).toSec() > 5.0) {
      ROS_ERROR("No position messages. Start turtlesim first.");
      break;
    }
    if (received_pose && (now - last_pose_time).toSec() > 0.5) {
      ROS_ERROR("Position messages stopped; stopping movement.");
      break;
    }
    if ((now - start).toSec() > 30.0) {
      ROS_ERROR("Target not reached within 30 seconds.");
      break;
    }
    rate.sleep();
  }

  geometry_msgs::Twist stop;
  for (int i = 0; i < 3 && ros::ok(); ++i) {
    velocity_publisher.publish(stop);
    ros::WallDuration(0.05).sleep();
  }
  if (reached) {
    ROS_INFO("Reached target: x=%.3f target=%.3f; stopped.", current_x, target_x);
  } else if (!heading_ok) {
    ROS_ERROR("This lesson requires facing right. Run: rosservice call /reset");
  } else if (stop_requested) {
    ROS_INFO("Interrupted; zero velocity sent.");
  }
  ros::shutdown();
  return reached || stop_requested ? 0 : 1;
}
