

```
docker start wpr_ros1
docker exec wpr_ros1 service ssh start
xhost +
```

宿主机
xhost +
容器
export DISPLAY=:0

启动ROS(在容器中)
```
source /opt/ros/noetic/setup.bash
source ~/catkin_ws/devel/setup.bash
```

## 启动关闭
```
docker start 
docker stop ssh_wpr_sim
xhost +
```

## 删除
```
docker rm name
```




## 创建完全体
```
# 1. 先允许本机 Docker 连接 X Server
xhost +local:docker

# 2. 启动容器
docker run -it -d \
  --name wpr_ros1 \
  -p 2223:22 \
  --device nvidia.com/gpu=all \
  -e NVIDIA_DRIVER_CAPABILITIES=all \
  -e DISPLAY=$DISPLAY \
  -e QT_X11_NO_MITSHM=1 \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  -v /home/zlz/catkin_ws:/root/catkin_ws \
  wpr_sim_image:gpu
```

