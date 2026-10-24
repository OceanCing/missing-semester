格式
[[urdf|urdf]]

## 作用
- 放机器模型
- 被加载进Rviz和gazebo中
- 存放gazebo插件，用于控制模型关节


## ros_control
`ros_control` 是 ROS 的一个**控制框架**，它把：

text

上层应用（你的控制程序 / 导航 / 手柄）
        ↓ 发指令
     ros_control
        ↓ 转成硬件指令
     Gazebo（仿真） / 真实电机（硬件）

统一起来，让你**不用关心底层是仿真还是真实硬件**，代码基本一样

## `<transmission>` 传动定义

`<transmission>` 是 **URDF 和 ros_control 之间的桥梁**，告诉 ros_control：

- 哪个关节要被控制
- 用什么硬件接口
- 对应哪个执行器
### 基本结构


xml

<transmission name="wheel_lb_trans">
    <type>transmission_interface/SimpleTransmission</type>
    <joint name="joint_lb">
        <hardwareInterface>hardware_interface/VelocityJointInterface</hardwareInterface>
    </joint>
    <actuator name="wheel_lb_motor">
        <mechanicalReduction>1</mechanicalReduction>
        <hardwareInterface>hardware_interface/VelocityJointInterface</hardwareInterface>
    </actuator>
</transmission>

| 标签                                | 作用                            |
| --------------------------------- | ----------------------------- |
| `<type>`                          | 传动类型，固定写 `SimpleTransmission` |
| `<joint name="...">`              | 要控制的关节名                       |
| `<hardwareInterface>`             | **硬件接口类型**（关键！）               |
| `<actuator>`                      | 执行器（电机）定义                     |
| `<mechanicalReduction>`           | 减速比，仿真写 1                     |
| `<hardwareInterface>`（actuator 内） | 与 joint 内保持一致                 |
|                                   |                               |

|参数|值|类别|作用|
|---|---|---|---|
|`kp`|1e7|**接触刚度（弹性）**|轮子-地面接触的“硬度”|
|`kd`|1.0|接触阻尼|接触振动衰减|
|`mu1`/`mu2`|0.5|摩擦|切向防滑|
|`minDepth`|0.005|接触约束|允许穿透深度|
|`maxVel`|100.0|接触约束|摩擦约束速度上限|
|`damping`|1.0|关节阻尼|关节粘滞阻力|
|`friction`|0.2|关节摩擦|关节库仑摩擦|

### 控制方式

|接口|用途|典型控制器|
|---|---|---|
|`hardware_interface/EffortJointInterface`|**力矩控制**|`JointEffortController`|
|`hardware_interface/VelocityJointInterface`|**速度控制**|`JointVelocityController`|
|`hardware_interface/PositionJointInterface`|**位置控制**|`JointPositionController`|

## 总流程
### 放模型树
### 定义\<transmission>
将关节绑定执行器（电机）
### 加gazeb插件参数

