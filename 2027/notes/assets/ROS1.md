# 第一部分：从零开始，创建并跑通一个新容器

## 1. 宿主机终端（绿色字）操作：

``` bash
# 1. 允许 Docker 访问你的屏幕（每次重启电脑后都要敲）
xhost +local:docker

# 2. 创建并进入带图形界面的新容器（注意镜像名和容器名）
sudo docker run -it --env="DISPLAY=$DISPLAY" --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" --name=ros_noetic osrf/ros:noetic-desktop-full /bin/bash  
```

*(敲完这个命令，你就已经身在容器内部了，提示符变成 `root@...#`)*

## 2. 初始化设置：

```bash
# 1. 确保在 bash 环境（防止 source 报错）
bash

# 2. 激活 ROS 环境
source /opt/ros/noetic/setup.bash

# 3. 把激活命令写进配置，以后进容器自动加载，不用每次敲
echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
```

## 3. 开始ROS 开发：

```bash
# 创建你的工作空间
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws
catkin_make
```

# 第二部分：日常使用已有容器（开机/重启电脑后）

## 1. 启动容器：

在你的 Ubuntu 宿主机终端输入：

```bash
sudo docker start ros_noetic
```

## 2. 进入容器：

```bash
sudo docker exec -it ros_noetic /bin/bash
```

*(如果发现新终端没有 Tab 补全或者找不到命令，顺手敲个 `bash` 即可)*

## 3. 运行你的程序：

在容器内，因为之前做过 `.bashrc` 配置，环境通常会自动加载。如果不放心，可以敲一次 

```bash
source /opt/ros/noetic/setup.bash
```

```bash
# 终端 1：启动大脑
roscore

# 终端 2：进入容器后，运行你的节点或小海龟
rosrun turtlesim turtlesim_node
```

# 第三部分：停止容器

## 1. 在容器内停掉程序：

在跑着程序的终端里，按 `Ctrl + C`。把所有跑着程序的终端都停掉。

## 2. 退出容器（出门）：

在容器终端里输入：

```bash
exit
```

*(终端提示符变回你原来的电脑名字)*

## 3. 停止容器（关机省资源）：

在你的 Ubuntu 宿主机终端输入：

```bash
sudo docker stop ros_noetic
```

# 常见报错速查表（遇到问题看这里）

1. **`permission denied`**：忘了加 `sudo`。
2. **`container ... is not running`**：容器没启动，先敲 `sudo docker start ros_noetic`。
3. **`command not found`（如 catkin_make / rosrun）**：没 `source` 环境，或者没进容器。
4. **`could not connect to display`**：重启电脑后忘了敲 `xhost +local:docker`，或者重新创建容器时没加 `--env="DISPLAY=$DISPLAY"` 等参数。
5. **`Failed to contact master`**：没开 `roscore`，或者开了没连上