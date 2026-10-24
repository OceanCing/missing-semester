## 加载进Rviz
### urdf
```
<launch>  
    <param name="robot_description"  
           textfile="$(find learnModel)/description/first_v1.urdf"/>  
    <node name="joint_state_publisher_gui" pkg="joint_state_publisher_gui" type="joint_state_publisher_gui"/>  
    <node name="robot_state_publisher" pkg="robot_state_publisher" type="robot_state_publisher"/>  
    <node name="rviz" pkg="rviz" type="rviz"/>  
</launch>
```


## 加载进gazebo
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

## 步骤流程

### 加载机器模型文件到参数服务器
```xml
<param name="robot_description"  
       textfile="$(find learnModel)/description/first_v1.urdf"/>
```
- `name="robot_description"`：参数服务器上的参数名
- `textfile="..."`：表示读取该文件的**文本内容**，并作为参数值存进去
### 启动gazebo
空世界
```xml
<include file="$(find gazebo_ros)/launch/empty_world.launch">  
    <arg name="paused" value="false"/>  
    <arg name="use_sim_time" value="true"/>  
    <arg name="gui" value="true"/>  
    <arg name="headless" value="false"/>  
</include>
```
### 加载模型进gazebo
```xml
  
<!-- 3. 将机器人模型生成到 Gazebo 中 -->  
<node name="spawn_urdf" pkg="gazebo_ros" type="spawn_model"  
      args="-param robot_description -urdf -model first_v1 -x 0 -y 0 -z 0.05"/>
```

### 加载控制器 YAML
```xml
<rosparam file="$(find learnModel)/config/my_robot_control.yaml" command="load"/>
```
### 使用仿真时间
```xml
<node name="robot_state_publisher" pkg="robot_state_publisher" type="robot_state_publisher">  
    <param name="use_sim_time" value="true"/>
```
