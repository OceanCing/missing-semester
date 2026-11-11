#include <hero_chassis_controller/chassis_controller.h>

#include <algorithm>
#include <cmath>
#include <pluginlib/class_list_macros.hpp>
#include <vector>

namespace hero_chassis_controller {
bool ChassisController::init(hardware_interface::EffortJointInterface* hw, ros::NodeHandle& root,
                             ros::NodeHandle& controller) {
  std::vector<std::string> names;
  std::vector<double> signs;
  if (!controller.getParam("joints", names) || names.size() != 4 ||
      !controller.getParam("wheel_signs", signs) || signs.size() != 4) {
    ROS_ERROR("Configure four joints and wheel_signs in FL, FR, BL, BR order.");
    return false;
  }
  double wheelbase, track;
  controller.param("wheel_radius", radius_, 0.07625);
  controller.param("wheel_base", wheelbase, 0.4);
  controller.param("wheel_track", track, 0.4);
  controller.param("max_wheel_speed", max_wheel_speed_, 40.0);
  lever_ = (wheelbase + track) / 2.0;
  if (!std::isfinite(radius_) || !std::isfinite(lever_) || radius_ <= 0 || lever_ <= 0 ||
      !std::isfinite(max_wheel_speed_) || max_wheel_speed_ <= 0) {
    ROS_ERROR("Wheel geometry and speed limit must be finite and positive.");
    return false;
  }
  controller.param<std::string>("odom_frame", odom_frame_, "odom");
  controller.param<std::string>("base_frame", base_frame_, "base_link");
  controller.param<std::string>("world_frame", world_frame_, "odom");
  const std::array<std::string, 4> labels{{"front_left", "front_right", "back_left", "back_right"}};
  for (size_t i = 0; i < 4; ++i) {
    if (std::abs(signs[i]) != 1.0) {
      ROS_ERROR("wheel_signs must be +1 or -1.");
      return false;
    }
    signs_[i] = signs[i];
    try {
      joints_[i] = hw->getHandle(names[i]);
    } catch (const hardware_interface::HardwareInterfaceException& exception) {
      ROS_ERROR_STREAM(exception.what());
      return false;
    }
    ros::NodeHandle pid_nh(controller, "pid/" + labels[i]);
    if (!pids_[i].init(pid_nh)) return false;
    pid_nh.param("effort_limit", effort_limits_[i], 5.0);
    if (!std::isfinite(effort_limits_[i]) || effort_limits_[i] <= 0) return false;
    wheel_publishers_[i].reset(new WheelPublisher(controller, labels[i] + "/state", 1));
  }
  ChassisConfig config = ChassisConfig::__getDefault__();
  std::string mode;
  controller.param<std::string>("cmd_vel_mode", mode, "base_link");
  if (mode != "base_link" && mode != "world") {
    ROS_ERROR("cmd_vel_mode must be base_link or world.");
    return false;
  }
  config.world_mode = mode == "world";
  controller.param("cmd_vel_timeout", config.cmd_vel_timeout, config.cmd_vel_timeout);
  controller.param("max_acceleration_x", config.max_acceleration_x, config.max_acceleration_x);
  controller.param("max_acceleration_y", config.max_acceleration_y, config.max_acceleration_y);
  controller.param("max_acceleration_wz", config.max_acceleration_wz, config.max_acceleration_wz);
  reconfigure_.reset(new dynamic_reconfigure::Server<ChassisConfig>(controller));
  reconfigure_->updateConfig(config);
  reconfigure_->setCallback([this](ChassisConfig& cfg, uint32_t level) { configure(cfg, level); });
  listener_.reset(new tf::TransformListener(ros::Duration(5.0)));
  odom_publisher_.reset(new realtime_tools::RealtimePublisher<nav_msgs::Odometry>(root, "odom", 1));
  odom_publisher_->msg_.header.frame_id = odom_frame_;
  odom_publisher_->msg_.child_frame_id = base_frame_;
  for (size_t i = 0; i < 6; ++i) {
    const double variance = (i == 0 || i == 1 || i == 5) ? 0.02 : 1e6;
    odom_publisher_->msg_.pose.covariance[i * 6 + i] = variance;
    odom_publisher_->msg_.twist.covariance[i * 6 + i] = variance;
  }
  tf_publisher_.reset(new realtime_tools::RealtimePublisher<tf2_msgs::TFMessage>(root, "/tf", 1));
  tf_publisher_->msg_.transforms.resize(1);
  tf_publisher_->msg_.transforms[0].header.frame_id = odom_frame_;
  tf_publisher_->msg_.transforms[0].child_frame_id = base_frame_;
  command_subscriber_ = root.subscribe("cmd_vel", 1, &ChassisController::receiveCommand, this);
  transform_timer_ =
      controller.createTimer(ros::Duration(0.02), &ChassisController::transformCommand, this);
  return true;
}
void ChassisController::configure(ChassisConfig& config, uint32_t /*level*/) {
  settings_.writeFromNonRT(config);
}
void ChassisController::receiveCommand(const geometry_msgs::TwistConstPtr& msg) {
  Command command;
  command.stamp = ros::Time::now();
  command.valid =
      std::isfinite(msg->linear.x) && std::isfinite(msg->linear.y) && std::isfinite(msg->angular.z);
  if (command.valid)
    command.velocity = {clamp(msg->linear.x, 0.6), clamp(msg->linear.y, 0.6),
                        clamp(msg->angular.z, 1.2)};
  raw_command_.writeFromNonRT(command);
}
void ChassisController::transformCommand(const ros::TimerEvent& /*event*/) {
  Command command = *raw_command_.readFromNonRT();
  const auto config = *settings_.readFromNonRT();
  if (command.valid && config.world_mode) {
    try {
      tf::StampedTransform transform;
      listener_->lookupTransform(base_frame_, world_frame_, ros::Time(0), transform);
      if (!transform.stamp_.isZero() && (ros::Time::now() - transform.stamp_).toSec() > 0.2)
        throw tf::TransformException("world-to-base transform is stale");
      tf::Stamped<tf::Vector3> input(tf::Vector3(command.velocity.x, command.velocity.y, 0),
                                     ros::Time(0), world_frame_);
      tf::Stamped<tf::Vector3> output;
      listener_->transformVector(base_frame_, input, output);
      command.velocity.x = output.x();
      command.velocity.y = output.y();
    } catch (const tf::TransformException& exception) {
      command.valid = false;
      ROS_WARN_THROTTLE(1.0, "World command stopped: %s", exception.what());
    }
  }
  body_command_.writeFromNonRT(command);
}
void ChassisController::starting(const ros::Time& time) {
  for (auto& pid : pids_) pid.reset();
  limited_ = {};
  was_active_ = false;
  x_ = y_ = yaw_ = 0.0;
  last_publish_ = time;
}
void ChassisController::update(const ros::Time& time, const ros::Duration& period) {
  const double dt = period.toSec();
  if (dt <= 0 || !std::isfinite(dt)) return;
  const auto command = *body_command_.readFromRT();
  const auto config = *settings_.readFromRT();
  const bool active =
      command.valid && commandFresh(time.toSec(), command.stamp.toSec(), config.cmd_vel_timeout);
  if (active) {
    limited_.x = approach(limited_.x, command.velocity.x, config.max_acceleration_x * dt);
    limited_.y = approach(limited_.y, command.velocity.y, config.max_acceleration_y * dt);
    limited_.yaw = approach(limited_.yaw, command.velocity.yaw, config.max_acceleration_wz * dt);
  } else {
    limited_ = {};  // Watchdog and TF failure bypass the acceleration ramp.
  }
  target_ = inverse(limited_, radius_, lever_);
  double largest = 0;
  for (double speed : target_) largest = std::max(largest, std::abs(speed));
  if (largest > max_wheel_speed_)
    for (double& speed : target_) speed *= max_wheel_speed_ / largest;
  for (size_t i = 0; i < 4; ++i) {
    measured_[i] = signs_[i] * joints_[i].getVelocity();
    error_[i] = target_[i] - measured_[i];
    if (!active && was_active_) pids_[i].reset();
    effort_[i] = clamp(pids_[i].computeCommand(error_[i], period), effort_limits_[i]);
    joints_[i].setCommand(signs_[i] * effort_[i]);
  }
  was_active_ = active;
  const auto measured = forward(measured_, radius_, lever_);
  const double mid_yaw = yaw_ + 0.5 * measured.yaw * dt;
  x_ += (std::cos(mid_yaw) * measured.x - std::sin(mid_yaw) * measured.y) * dt;
  y_ += (std::sin(mid_yaw) * measured.x + std::cos(mid_yaw) * measured.y) * dt;
  yaw_ = std::atan2(std::sin(yaw_ + measured.yaw * dt), std::cos(yaw_ + measured.yaw * dt));
  if ((time - last_publish_).toSec() >= 0.02) {
    last_publish_ = time;
    publishState(time, period, measured);
  }
}
void ChassisController::publishState(const ros::Time& time, const ros::Duration& period,
                                     const Velocity& measured) {
  const auto orientation = tf::createQuaternionMsgFromYaw(yaw_);
  if (odom_publisher_->trylock()) {
    auto& msg = odom_publisher_->msg_;
    msg.header.stamp = time;
    msg.pose.pose.position.x = x_;
    msg.pose.pose.position.y = y_;
    msg.pose.pose.orientation = orientation;
    msg.twist.twist.linear.x = measured.x;
    msg.twist.twist.linear.y = measured.y;
    msg.twist.twist.angular.z = measured.yaw;
    odom_publisher_->unlockAndPublish();
  }
  if (tf_publisher_->trylock()) {
    auto& msg = tf_publisher_->msg_.transforms[0];
    msg.header.stamp = time;
    msg.transform.translation.x = x_;
    msg.transform.translation.y = y_;
    msg.transform.rotation = orientation;
    tf_publisher_->unlockAndPublish();
  }
  for (size_t i = 0; i < 4; ++i) {
    if (!wheel_publishers_[i]->trylock()) continue;
    auto& msg = wheel_publishers_[i]->msg_;
    msg.header.stamp = time;
    msg.set_point = target_[i];
    msg.process_value = measured_[i];
    msg.error = error_[i];  // target - measured
    msg.command = effort_[i];
    msg.time_step = period.toSec();
    const auto gains = pids_[i].getGains();
    msg.p = gains.p_gain_;
    msg.i = gains.i_gain_;
    msg.d = gains.d_gain_;
    msg.i_clamp = gains.i_max_;
    msg.antiwindup = gains.antiwindup_;
    wheel_publishers_[i]->unlockAndPublish();
  }
}
void ChassisController::stopping(const ros::Time& /*time*/) {
  for (auto& joint : joints_) joint.setCommand(0.0);
}
}  // namespace hero_chassis_controller
PLUGINLIB_EXPORT_CLASS(hero_chassis_controller::ChassisController,
                       controller_interface::ControllerBase)
