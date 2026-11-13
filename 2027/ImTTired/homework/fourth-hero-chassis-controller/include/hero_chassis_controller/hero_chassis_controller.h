
#ifndef HERO_CHASSIS_CONTROLLER_HERO_CHASSIS_CONTROLLER_H_
#define HERO_CHASSIS_CONTROLLER_HERO_CHASSIS_CONTROLLER_H_

#include <array>
#include <string>

#include <controller_interface/controller.h>
#include <control_toolbox/pid.h>
#include <geometry_msgs/Twist.h>
#include <hardware_interface/joint_command_interface.h>
#include <nav_msgs/Odometry.h>
#include <realtime_tools/realtime_buffer.h>
#include <ros/ros.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf/transform_listener.h>

namespace hero_chassis_controller {

class HeroChassisController
    : public controller_interface::Controller<
          hardware_interface::EffortJointInterface> {
 public:
  bool init(hardware_interface::EffortJointInterface* hardware,
            ros::NodeHandle& root_nh,
            ros::NodeHandle& controller_nh) override;

  void starting(const ros::Time& time) override;

  void update(const ros::Time& time,
              const ros::Duration& period) override;

  void stopping(const ros::Time& time) override;

 private:
    struct VelocityCommand {
        double vx = 0.0;
        double vy = 0.0;
        double wz = 0.0;
        ros::Time stamp;

    };


  void cmdVelCallback(const geometry_msgs::TwistConstPtr& msg);
  void cmdVelWorldCallback(const geometry_msgs::TwistConstPtr& msg);
  void acceptVelocityCommand(const geometry_msgs::TwistConstPtr& msg,
                             bool use_world_frame);

  void updateOdometry(const ros::Time& time, double dt);
  void publishOdometry(const ros::Time& time,
                       double vx, double vy, double wz);

  // Wheel order: front-left, front-right, rear-left, rear-right
  std::array<hardware_interface::JointHandle, 4> wheels_;
  std::array<control_toolbox::Pid, 4> pids_;

  realtime_tools::RealtimeBuffer<VelocityCommand> command_buffer_;
    ros::Subscriber cmd_vel_sub_;
  ros::Subscriber cmd_vel_world_sub_;
  std::string velocity_frame_mode_ = "body";
  tf::TransformListener tf_listener_;

  double wheel_radius_ = 0.07625;
  double wheel_track_ = 0.4;
  double wheel_base_ = 0.4;

  double max_effort_ = 2.0;
  double max_wheel_speed_ = 20.0;
  double command_timeout_ = 0.5;

  std::array<double, 4> wheel_signs_{{1.0, 1.0, 1.0, 1.0}};

  // V0.3: odometry
  double x_ = 0.0;
  double y_ = 0.0;
  double yaw_ = 0.0;

  double vx_actual_ = 0.0;
  double vy_actual_ = 0.0;
  double wz_actual_ = 0.0;

  double odom_publish_rate_ = 30.0;
  ros::Time last_publish_time_;

  std::string odom_frame_ = "odom";
  std::string base_frame_ = "base_link";
  bool publish_tf_ = false;

  ros::Publisher odom_pub_;
  tf2_ros::TransformBroadcaster tf_broadcaster_;
};

}  // namespace hero_chassis_controller

#endif
