表示机器的一种结构,只能表示分支树
```
base_footprint (根)
    └─ base_link (车体)
         ├─ wheel_lb
         ├─ wheel_rb
         ├─ wheel_lf
         └─ wheel_rf
```

## <link>
表示一个零件，包含visusl .collision（碰撞）,inertial(惯性)
<link name="base_link">
    <!-- 1. 外观：RViz 里看到的 -->
    <visual>
        <geometry>
            <box size="0.4 0.3 0.1"/>   <!-- 长 宽 高 -->
        </geometry>
        <origin xyz="0 0 0" rpy="0 0 0"/>  <!-- 相对 link 原点的偏移 -->
        <material name="blue">
            <color rgba="0 0 1 1"/>     <!-- 红绿蓝透明度 -->
        </material>
    </visual>
    <!-- 2. 碰撞：Gazebo 物理引擎用的，形状通常和 visual 一样 -->
    <collision>
        <geometry>
            <box size="0.4 0.3 0.1"/>
        </geometry>
    </collision>
    <!-- 3. 惯性：Gazebo 算物理用的，必须有，否则仿真会警告或飞走 -->
    <inertial>
        <mass value="5.0"/>   <!-- 质量 kg -->
        <inertia ixx="0.05" ixy="0" ixz="0" iyy="0.08" iyz="0" izz="0.1"/>
    </inertial>
</link>
其中：
#### geometry

| 形状  | 写法                                  | 参数含义   |
| --- | ----------------------------------- | ------ |
| 长方体 | `<box size="x y z"/>`               | 长宽高（米） |
| 圆柱  | `<cylinder radius="r" length="l"/>` | 半径、高   |
| 球   | `<sphere radius="r"/>`              | 半径     |
## \<joint>
用来连接两个 /<link>
<joint name="joint_lb" type="continuous">
    <parent link="base_link"/>       <!-- 父 -->
    <child link="wheel_lb"/>         <!-- 子 -->
    <origin xyz="-0.15 0.15 -0.05" rpy="0 0 0"/>  <!-- 子相对父的位置 -->
    <axis xyz="0 1 0"/>              <!-- 绕哪个轴转 -->
    <limit effort="10.0" velocity="5.0" lower="-1.57" upper="1.57"/>
</joint>

### 连接类型

| type         | 含义     | 用在哪     |
| ------------ | ------ | ------- |
| `fixed`      | 焊死，不能动 | 车体和虚拟底座 |
| `continuous` | 无限旋转   | 车轮      |
| `revolute`   | 有限角度旋转 | 机械臂关节   |
| `prismatic`  | 直线滑动   | 升降台     |
### \<limit>
#### 旋转关节

|属性|含义|单位|
|---|---|---|
|`effort`|**最大力矩**|N·m|
|`velocity`|**最大速度**|rad/s|
|`lower` / `upper`|角度范围|rad|
#### 移动关节（prismatic）
|属性|含义|单位|
|---|---|---|
|`effort`|最大推力|N|
|`velocity`|最大速度|m/s|
|`lower` / `upper`|位移范围|m|

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
	    <hardwareInterface>
		    hardware_interface/VelocityJointInterface
	    </hardwareInterface>
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

### 控制方式

| 接口                                          | 用途       | 典型控制器                     |
| ------------------------------------------- | -------- | ------------------------- |
| `hardware_interface/EffortJointInterface`   | **力矩控制** | `JointEffortController`   |
| `hardware_interface/VelocityJointInterface` | **速度控制** | `JointVelocityController` |
| `hardware_interface/PositionJointInterface` | **位置控制** | `JointPositionController` |
