## .launch

### xml语言:
#### 设计目的:用于描述数据是什么, 适合作为配置文件使用
#### 语法特点:
##### 语法严格, 使用成对的开始和结束标签, 如<name> ... </name>, 也可将没有内容的尾部标记省略: <name ... />, 大标签嵌套小标签
成对标签式写属性:
``` xml
<node>
  <pkg>turtlesim</pkg>
  <type>turtlesim_node</type>
  <name>my_turtle</name>
  <output>screen</output>
</node>
```
省略后标签:
```xml
<node pkg="turtlesim" type="turtlesim_node" name="my_turtle" output="screen"/>
```


### .launch文件结构:
##### <launch>是根元素, 最常用的标签是<node>, 用于指定启动哪个功能包(pkg)里面的哪个可执行文件(type), 以及给这个节点起什么名字(name)
##### pkg即包名, type是CMakeLists.txt里面add_executable(可执行文件名, src/源文件)的可执行文件名

```xml

<launch>

    <node pkg="pub_pkg" type="pub_pkg_p1_node" name="p1_node" output="screen"/>

    <node pkg="pub_pkg" type="pub_pkg_p2_node" name="p2_node" launch-perfix="gnome-terminal -e"/>

    <node pkg="sub_pkg" type="sub_pkg_node" name="s_node" output="screen"/>

</launch>

```

### 添加启动属性
#### 让节点cout打印的信息显示在屏幕上:output = "screen" , ROS_WARN()则不需要这个属性, ="log"则是打印在日志
```xml
<node pkg="sub_pkg" type="sub_pkg_node" name="s" output="screen"/>
```

#### 让一个节点单独在终端运行:launch-prefix="终端名", 这个动作会占用已打开的一个tmux窗口来打印内容
```xml
<node pkg="sub_pkg" type="sub_pkg_node" launch-prefix="tmux new-window"/>
```



### 使用.launch启动多个节点
start.launch文件可以放在任意一个软件包内, 可以在pkg文件夹内新建一个launch文件夹, 启动后就会在搜索前目录下其他的软件包并启动节点
使用.launch文件在启动任意一个节点的时候, 如果roscore没有启动, 都会自动启动roscore
#### 运行.launch文件指令
    roslaunch 包名 文件名.launch
    roslaunch rosetestsub_pkg test.launch

