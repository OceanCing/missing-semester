配量启动节点,本质XML

```
<launch>  
  
    <node pkg="ssr_pkg" type="my_ssr_node" name="my_ssr_node"/>  
  
    <node pkg="atr_pkg" type="receive_node" name="receive_node" output="screen"/>  
  
</launch>
```
大致这个样

可以通过加标签
```
launch-prefix="gnome-terminal -e"
```
使节点在独立终端运行


## 语法
### 注释语法

<!-- 这是注释内容 -->
注释可以放在任何位置

### `<node>`
启动节点
```
<node name="节点名" pkg="功能包名" type="可执行文件名" [其他属性]/>
```
**常用属性说明：**

|属性|说明|示例|
|---|---|---|
|`name`|节点名称（必须唯一）|`name="robot"`|
|`pkg`|功能包名称|`pkg="gazebo_ros"`|
|`type`|可执行文件名|`type="spawn_model"`|
|`args`|命令行参数|`args="-x 1.0 -y 2.0"`|
|`output`|输出重定向|`output="screen"`（显示到终端）|
|`respawn`|崩溃后自动重启|`respawn="true"`|
|`required`|必须运行，否则终止launch|`required="true"`|

### `args` 参数详解

**spawn_model命令的参数格式：**

xml

args="-file 模型文件路径 -x X坐标 -y Y坐标 -z Z坐标 -Y 偏航角 -urdf -model 模型名称"

参数说明：

- `-file`：模型文件路径
    
- `-x, -y, -z`：三维空间位置（米）
    
- `-Y`：偏航角（弧度），绕Z轴旋转
    
- `-urdf`：指定使用URDF格式
    
- `-model`：赋予模型的名称


### `<include>` 标签 - 文件包含

**语法结构：**

xml
```
<include file="$(find 包名)/路径/文件名.launch">
  <arg name="参数名" value="参数值"/>
</include> 
```
**示例解析：**

xml
<include file="$(find gazebo_ros)/launch/empty_world.launch">
  <arg name="world_name" value="$(find wpr_simulation)/worlds/wpb_simple.world"/>
  <arg name="paused" value="false"/>
  <arg name="use_sim_time" value="true"/>
  <arg name="gui" value="true"/>
</include>
- `$(find 包名)`：ROS的路径查找命令，自动定位功能包路径
- `<arg>`：向被包含的launch文件传递参数
- 作用：复用其他launch文件的配置