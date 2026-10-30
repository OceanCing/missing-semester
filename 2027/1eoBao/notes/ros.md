## ROS笔记
# rosrun
- rosrun ros_pkg ros_node _arg1=param --argc argv的使用 参数服务器全局参数设置
  
# rosinit
- \param options 节点启动选项，被封装进了ros::init_options  

1.ros::init_options::AnonymousName：在节点名后加随机后缀，允许启动多个同名节点。

# publisher
- Publisher advertise(const std::string& topic, uint32_t queue_size, bool latch = false)  
  latch=false:不保存最后发布消息 true：保存
    作用：发布低频不变消息如地图

# 时间
- ros::Time right_now = ros::Time::now();//将当前时刻封装成对象
- ROS_INFO("当前时刻:%.2f",right_now.toSec());//获取距离 1970年01月01日 00:00:00 的秒数  
ROS_INFO("当前时刻:%d",right_now.sec);//获取距离 1970年01月01日 00:00:00 的秒数
- ros::Duration du(10);//持续10秒钟,参数是double类型的，以秒为单位   du.sleep();//按照指定的持续时间休眠
- 循环和休眠  

      ros::Rate r(20);
      r.sleep();

# 服务通信
- 编辑配置文件
package.xml中添加编译依赖与执行依赖

      <build_depend>message_generation</build_depend>  
      <exec_depend>message_runtime</exec_depend>  
      <!-- 
      exce_depend 以前对应的是 run_depend 现在非法
      -->
- CMakeLists.txt编辑 srv 相关配置

      find_package(catkin REQUIRED COMPONENTS
        roscpp
        rospy
        std_msgs
        message_generation
      )
      # 需要加入 message_generation,必须有 std_msgs
      
      add_service_files(
        FILES
        AddInts.srv
      )
      generate_messages(
        DEPENDENCIES
        std_msgs
      )
- 定义srv文件
  服务通信中，数据分成两部分，请求与响应，在 srv 文件中请求和响应使用---分割，具体实现如下:

  功能包下新建 srv 目录，添加 xxx.srv 文件，内容:

      # 客户端请求时发送的两个数字
      int32 num1
      int32 num2
      ---
      # 服务器响应发送的数据
      int32 sum  

# nodelet
- 