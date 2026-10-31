#include <cmath>
#include <csignal>
#include <geometry_msgs/Twist.h>
#include <ros/ros.h>

namespace {
volatile std::sig_atomic_t stop_requested = 0;
void requestStop(int) { stop_requested = 1; }
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "turtle_motion", ros::init_options::NoSigintHandler);
  std::signal(SIGINT, requestStop);
  std::signal(SIGTERM, requestStop);
  ros::NodeHandle nh;
  ros::NodeHandle private_nh("~");

  double linear, angular, duration;
  private_nh.param("linear", linear, 1.0);
  private_nh.param("angular", angular, 0.0);
  private_nh.param("duration", duration, 3.0);
  if (!std::isfinite(linear) || !std::isfinite(angular) ||
      !std::isfinite(duration) || duration <= 0.0) {
    ROS_ERROR("Speeds must be finite; duration must be positive.");
    return 1;
  }

  // The simulator subscribes to this topic to receive velocity commands.
  auto publisher = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1);
  ros::WallRate rate(20);

  // Allow the simulator to connect before the timed movement starts.
  const auto deadline = ros::WallTime::now() + ros::WallDuration(5.0);
  while (ros::ok() && !stop_requested && publisher.getNumSubscribers() == 0) {
    if (ros::WallTime::now() >= deadline) {
      ROS_ERROR("No subscriber. Start turtlesim or use lesson.launch.");
      return 1;
    }
    ros::spinOnce();
    rate.sleep();
  }

  // These are the key lines for this lesson.
  geometry_msgs::Twist command;
  command.linear.x = linear;    // Forward velocity.
  command.angular.z = angular; // Turning velocity, in radians per second.

  ROS_INFO("Moving: linear=%.2f angular=%.2f duration=%.2f seconds",
           linear, angular, duration);
  const auto start = ros::WallTime::now();
  while (ros::ok() && !stop_requested &&
         (ros::WallTime::now() - start).toSec() < duration) {
    publisher.publish(command);
    ros::spinOnce();
    rate.sleep();
  }

  // A default Twist has zero velocities. Send it before closing the connection.
  geometry_msgs::Twist stop;
  for (int i = 0; i < 3 && ros::ok(); ++i) {
    publisher.publish(stop);
    ros::WallDuration(0.05).sleep();
  }
  ROS_INFO("Movement finished; zero velocity sent.");
  ros::shutdown();
  return 0;
}
