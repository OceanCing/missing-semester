#ifndef HERO_CHASSIS_CONTROLLER_HPP_
#define HERO_CHASSIS_CONTROLLER_HPP_

#include<geometry_msgs/Twist.h>
#include<nav_msgs/Odometry.h>
#include<tf/transform_broadcaster.h>

// ROS Control核心头文件
#include <control_toolbox/pid.h>
#include <controller_interface/controller.h>
#include <hardware_interface/joint_command_interface.h>
#include <ros/ros.h>

namespace hero_chassis_controller {

class HeroChassisController : public controller_interface::Controller<hardware_interface::EffortJointInterface> {
 public:
  //四个核心函数
    bool init(hardware_interface::EffortJointInterface* hw, ros::NodeHandle& root_nh,ros::NodeHandle& controller_nh) override;
    void update(const ros::Time& time, const ros::Duration& period) override;
    void starting(const ros::Time& time) override;
    void stopping(const ros::Time& time) override;

 private:
    ros::Publisher odom_pub_;
    tf::TransformBroadcaster odom_broadcaster_;

    //里程计累积量（odom 坐标系）
    double odom_x_{0.0};
    double odom_y_{0.0};
    double odom_theta_{0.0};

    //四个轮子的关节句柄
    hardware_interface::JointHandle front_left_joint_;
    hardware_interface::JointHandle front_right_joint_;
    hardware_interface::JointHandle back_left_joint_;
    hardware_interface::JointHandle back_right_joint_;

    //四个轮子的 PID 控制器
    control_toolbox::Pid pid_front_left_;
    control_toolbox::Pid pid_front_right_;
    control_toolbox::Pid pid_back_left_;
    control_toolbox::Pid pid_back_right_;

    //力矩上限（N·m），从 yaml 的 effort_limit 读
    double effort_limit_{1.0};

    // /cmd_vel 指令（底盘坐标系），update() 里读取
    ros::Subscriber cmd_vel_sub_;
    double cmd_vx_{0.0};
    double cmd_vy_{0.0};
    double cmd_wz_{0.0};

    void cmdVelCallback(const geometry_msgs::Twist& msg);
};

}  // namespace hero_chassis_controller

#endif