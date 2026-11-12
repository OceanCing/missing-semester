#ifndef HERO_CHASSIS_CONTROLLER_CHASSIS_CONTROLLER_H_
#define HERO_CHASSIS_CONTROLLER_CHASSIS_CONTROLLER_H_
#include <control_msgs/JointControllerState.h>
#include <control_toolbox/pid.h>
#include <controller_interface/controller.h>
#include <dynamic_reconfigure/server.h>
#include <geometry_msgs/Twist.h>
#include <hardware_interface/joint_command_interface.h>
#include <hero_chassis_controller/ChassisConfig.h>
#include <hero_chassis_controller/kinematics.h>
#include <nav_msgs/Odometry.h>
#include <realtime_tools/realtime_buffer.h>
#include <realtime_tools/realtime_publisher.h>
#include <tf/transform_listener.h>
#include <tf2_msgs/TFMessage.h>

#include <array>
#include <memory>
#include <string>
namespace hero_chassis_controller {
class ChassisController
    : public controller_interface::Controller<hardware_interface::EffortJointInterface> {
 public:
  bool init(hardware_interface::EffortJointInterface* hw, ros::NodeHandle& root,
            ros::NodeHandle& controller) override;
  void starting(const ros::Time& time) override;
  void update(const ros::Time& time, const ros::Duration& period) override;
  void stopping(const ros::Time& time) override;

 private:
  struct Command {
    Velocity velocity;
    ros::Time stamp;
    bool valid{false};
  };
  using WheelPublisher = realtime_tools::RealtimePublisher<control_msgs::JointControllerState>;
  void receiveCommand(const geometry_msgs::TwistConstPtr& msg);
  void transformCommand(const ros::TimerEvent& event);
  void configure(ChassisConfig& config, uint32_t level);
  void publishState(const ros::Time& time, const ros::Duration& period, const Velocity& measured);
  std::array<hardware_interface::JointHandle, 4> joints_;
  std::array<control_toolbox::Pid, 4> pids_;
  std::array<std::unique_ptr<WheelPublisher>, 4> wheel_publishers_;
  std::array<double, 4> signs_{{1, 1, 1, 1}}, effort_limits_{{5, 5, 5, 5}};
  std::array<double, 4> target_{{0, 0, 0, 0}}, measured_{{0, 0, 0, 0}};
  std::array<double, 4> error_{{0, 0, 0, 0}}, effort_{{0, 0, 0, 0}};
  realtime_tools::RealtimeBuffer<Command> raw_command_, body_command_;
  realtime_tools::RealtimeBuffer<ChassisConfig> settings_;
  std::unique_ptr<dynamic_reconfigure::Server<ChassisConfig>> reconfigure_;
  std::unique_ptr<tf::TransformListener> listener_;
  std::unique_ptr<realtime_tools::RealtimePublisher<nav_msgs::Odometry>> odom_publisher_;
  std::unique_ptr<realtime_tools::RealtimePublisher<tf2_msgs::TFMessage>> tf_publisher_;
  ros::Subscriber command_subscriber_;
  ros::Timer transform_timer_;
  Velocity limited_;
  std::string world_frame_, odom_frame_, base_frame_;
  double radius_{0.07625}, lever_{0.4}, max_wheel_speed_{40.0};
  double x_{0.0}, y_{0.0}, yaw_{0.0};
  ros::Time last_publish_;
  bool was_active_{false};
};
}  // namespace hero_chassis_controller
#endif  // HERO_CHASSIS_CONTROLLER_CHASSIS_CONTROLLER_H_
