# 自定义控制器编写思路

控制器可以被实现为插件并封装在动态库中，pluginlib提供了动态加载插件（即动态库）的功能，进而实现了ROS节点能够在运行时动态地获取并使用控制器的功能。

### 创建：需按规范编写一系列文件

#### .h和.cpp文件

写控制器要继承一个基类

```cpp
class SimpleChassisController : public controller_interface::Controller<hardware_interface::EffortJointInterface> {}
```

写控制器的基本接口，其中init、update、starting、stop四个函数必须要有，因为ros_control中使用控制器的方法就是：加载控制器的时候调用一次init、开启控制器时调用startinig、然后controller manager按一定频率一直调用update、停止控制器调用stop

```cpp
bool init(hardware_interface::EffortJointInterface *effort_joint_interface, ros::NodeHandle &root_nh,
          ros::NodeHandle &controller_nh);
void starting(const ros::Time& time);
void update(const ros::Time& time, const ros::Duration& period);
void stop(const ros::Time& time, const ros::Duration& period);
```

#### 修改CMakeLists.txt

```cmake
//添加以下两句
## Declare cpp executables
add_library(${PROJECT_NAME}
        src/MyController.cpp
        )

## Specify libraries to link executable targets against
target_link_libraries(${PROJECT_NAME}
        ${catkin_LIBRARIES}
        )

//如果想要调用其他功能包的头文件，添加以下
    find_package(catkin REQUIRED
        COMPONENTS
        other_package
        )
```

#### 控制器注册为插件

##### 只有将控制器注册为插件，这个控制器才能被controller_manager加载

1.在.cpp文件最后一行加入以下代码

```cpp
PLUGINLIB_EXPORT_CLASS(simple_chassis_controller::SimpleChassisController,
                       controller_interface::ControllerBase)
//注意加入头文件
#include <pluginlib/class_list_macros.hpp>
```

2.修改package.xml（加入以下代码）

```xml
<export>
    <controller_interface
            plugin="${prefix}/pkg_name_plugins.xml"/>
</export>
```

3.编写插件描述文件pkg_name_plugins.xml（内容如下）

- path:动态库路径

- name：插件名称（包名/类名）

- type：插件类型（带命名空间）
```xml
<library path="lib/libsimple_chassis_controller">
    <class name="pkg_name/class_name"
           type="pkg_name::class_name"
           base_class_type="controller_interface::ControllerBase">
        <description>
              ...
        </description>
    </class>
</library>
```



### 加载：launch文件, 配置文件

```xml
<launch>
  <rosparam file="$(find simple_chassis_controller)/config/controllers.yaml" command="load"/>
  <!-- load the controllers -->
  <node name="controller_spawner" pkg="controller_manager" type="spawner" respawn="false"
        output="screen" args="
        controller/simple_chassis_controller
        controller/joint_state_controller
  "/>
</launch>
```

controller_spawner（一个ROS节点）向controller_manager（ROS中用于管理机器人控制器的核心组件）发送请求，告知要加载的控制器类型和参数等信息，controller_manager接到请求后执行具体操作并将控制器纳入管理。