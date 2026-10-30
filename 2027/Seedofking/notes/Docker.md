# Docker命令

### 拉取镜像
`docker pull xxx`
`docker pull hello-world`
    xxx: docker.n8n.io/n8nio/n8n  作者名/镜像名/(版本)

##### 若拉取超时, 可尝试添加镜像站
```
    sudo tee /etc/docker/daemon.json > /dev/null <<'EOF'
    {
    "registry-mirrors": [
        "https://docker.m.daocloud.io/",
        "https://docker.1panel.live/",
        "https://hub.rat.dev/"
    ]
    }
    EOF
```

### 状态查看

#### 显示正在运行的容器, 并列出一些命令
`docker ps`
    正在运行的容器会显示在底部

#### 显示所有容器, 包括不在运行的
`docker ps -a`

#### 显示已有的镜像
`docker image ls`


##### 官方仓库Docker Hub 也可以搜索镜像, 并找到拉取镜像的命令

### docker run 创建容器命令

#### 简单运行:
`docker run hello-world`

#### 添加参数的运行:
`docker run -p xxx -v xxx 容器镜像名`
`--name=指定创建的容器名`

#### 端口映射:
`docker run -p 80:80`
主机的网络默认与容器内的网络是隔离的, 需要进行端口映射80:80意为将宿主机80端口映射到容器的80端口
以后通过localhost:80访问宿主机端口的时候, 把宿主的80端口转发到容器内的80端口进行处理
这样操作之后, 访问宿主机端口的时候就相当于是访问容器的端口

#### Volumn通道
`-v 主机同步目录:从机同步目录`
`-v /home/seed:/home/seed`
主机与容器间同步的文件, 这样主机更改的文件也会修改到容器, 容器修改的文件也会保存到主机
容器删除之后容器里的那份文件也会被删除, 但同步的文件在容器删除之后仍然会保存在宿主机

#### Device挂载设备, 使得容器能访问宿主机的物理设备
`--device=/dev/dri/renderD128`



#### 指定被实例化的镜像名
`wpr_sim_image:gpu`     

### 删除操作

#### 删除容器
`docker rm 容器id`
`docker rm 51ccf2846a95`

#### 强制删除正在运行的容器
`doker rm -f 容器id`

#### 删除镜像
`docker rmi 镜像名`

## 创建ros容器过程:

#### 1.让图形化可用:
###### 将xhost +SI:localuser:root添加进宿主机.bashrc

#### 2.挂载工作空间:
>在宿主机建立目录, 这样在容器里 /root/catkin_ws 下的代码，在宿主机 ~/catkin_ws 里也能直接访问
`mkdir -p ~/catkin_ws/src`
`mkdir -p ~/.gazebo`   # 用于缓存 Gazebo 模型，避免每次重下

#### 3.启动容器:
```

  docker run -it \
    --name ros_noetic \
    --init \
    --net=host --ipc=host \
    --cap-add=SYS_PTRACE \
    --device=/dev/dri:/dev/dri \
    --device-cgroup-rule='c 188:* rmw' \
    --device-cgroup-rule='c 166:* rmw' \
    -e DISPLAY=$DISPLAY \
    -e QT_X11_NO_MITSHM=1 \
    -e QT_QPA_PLATFORM=xcb \
    -e XAUTHORITY=/tmp/docker.xauth \
    -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
    -v /tmp/docker.xauth:/tmp/docker.xauth:ro \
    -v ~/catkin_wst:/root/catkin_wst \
    -v ~/catkin_ws:/root/catkin_ws \
    -v ~/.gazebo:/root/.gazebo \
    -v ~/rosbags:/root/rosbags \
    -v /dev/input:/dev/input:ro \
    noetic-intel-gpu-1 \
    bash


  ```

#### 4.容器内安装开发与仿真工具:
```
apt update
apt install -y python3-catkin-tools build-essential gdb
```

#### 5.配置在Clion开发环境:

##### 5.1在容器中安装SSH:
`apt update && apt install -y openssh-server`

##### 5.2为root用户设置一个密码用于连接:
`passwd root`

##### 5.3在容器内启动SSH服务:
`service ssh start`

##### 5.4去JetBrains官网下载Gateway客户端
>解压后进入bin目录运行Gateway
`./gateway.sh`

##### 5.5在Gateway中新建SSH连接:
打开 Gateway，选择 SSH，点击 New Connection。
填入连接信息：Username 填 root，Host 填 localhost（因为 --net=host），Port 填 22，Password 填你刚设置的 root 密码。
点击 Check Connection and Continue。
 ###### 若连接失败: 
 ###### 修改 SSH 服务端配置
 ``` 
 echo "AllowTcpForwarding yes" >> /etc/ssh/sshd_config
 echo "PermitRootLogin yes" >> /etc/ssh/sshd_config
 echo "PasswordAuthentication yes" >> /etc/ssh/sshd_config
 ```
 ###### 重启 SSH 服务让配置生效：
 `service ssh restart`

##### 5.6选择IDE版本并连接:
连接成功后，在 IDE Version 下拉框中选择 CLion。
在项目路径中填入容器内的路径，例如 /root/catkin_ws。
点击 Start IDE and Connect。Gateway 会自动在容器内下载并启动 CLion 后端，然后打开项目窗口。

##### 5.7设置clion远程工具链
在 设置/构建、执行、部署/工具链 选项栏右边, 将CMake选择为ros系统的CMake, 否则clion默认版本CMake会过高

#### 6. 将现有容器保存为镜像
`docker commit 容器名 镜像名`
`docker commit ros_noetic ros_noetic_clion`

#### 6.1 将镜像导出
`docker save -o 导出的.tar文件路径 镜像`
`docker save -o ~/Docker/noetic-intel-gpu.tar noetic-intel-gpu`

## 后续使用

#### 启动容器:
`docker start -ai 容器名`
`docker start -ai ros_noetic`

#### 进入在后台运行的容器, 常用与终端已被关闭:
`docker exec -it 容器名 bash`
`docker exec -it ros_noetic bash`

#### 重启SSH服务:
`service ssh start`

#### 退出容器:
`exit`

#### 在宿主机停止容器, 常用于终端已被关闭:
`docker stop 容器名`
`docker stop ros_noetic`

#### 强制立即停止容器
`docker kull 容器名`

#### 删除容器
`docker rm 容器名或id`

#### 强制删除运行中的容器
`docker rm -f 容器名或id`

#### 批量清理所有停止的容器
`docker container prune`

#### 删除镜像
`docker rmi 镜像名或id`


### 内容传输：
###### 在容器内无法与主机共享剪贴板
##### 文件移动：
    docker cp 容器名或id:/容器文件路径 /主机文件路径
    docker cp ros_noetic:/root/.tmux.conf /home/seed/.tmux.conf

##### 带有软链接(比如catkin构造的CMakeLists.txt)的复制:
    docker cp -L 容器名或id:/容器文件路径 /主机文件路径
    docker cp -L ros_noetic:/catkin_wst/dm1/CMakeLists.txt ~/CM1.txt



### 下载：
#### 若下载的是源码包(github)：
建议安装在主机的catkin_ws/src中，以便修改的代码仍然存在

#### 若下载的是二进制包(apt install):
    sudo apt install ros-noetic-xxx
需要下载进容器的ros目录内，且不需要关心下载到哪个文件夹
不过这些部分会随着容器的删除而消失，需要保留镜像和记录安装命令

#### ros软件包下载：
###### 可在https://index.ros.org中上面的packege list，选择noetic版本，再去查找软件包，点进去后可在website里面查看介绍
    sudo apt install ros-noetic-软件包名    


#### 已在容器中下载的软件包：
sudo apt install ros-noetic-rqt-robot-steering(noetic_intel_gpu 已包含)
apt update && apt install -y tmux(noetic_intel_gpu 已包含)
apt-get install -y xclip
apt-get install -y \
    ros-noetic-ros-control ros-noetic-ros-controllers \
    ros-noetic-tf2-tools ros-noetic-teleop-twist-keyboard ros-noetic-teleop-twist-joy \
    ros-noetic-plotjuggler-ros ros-noetic-joint-state-publisher-gui






