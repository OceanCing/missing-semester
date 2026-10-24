## 启动配置好的设置
```xml
<node pkg="rviz" type="rviz" name="rviz" args="-d $(find 你的包名)/rviz/你的配置.rviz" />
```


## Rviz显示
### tf话题
|话题|消息类型|用途|
|---|---|---|
|`/tf`|`tf2_msgs/TFMessage`|**动态变换**（会随时间变化，如轮子转动、odom→base_link）|
|`/tf_static`|`tf2_msgs/TFMessage`|**静态变换**（固定不变，如 base_footprint→base_link）|
RViz 内部通过 `tf2_ros::TransformListener` 订阅这两个话题，**无论你的机器人用了什么命名空间，TF 永远都在 `/tf` 和 `/tf_static` 下**。

##### 话题发布者
**1. `robot_state_publisher`**

- 订阅 `/joint_states`
    
- 根据 URDF 里的关节定义，计算并发布 **`base_link → wheel_lf`、`base_link → wheel_lb`** 等动态 TF 到 `/tf`
    
- 同时发布 URDF 里 fixed joint 的静态 TF（如 `base_footprint → base_link`）到 `/tf_static`
**2. `diff_drive_controller`**

- 根据轮子编码器（仿真里是 Gazebo 物理引擎）计算里程计
    
- 发布 **`odom → base_footprint`** 到 `/tf`