**在师兄笔记原有基础上添加方便自己理解的注释**
# 1. XML

XML 是一种用来传输和存储数据的标记语言。
在 ROS 中，XML 用来 
- 书写机器人统一描述文件 urdf，以及进一步的 xacro
- 实现进程间通信（XML-rpc）
在 rm_control 的代码中可以看见不少 XML 的身影，之后再来补充

**补充**
**注意**：XML 本身不是 ROS 节点间所有通信的协议。而是一种结构化标记与语言，适合描述层次化配置和数据。
ROS 1 中，XML-RPC 主要用于 ROS Master API、节点注册和参数服务器；话题和服务的实际数据传输主要走 TCPROS/UDPROS。

ROS 中常见的 XML 用途可以分成这几类：
- `URDF`：描述机器人连杆、关节、惯量、碰撞和视觉模型。
- `xacro`：用 XML 宏生成 URDF，避免重复描述。
- `.launch`：描述要启动的节点、参数、命名空间和 include 关系。
-` package.xml`：包元数据和依赖声明。
- 插件 XML：例如 `simple_chassis_controller_plugins.xml`，告诉` pluginlib` 动态库在哪里、类名是什么、基类是什么。
- `XML-RPC：ROS Master` 提供 API，节点通过它注册、查询参数和获取系统状态。
`controllers.yaml` 不是 XML，而是 YAML；它负责控制器参数，不参与 URDF 建模。

在 `rm_control` 里看到 XML 时，先判断它属于哪一类：
1. 如果是 `.urdf.xacro`：在看机器人模型；
2. 如果是 `plugin.xml`：在看插件注册；
3. 如果是 `.launch`：在看节点启动流程；
4. 如果是 `XmlRpc` 相关代码：在看 ROS Master 或参数服务器通信。

XML 语法的核心是树结构，以下面的 urdf 文件为例：
```XML
<?xml version="1.0" encoding="UTF-8"?>
<robot name="myfirst">
  <link name="base_link">
    <visual>
      <geometry>
        <cylinder length="0.6" radius="0.2"/>
      </geometry>
    </visual>
  </link>
  <link name="link1">
    ...
  </link>
</robot>
```
`robot` 是根，随后有 `base_link` 和 `link` 两个分叉，分叉往后又有别的枝叶。
这里引出**元素**和**属性**的概念：
- 以上面的文件为例，`robot`, `link`, `visual` 等就是元素。元素指的是从（且包括）开始标签直到（且包括）结束标签的部分。一个元素可以包含：
	- 其他元素
	- 文本
	- 属性
	- 或混合以上所有...
- 在元素后面的，如 `name`, `radius` 是属性。属性的数值一定要用双引号围着，不同的属性用空格分隔

进一步看该文档的整体结构，最开始的 `<?xml version="1.0" encoding="UTF-8"?>` **声明**了文件的 XML 版本和所使用的编码，这是可选的。
下一行就是文件的**根元素**，说明了本文档描述了一个机器人。元素的属性 `name` 说明了机器人名称。
需要明确的是，正如一棵树只有一个根，一个 XML 文件需要一个包裹一切（除了声明）的**根元素**，它是**所有其他元素的父元素**。

所有的 XML 元素一般都有一个**关闭标签**。以 visual 为例：有代表开始的 `<visual>`，以及代表结束的 `</visual>`
但 XML 也允许单标签的使用，写法如下：
```xml
<elementName attribute="value" />
```

现在我们回到 XML 的目的，用来存储和传输数据，通过以上的学习，可以尝试用一个 XML 文件来描述一个图书馆的数据：

首先我们需要一个根元素，想必就是 library 了
```xml
<library>
</library>
```
里面需要一些书：
```xml
<library>
	<book name="Harry Potter">
	</book>
	<book name="Learning XML">
	</book>
</library>
```
我还想记录书的作者，出版年份等信息：
```xml
<library>
	<book name="Harry Potter">
		<author>J K. Rowling</author>
		 <year>2005</year> 
		 <price>29.99</price>
	</book>
	<book name="Learning XML">
		<author>Erik T. Ray</author>
		 <year>2003</year>
		  <price>39.95</price>
	</book>
</library>
```
很好，就是如此，我们得到了一个形式良好的 XML 文件，记录了图书馆的数据。
请记住以下书写形式良好的 XML 文件的要求：
- XML 文档必须有一个根元素
- XML元素都必须有一个关闭标签
- XML 标签对大小写敏感
- XML 元素必须被正确的嵌套
- XML 属性值必须加引号

# 2 .URDF
[官方 URDF 文档](https://docs.ros.org/en/rolling/Tutorials/Intermediate/URDF/URDF-Main.html)
[翻译 URDF 文档](http://fishros.org/doc/ros2/humble/Tutorials/Intermediate/URDF/Building-a-Visual-Robot-Model-with-URDF-from-Scratch.html)
虽然上述文档是 ros 2 的，但他们在 ros 1 中同样适用
**[URDF XML 规范总览主页 (ROS Wiki)](https://www.google.com/url?sa=i&source=web&rct=j&url=https://wiki.ros.org/urdf/XML&ved=2ahUKEwjS27WjufSRAxUmdPUHHSCMIwMQy_kOegYIAQgEEAE&opi=89978449&cd&psig=AOvVaw3Q4IrZos3kwx6nIVhHvbeJ&ust=1767704155762000)**：该页面列出了构成机器人模型的所有核心 XML 元素。
[古月居的一篇记录帖](http://dev.guyuehome.com/detail?id=1825483221320433665)
[xacro文档](https://wiki.ros.org/xacro)

要写一个机器人，主要是以下的流程：
- 创建一个根元素 `<robot>`
- 为机器人创建很多 `link`：`base_link` 一般作为车的底盘，接着 1 轮子是一个 `link `，云台是一个 `link`...类比人体，link 就是大腿、小腿之类的部位
- 接着为机器人创建关节 joint，joint 会连接两个 link，就像膝盖连接着大腿和小腿
- 为 joint 创建 transmission，这是控制器控制机器人的关键
- 还有一个 gazebo 拓展标签

## 2.1 根元素 `<robot>`

根元素包裹着其他所有元素，有一个 name 属性，代表我们机器人的名字
值得注意的是，如果要用 xacro，则需要加上 `xmlns:xacro="http://www.ros.org/wiki/xacro"` 这一属性。
综上，根元素的一个例子是：
```xml
<robot name="my_robot" xmlns:xacro="http://www.ros.org/wiki/xacro">

</robot>
```

## 2 .2 `<link>`
描述具有惯性、视觉特征和碰撞属性的刚体（由这句话我们可以知道 link 元素内部就是这几个东西），如下图所示
![[Pasted image 20260107171650.png]]
这里给出一个 link 的例子：
```xml
<link name="gimbal">   //定义一个名为gimbal的（万向节/云台）
  <inertial>
    <origin xyz="0.0 0.0 0.0" rpy="0.0 0.0 0.0"/> //定义质心在哪里
    <mass value="0.1"/> //定义质量是多少
    <inertia ixx="0.0000175" ixy="0.0" ixz="0.0" iyy="0.0000175" iyz="0.0" izz="0.00002"/>
  </inertial> //开始定义3*3的转动惯量矩阵
  <visual name=""> //只是一个视觉标签，在没有加额外视觉贴图模型的情况下不影响正常加载。
    <origin xyz="0.0 0.0 0.0" rpy="0.0 0.0 0.0"/> //Roll(横滚角 x轴） Pitch（俯仰角 y轴） Yaw（偏航角 z轴）
    <geometry> //定义几何形状
      <cylinder length="${gimbal_height}" radius="${gimbal_radius}"/> //定义形状为圆柱体
    </geometry>
    <material name="white"/> //定义颜色
  </visual>
  <collision>
    <origin xyz="0.0 0.0 0.0" rpy="0.0 0.0 0.0"/>
    <geometry>
      <cylinder length="${gimbal_height}" radius="${gimbal_radius}"/>
    </geometry>
  </collision>
</link>
```

### 2.2.1 惯性 `<inertial>`

本元素描述了物体的物理属性
`<origin>` 描述了物体质心相对于这个 link 坐标系的偏移，我们一般创建的都是标准的几何体，质心就在原点，所以全填 0 就好
`<mass>` 描述了物体的质量，urdf 中所有的数值单位都是国际单位，这里为 kg
`<inertia>` 3x3 转动惯量矩阵，我这次是让 ai 生一份填进去。以下引用嗣音的笔记内容：
> 我对这个没什么研究，只是平时都将ixx、iyy、izz设置为0.1或者0.001，如果仿真模型加载到gazebo里后原地乱飞就得想想这个是不是设置太小了。

### 2.2.2 视觉 `<visual>`

本元素描述了物体的视觉属性
`<origin>` 描述了**视觉几何元素的坐标系相对于这个link坐标系的位姿**，这个一般也全填写 0。刚开始尝试时我选择修改并统一 visual 和 collision 元素的 origin 元素来修改物体位置，但是这样并不是好的实现，应当在 joint 的 origin 元素里定义

`<geometry>` 描述了这物体看起来是个什么形状，有几个可选标签：
- box（立方体）
- cylinder（圆柱）
- sphere（球体）
- mesh（自定义文件）
几何形状的具体参数在属性里给出：
```xml
<cylinder length="${gimbal_height}" radius="${gimbal_radius}"/> 
```
这里使用了一点 xacro

`<material>` 描述了物体的外观材料
在 urdf 前半部分我们可以统一定义各种颜色，如：
```xml
<material name="white">
  <color rgba="1 1 1 1"/>
</material>
```
我们给 `<material>` 元素设置属性 `name="while"`，在 gazebo 以及 rviz 里这看起来就是白色的了。
还可以从文件加载材质。

### 2.2.3  碰撞箱 `<collision>`

`<origin>` 描述了**碰撞箱几何元素的坐标系相对于这个 link 坐标系的位姿**，这个一般也全填写 0。

`<geometry>` 描述了这物体碰撞箱是个什么形状，设置的与visual一样即可。
> 我们可以通过此项来设置安全区，假设有个机器人头部是半球形，我们希望整个区域都不能靠近，那么可以把碰撞箱设置得更大，如设置为一个长方体

## 2 .3 `<joint>`

joint是链接link的关节，如下图所示，joint 描述了连接的 link 分别是谁，link 旋转的方向以及最为关键的，被之相连的 link 之间的坐标变换。
![[Pasted image 20260107180616.png]]
以下给出一个 joint 例:
```xml
<joint name="gimbal_joint" type="continuous"> //continuous代表无限旋转关节
  <origin xyz="0.035 0 0.15" rpy="0 0.0 0.0"/> //相对于父连杆定义位姿
  <parent link="base_link"/> 定义父连杆即相对固定的、起支撑作用的部件
  <child link="gimbal"/> //定义子连杆即相对运动的部件
  <axis xyz="0.0 0.0 1"/>  //定义绕哪个轴转
  <limit effort="1000" velocity="1000"/> //限制最大力矩和最大速度
</joint>
```

`type="continuous"` 给出了关节的类型，轮子就用这个。更多类型参[附录](#关节类型表 )

`<origin>` 描述的是**从父连杆（Parent Link）坐标系到子连杆（Child Link）坐标系**的静态空间变换。通俗理解就是，我们现在研究两个 link 的坐标原点，从父 link 原点出发，走过 xyz 描述的距离就到了子 link 的坐标轴原点。进一步按 rpy 旋转，两个坐标系就重合了。
综上，我们 link与 link的关系不用自身的视觉和碰撞箱的位姿描述，而应该用 joint的位姿描述。

`<axis>` 描述了关节的运动轴。具体来说，是定义在子 link 坐标系下的。表明了 joint 是沿哪个轴运动。
> xyz为一个正交向量。对revolute、continuous旋转关节为旋转轴，对prismatic滑动关节为平移轴，对planar平面关节为平面的法线。这个轴向是相对于两个link的接触面的。

## 2.4 `<tranmission>` //传动

定义了执行器（Actuator，如电机）与关节（Joint）之间的机械关系。
以下给出示例：
```xml
<transmission name="gimbal_trans"> //trans 是缩写也是传动
  <type>transmission_interface/SimpleTransmission</type> //为简单的一对一传动
  <joint name="gimbal_joint"> 
    <hardwareInterface>hardware_interface/VelocityJointInterface</hardwareInterface> //第一个表示硬件接口，第二个表示这个接口是ROS官方提供的标准接口，第三个表示速度关节接口，代表的是角速度。 
  </joint>
  <actuator name="gimbal_motor"> //定义电机名字
    <mechanicalReduction>1</mechanicalReduction> //减速比，n圈关节才转一圈
  </actuator>
</transmission>
```

` <type>transmission_interface/SimpleTransmission</type>` 表示电机和关节之间是简单的线性缩放关系

`<joint name="gimbal_joint">` 名字要匹配前面定义的 joint

**`<hardwareInterface>`** 定义了关节支持的控制模式：
- `EffortJointInterface`：力矩控制
- `VelocityJointInterface`：速度控制
- `PositionJointInterface`：位置控制

`<mechanicalReduction>` 减速比，这里填 n就表示执行器转 n 圈关节才转 1 圈。

## 2.5 gazebo插件

通过以下代码段包含 `libgazebo_ros_control.so` 插件，这将[硬件和 ros_control 连接在一起](#连接ros_control)：
```xml
<gazebo>
  <plugin name="gazebo_ros_control" filename="libgazebo_ros_control.so"> //给插件起名字，加入插件。
    <robotNamespace>/</robotNamespace> //命名空间，避免发送指令使多台机器人冲突。
  </plugin>
</gazebo>
```
# 3. xacro 
这是一个用来命名和封装的工具，可以减少许多重复的工作。比如在画差速小车的过程中，我通过 xacro定义了轮子，然后分别对左轮和右轮实例化。
我还通过 xacro定义了车辆的各种参数，减少计算量以及方便统一更改。

**属性定义** 
一般用于定义轮宽，轮半径之类的参数，定义方法为：
```xml
<xacro:property name="my_name" value="0.05" /> name 变量名，value是变量值，property是变量
```
调用的时候将其写为${my_name}，比如
```xml
<origin rpy="0 0 ${my_name}" xyz="0 0 0"/> 将0.05填充进去
```

**宏定义** 
类似于函数封装，定义方法为：
```xml
<xacro:macro name="default_inertial" params="mass"> //形参是mass，macro是函数
  <inertial>
    <mass value="${mass}" />
    <inertia ixx="0.1" ixy="0.0" ixz="0.0" iyy="0.1" iyz="0.0" izz="0.1" />
  </inertial>
</xacro:macro>
```

params可以包含多个参数，用空格隔开就行。也可以设置默认值，加上:=即可。 调用方法为
```xml
<xacro:default_inertial mass="0.0001"/>
```

使用宏的时候可以仔细思考参数如何设置，譬如对于对称于车体两端的轮子，可以设置一个 reflect参数等于 1 或 -1，然后在计算 joint 的位姿的时候乘上。

以下给出差速小车的宏用例：
```xml
<xacro:macro name="wheel" params="prefix reflect num"> //prefix前缀 reflect反射系数 num编号 
  <link name="${prefix}_wheel">
    <inertial>
      <origin xyz="0.0 0.0 0.0" rpy="0.0 0.0 0.0"/>
      <mass value="0.2"/>
      <inertia ixx="0.00014" ixy="0.0" ixz="0.0" iyy="0.00014" iyz="0.0" izz="0.00025"/>
    </inertial>
    <visual>
      <geometry>
        <cylinder length="${wheel_length}" radius="${wheel_radius}"/>
      </geometry>
      <material name="white"/>
    </visual>
    <collision>
      <geometry>
        <cylinder length="${wheel_length}" radius="${wheel_radius}"/>
      </geometry>
    </collision>
  </link>

  <joint name="wheel_${num}_joint" type="continuous">
    <origin xyz="0.035 ${reflect*(width/2+wheel_length/2)} 0.1" rpy="${-pi/2} 0.0 0.0"/>
    <parent link="base_link"/>
    <child link="${prefix}_wheel"/>
    <axis xyz="0.0 0.0 1"/>
    <limit effort="1000" velocity="1000"/>
  </joint>

  <transmission name="wheel_${num}_trans">
    <type>transmission_interface/SimpleTransmission</type>
    <joint name="wheel_${num}_joint">
      <hardwareInterface>hardware_interface/VelocityJointInterface</hardwareInterface>
    </joint>
    <actuator name="wheel_${num}_motor">
      <mechanicalReduction>1</mechanicalReduction>
    </actuator>
  </transmission>
</xacro:macro>

<xacro:wheel prefix="left" num="0" reflect="1"/>
<xacro:wheel prefix="right" num="1" reflect="-1"/>
```
 这样通过 xacro，我们很方便地完成了两个轮子 link，joint和 transmission的定义。

xacro还有别的用法，参[文档](https://wiki.ros.org/xacro)

---
# 关节类型表
### 1. 固定关节 (Fixed)

- **描述**：不允许任何相对运动。父子连杆被“焊死”在一起。
- **适用范围**：
    - 将传感器（如雷达、摄像头）固定在底盘上。
    - 将机器人底座固定在世界坐标系（world）中。
    - 将复杂的机器人零件拆分成多个 Link 以便分配不同的质量或惯性。
### 2. 旋转关节 (Revolute)
- **描述**：绕着一个轴转动，但有**角度限制**（必须指定 `upper` 和 `lower` 限制）。
- **适用范围**：
    - 机械臂的关节（如手肘、肩膀）。
    - 舵机驱动的结构。
    - 有活动范围限制的摆动机构。
### 3. 连续旋转关节 (Continuous)
- **描述**：绕着一个轴转动，但**没有角度限制**。可以 360 度无限旋转。
- **适用范围**：
    - 轮式机器人的车轮。
    - 风扇叶片。
    - 某些可以无限旋转的工业转台。
### 4. 滑动关节 (Prismatic)
- **描述**：沿着一个轴进行**线性平移**，有行程限制。
- **适用范围**：
    - 升降台。
    - 数控机床（CNC）的 X/Y/Z 轴。
    - 伸缩杆结构。
### 5. 浮动关节 (Floating)
- **描述**：允许在 3D 空间中进行 6 个自由度（全向移动和全向旋转）的运动。
- **适用范围**：
    - 无人机（相对于世界坐标系的运动）。
    - 水下机器人。
    - 模拟未固定在地面上的物体。
### 6. 平面关节 (Planar)
- **描述**：允许在某个平面内进行平移和绕该平面法线的旋转。
- **适用范围**：
    - 在平地上移动且能自转的扫地机器人。
    - 在气垫桌上滑动的物体。

---
# 连接ros_control

加载 `libgazebo_ros_control.so` 插件实现了以下 4个核心功能
### 1. 解析 URDF 中的 Transmission 标签
- 这个插件会扫描整个 URDF，读取所有的 `transmission` 定义。
- 它会识别出哪些关节需要被控制，以及它们使用的是什么接口（位置、速度或力矩）。
### 2. 硬件抽象化（Hardware Abstraction Layer）
该插件充当了**虚拟硬件接口**。
- 在真实机器人上，你需要写驱动程序来读写电机的编码器值和电压。
- 在 Gazebo 中，这个插件模拟了这些行为。它从 Gazebo 物理引擎获取关节状态（位置、速度、受力），并将其包装成 `ros_control` 能够理解的标准格式。
### 3. 连接 ROS Controller Manager
这是最关键的一步。插件启动后，会初始化一个 `controller_manager`。
- 它可以让你通过 ROS 服务（如 `switch_controller` 或 `load_controller`）动态地加载、启动或停止控制器。
### 4. 同步仿真时钟与控制频率
- **时间同步**：它确保控制循环与 Gazebo 的物理步进同步。如果仿真变慢了（Real Time Factor < 1），控制频率也会相应调整，防止控制器因为时间偏差而发散。
- **闭环控制**：它以特定的频率（通常在插件配置中设定，如 100Hz 或 1000Hz）执行：
    1. 从 Gazebo 读取当前关节状态。
    2. 调用 `ros_control` 计算控制输出。
    3. 将计算后的力矩/速度/位置指令写回 Gazebo 物理引擎。
