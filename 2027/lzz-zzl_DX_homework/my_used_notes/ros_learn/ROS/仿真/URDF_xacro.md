基于XML的编程语言
通过xacro 工具转化为.urdf

转化方式（类似编译）
命令行：
```
xacro robot.xacro > robot.urdf
```
在launch中家加载
```
<param name="robot_description"
       command="$(find xacro)/xacro $(find your_pkg)/urdf/robot.xacro" />
```


## 语法

## 开头
任何 xacro 文件的第一行都必须给 `<robot>` 加上 `xmlns:xacro`：
```xml
<?xml version="1.0"?>
<robot xmlns:xacro="http://www.ros.org/wiki/xacro" name="my_robot">
    <!-- 内容 -->
</robot>


```
> 只有声明了这个命名空间，下面所有 `<xacro:xxx>` 标签才会被识别。  
> 普通 URDF 标签（`<link>`、`<joint>`）照常写，不用改

## 定义变量
```xml
<xacro:property name="wheel_radius" value="0,1" />
```
键值对
使用时
```
xxx = ${wheel_radius}   
```
将替换为0.1

## \#{}
- 里面是一个py表达式
- 可以进行运算
- xacro 是**顺序解析**的：变量必须在**使用之前**定义，否则报错

## macro:可复用的代码块
```xml
<xacro:macro name ="a_macro" params="arg1 arg2">  
<!--  放urdf代码片段  <-->     
	<link name="${arg1}"> ... </link>
    <joint name="joint_${arg1}"> ... </joint>
</xacro:macro>
```
- `name`：宏名，调用时用。
- `params`：参数列表，空格分隔。
### 调用
```xml
<xacro:my_macro arg1="wheel_lb" arg2="0.17"/>
```
### 默认参数
```xml
<xacro:macro name="wheel" params="prefix x y:=0 z:=0">
```
`:=` 表示默认值。调用时不传 `y` 就用 0。

## include
包含其他文件，使用里面定义的macro和property

## if/unless:条件判断
```xml
<xacro:if value="${use_gpu}">
    <!-- 条件为真时展开 -->
</xacro:if>

<xacro:unless value="${use_gpu}">
    <!-- 条件为假时展开 -->
</xacro:unless>
```


## 加载进Rviz
```
<launch>  
    <param name="robot_description"  
           textfile="$(find learnModel)/description/first_v1.urdf"/>  
<!--           command="$(find xacro)/xacro $(find learnModel)/description/xacro/first_v1.xacro"/>-->  
  
  
    <node name="joint_state_publisher_gui" pkg="joint_state_publisher_gui" type="joint_state_publisher_gui"/>  
    <node name="robot_state_publisher" pkg="robot_state_publisher" type="robot_state_publisher"/>  
    <node name="rviz" pkg="rviz" type="rviz"/>  
</launch>
```

加载进gazebo
```
  
<launch>  
<!-- 1. 加载 URDF 到参数服务器 -->  
<!--<param name="robot_description" textfile="$(find learnModel)/description/first_v1.urdf"/>-->  
      
    <param name="robot_description"  
           textfile="$(find learnModel)/description/first_v1.urdf"/>  
      
<!-- 2. 启动 Gazebo 空世界 -->  
<include file="$(find gazebo_ros)/launch/empty_world.launch">  
    <arg name="paused" value="false"/>  
    <arg name="use_sim_time" value="true"/>  
    <arg name="gui" value="true"/>  
    <arg name="headless" value="false"/>  
</include>  
  
<!-- 3. 将机器人模型生成到 Gazebo 中 -->  
<node name="spawn_urdf" pkg="gazebo_ros" type="spawn_model"  
      args="-param robot_description -urdf -model first_v1 -x 0 -y 0 -z 0.05"/>  
  
<!-- 4. 启动 robot_state_publisher (Gazebo 仿真中不需要 joint_state_publisher_gui) -->  
<node name="robot_state_publisher" pkg="robot_state_publisher" type="robot_state_publisher">  
    <param name="use_sim_time" value="true"/>  
</node>  
</launch>
```