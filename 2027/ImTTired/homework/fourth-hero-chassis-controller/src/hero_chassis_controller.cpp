
#include "hero_chassis_controller/hero_chassis_controller.h"

#include <algorithm>
#include <cmath>
#include <exception>

#include <geometry_msgs/TransformStamped.h>
#include <pluginlib/class_list_macros.hpp>

namespace hero_chassis_controller {

bool HeroChassisController::init(
    hardware_interface::EffortJointInterface* hardware,
    ros::NodeHandle& root_nh,
    ros::NodeHandle& controller_nh) {

  const std::array<const char*, 4> joint_names{{
      "left_front_wheel_joint",
      "right_front_wheel_joint",
      "left_back_wheel_joint",
      "right_back_wheel_joint"
  }};

  try {
    for (size_t i = 0; i < wheels_.size(); ++i) {
      wheels_[i] = hardware->getHandle(joint_names[i]);
    }
  } catch (const std::exception& error) {
    ROS_ERROR_STREAM("Cannot get wheel joints: " << error.what());
    return false;
  }

  controller_nh.param("wheel_radius", wheel_radius_, 0.07625);
  controller_nh.param("wheel_track", wheel_track_, 0.4);
  controller_nh.param("wheel_base", wheel_base_, 0.4);
  controller_nh.param("max_effort", max_effort_, 2.0);
  controller_nh.param("max_wheel_speed", max_wheel_speed_, 20.0);
  controller_nh.param("command_timeout", command_timeout_, 0.5);

  if (!std::isfinite(wheel_radius_) ||
      !std::isfinite(wheel_track_) ||
      !std::isfinite(wheel_base_) ||
      !std::isfinite(max_effort_) ||
      !std::isfinite(max_wheel_speed_) ||
      !std::isfinite(command_timeout_) ||
      wheel_radius_ <= 0.0 || wheel_track_ <= 0.0 ||
      wheel_base_ <= 0.0 || max_effort_ <= 0.0 ||
      max_wheel_speed_ <= 0.0 || command_timeout_ <= 0.0) {
    ROS_ERROR("Invalid chassis parameters.");
    return false;
  }

  const std::array<const char*, 4> sign_names{{
      "front_left_sign",
      "front_right_sign",
      "rear_left_sign",
      "rear_right_sign"
  }};

  for (size_t i = 0; i < wheels_.size(); ++i) {
    controller_nh.param(sign_names[i], wheel_signs_[i], 1.0);

    if (!std::isfinite(wheel_signs_[i]) ||
        std::abs(std::abs(wheel_signs_[i]) - 1.0) > 1e-6) {
      ROS_ERROR("Wheel sign must be +1 or -1.");
      return false;
    }
  }

  double kp = 0.5;
  double ki = 0.0;
  double kd = 0.0;
  double i_clamp = 1.0;

  controller_nh.param("pid/p", kp, 0.5);
  controller_nh.param("pid/i", ki, 0.0);
  controller_nh.param("pid/d", kd, 0.0);
  controller_nh.param("pid/i_clamp", i_clamp, 1.0);

  if (!std::isfinite(kp) || !std::isfinite(ki) ||
      !std::isfinite(kd) || !std::isfinite(i_clamp) ||
      kp < 0.0 || ki < 0.0 || kd < 0.0 || i_clamp < 0.0) {
    ROS_ERROR("Invalid PID parameters.");
    return false;
  }

  for (auto& pid : pids_) {
    pid.initPid(kp, ki, kd, i_clamp, -i_clamp);
  }

  // V0.3 odometry parameters
  controller_nh.param(
      "odom_publish_rate", odom_publish_rate_, 30.0);
  controller_nh.param(
      "odom_frame", odom_frame_, std::string("odom"));
  controller_nh.param(
      "base_frame", base_frame_, std::string("base_link"));
  controller_nh.param("publish_tf", publish_tf_, false);
  controller_nh.param("velocity_frame_mode", velocity_frame_mode_,
                      std::string("body"));
  if (velocity_frame_mode_ != "body" && velocity_frame_mode_ != "world") {
    ROS_ERROR("velocity_frame_mode must be body or world");
    return false;
  }
  if (velocity_frame_mode_ == "world" && !publish_tf_) {
    ROS_ERROR("World mode requires publish_tf=true (or an external odom TF publisher)");
    return false;
  }

  if (!std::isfinite(odom_publish_rate_) ||
      odom_publish_rate_ <= 0.0 ||
      odom_frame_.empty() || base_frame_.empty() ||
      odom_frame_ == base_frame_) {
    ROS_ERROR("Invalid odometry parameters.");
    return false;
  }

  odom_pub_ = root_nh.advertise<nav_msgs::Odometry>(
      "/odom", 10);

  VelocityCommand initial_command;
  command_buffer_.writeFromNonRT(initial_command);

  cmd_vel_sub_ = root_nh.subscribe(
      "/cmd_vel", 1,
      &HeroChassisController::cmdVelCallback, this);

  cmd_vel_world_sub_ = root_nh.subscribe(
      "/cmd_vel_world", 1,
      &HeroChassisController::cmdVelWorldCallback, this);

  ROS_INFO_STREAM("Hero chassis initialized, velocity_frame_mode="
                  << velocity_frame_mode_);
  return true;
}

void HeroChassisController::cmdVelCallback(
    const geometry_msgs::TwistConstPtr& msg) {
  acceptVelocityCommand(msg, velocity_frame_mode_ == "world");
}

void HeroChassisController::cmdVelWorldCallback(
    const geometry_msgs::TwistConstPtr& msg) {
  // Backwards-compatible input: always interpreted in odom frame.
  acceptVelocityCommand(msg, true);
}

void HeroChassisController::acceptVelocityCommand(
    const geometry_msgs::TwistConstPtr& msg, bool use_world_frame) {
  if (!std::isfinite(msg->linear.x) ||
      !std::isfinite(msg->linear.y) ||
      !std::isfinite(msg->angular.z)) {
    ROS_WARN_THROTTLE(2.0, "Ignoring non-finite velocity command");
    return;
  }

  VelocityCommand command;
  command.vx = msg->linear.x;
  command.vy = msg->linear.y;
  command.wz = msg->angular.z;

  if (use_world_frame) {
    // Perform TF lookup outside the real-time controller update loop.
    // Use the latest available odom -> base_link transform.
    tf::Stamped<tf::Vector3> world_velocity(
        tf::Vector3(msg->linear.x, msg->linear.y, 0.0),
        ros::Time(0), odom_frame_);
    tf::Stamped<tf::Vector3> body_velocity;
    try {
      tf_listener_.transformVector(
          base_frame_, world_velocity, body_velocity);
    } catch (const tf::TransformException& error) {
      ROS_WARN_STREAM_THROTTLE(
          1.0, "Cannot transform velocity from " << odom_frame_
          << " to " << base_frame_ << ": " << error.what());
      // Do not turn an untransformable global command into a body command.
      VelocityCommand safe_stop;
      command_buffer_.writeFromNonRT(safe_stop);
      return;
    }
    command.vx = body_velocity.x();
    command.vy = body_velocity.y();
  }

  // Twist has no header; time of receipt is the command timestamp.
  command.stamp = ros::Time::now();
  command_buffer_.writeFromNonRT(command);
}

void HeroChassisController::starting(const ros::Time& time) {
  for (auto& pid : pids_) {
    pid.reset();
  }

  for (auto& wheel : wheels_) {
    wheel.setCommand(0.0);
  }

  // Initial pose is relative to the starting position.
  x_ = 0.0;
  y_ = 0.0;
  yaw_ = 0.0;

  vx_actual_ = 0.0;
  vy_actual_ = 0.0;
  wz_actual_ = 0.0;

  last_publish_time_ = time;
}

void HeroChassisController::update(
    const ros::Time& time,
    const ros::Duration& period) {

  const double dt = period.toSec();

  if (!std::isfinite(dt) || dt <= 0.0) {
    return;
  }

  const VelocityCommand* command = command_buffer_.readFromRT();

  double vx = 0.0;
  double vy = 0.0;
  double wz = 0.0;


  if (command != nullptr && !command->stamp.isZero()) {
    const double age = (time - command->stamp).toSec();

    if (age >= 0.0 && age <= command_timeout_) {

      // The callback already transformed commands into base_link.
      // The real-time loop performs no TF lookup.
      vx = command->vx;
      vy = command->vy;
      // Angular velocity is about the vertical Z axis.
      wz = command->wz;
    }
  }


  // V0.2: mecanum inverse kinematics
  const double k = (wheel_track_ + wheel_base_) / 2.0;
  const double r = wheel_radius_;

  const std::array<double, 4> target{{
      (vx - vy - k * wz) / r,
      (vx + vy + k * wz) / r,
      (vx + vy - k * wz) / r,
      (vx - vy + k * wz) / r
  }};

  for (size_t i = 0; i < wheels_.size(); ++i) {
    const double target_velocity = std::max(
        -max_wheel_speed_,
        std::min(max_wheel_speed_,
                 wheel_signs_[i] * target[i]));

    const double actual_velocity = wheels_[i].getVelocity();

    if (!std::isfinite(actual_velocity)) {
      wheels_[i].setCommand(0.0);
      pids_[i].reset();
      continue;
    }

    const double error = target_velocity - actual_velocity;

    const double effort =
        pids_[i].computeCommand(error, period);

    const double limited_effort = std::max(
        -max_effort_, std::min(max_effort_, effort));

    wheels_[i].setCommand(
        std::isfinite(limited_effort) ? limited_effort : 0.0);
  }

  // V0.3: integrate actual wheel velocities
  updateOdometry(time, dt);
}

void HeroChassisController::updateOdometry(
    const ros::Time& time, double dt) {

  std::array<double, 4> w;

  for (size_t i = 0; i < wheels_.size(); ++i) {
    w[i] = wheel_signs_[i] * wheels_[i].getVelocity();

    if (!std::isfinite(w[i])) {
      return;
    }
  }

  const double r = wheel_radius_;
  const double k = (wheel_track_ + wheel_base_) / 2.0;

  // Forward kinematics
  vx_actual_ = r * (w[0] + w[1] + w[2] + w[3]) / 4.0;

  vy_actual_ = r * (-w[0] + w[1] + w[2] - w[3]) / 4.0;

  wz_actual_ =
      r * (-w[0] + w[1] - w[2] + w[3]) / (4.0 * k);

  // Midpoint integration
  const double yaw_mid = yaw_ + 0.5 * wz_actual_ * dt;

  x_ += (vx_actual_ * std::cos(yaw_mid) -
         vy_actual_ * std::sin(yaw_mid)) * dt;

  y_ += (vx_actual_ * std::sin(yaw_mid) +
         vy_actual_ * std::cos(yaw_mid)) * dt;

  yaw_ += wz_actual_ * dt;

  yaw_ = std::atan2(std::sin(yaw_), std::cos(yaw_));

  // Publish at a controlled frequency
  if ((time - last_publish_time_).toSec() >=
      1.0 / odom_publish_rate_) {

    publishOdometry(time, vx_actual_, vy_actual_, wz_actual_);
    last_publish_time_ = time;
  }
}

void HeroChassisController::publishOdometry(
    const ros::Time& time,
    double vx, double vy, double wz) {

  geometry_msgs::Quaternion q;
  q.x = 0.0;
  q.y = 0.0;
  q.z = std::sin(yaw_ / 2.0);
  q.w = std::cos(yaw_ / 2.0);

  nav_msgs::Odometry odom;
  odom.header.stamp = time;
  odom.header.frame_id = odom_frame_;
  odom.child_frame_id = base_frame_;

  odom.pose.pose.position.x = x_;
  odom.pose.pose.position.y = y_;
  odom.pose.pose.position.z = 0.0;
  odom.pose.pose.orientation = q;

  odom.twist.twist.linear.x = vx;
  odom.twist.twist.linear.y = vy;
  odom.twist.twist.angular.z = wz;

  // Illustrative diagonal covariance; calibrate later.
  odom.pose.covariance[0] = 0.01;
  odom.pose.covariance[7] = 0.01;
  odom.pose.covariance[14] = 1e6;
  odom.pose.covariance[21] = 1e6;
  odom.pose.covariance[28] = 1e6;
  odom.pose.covariance[35] = 0.03;

  odom.twist.covariance[0] = 0.05;
  odom.twist.covariance[7] = 0.05;
  odom.twist.covariance[14] = 1e6;
  odom.twist.covariance[21] = 1e6;
  odom.twist.covariance[28] = 1e6;
  odom.twist.covariance[35] = 0.05;

  odom_pub_.publish(odom);

  if (publish_tf_) {
    geometry_msgs::TransformStamped tf_msg;

    tf_msg.header.stamp = time;
    tf_msg.header.frame_id = odom_frame_;
    tf_msg.child_frame_id = base_frame_;

    tf_msg.transform.translation.x = x_;
    tf_msg.transform.translation.y = y_;
    tf_msg.transform.translation.z = 0.0;
    tf_msg.transform.rotation = q;

    tf_broadcaster_.sendTransform(tf_msg);
  }
}

void HeroChassisController::stopping(const ros::Time& time) {
  (void)time;

  for (auto& wheel : wheels_) {
    wheel.setCommand(0.0);
  }

  for (auto& pid : pids_) {
    pid.reset();
  }
}

}  // namespace hero_chassis_controller

PLUGINLIB_EXPORT_CLASS(
    hero_chassis_controller::HeroChassisController,
    controller_interface::ControllerBase)
