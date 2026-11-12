# 冷启动手册：从开机到「四路 PID 跑起来」

适用场景：**每次关机/重启后**，把环境恢复到「四个轮速收敛到 3.0 rad/s」的状态。

坐标约定：

- 🖥️ = 宿主机终端（提示符 `zhuyiqueshizheng@zhuyiqueshizheng-JIAOLONG-Series`）
- 🐳 = 容器终端（提示符 `root@c9c188ec297d`）

> 同一个工作区有两个路径：宿主机 `~/catkin_ws_backup`，容器 `/root/catkin_ws_backup`（bind mount，改哪边都一样）。

---

## 1. 🖥️ 重建 X11 授权（**每次开机必做**）

`/tmp` 开机被清空，而容器靠这个 cookie 才能把 Gazebo / rqt 的窗口画到你的桌面上。

```bash
export XAUTH=/tmp/.docker.xauth
touch $XAUTH
xauth nlist $DISPLAY | sed -e 's/^..../ffff/' | xauth -f $XAUTH nmerge -
chmod 777 $XAUTH
xhost +local:docker
```

**判据**：`ls -l /tmp/.docker.xauth` 存在，权限 `-rwxrwxrwx`。

⚠️ **顺序很重要**：必须**先建好这个文件，再 `docker start`**。因为容器启动时要把这个文件 bind mount 进去，文件不存在的话 Docker 会建一个**同名目录**，X 连接就会失败。

---

## 2. 🖥️ 启动容器

```bash
docker start ros1_env
```

**判据**：`docker ps | grep ros1_env` 显示 `Up`。

> 容器 `RestartPolicy = no`，所以不会自启，每次都要手动 `docker start`。但**不要 `docker rm`**，删了就要重新 `docker run`（见文末附录）。

---

## 3. 🖥️ 开 4 个终端窗格

打开 terminator → 右键 → `Split Horizontally` / `Split Vertically`，分成 4 格。

**每一格都要先进入容器**：

```bash
docker exec -it ros1_env zsh
```

> 进容器后**不需要再 `source`**：容器的 `~/.zshrc` 里已经有两行
> `source /opt/ros/noetic/setup.zsh` 和 `source ~/catkin_ws_backup/devel/setup.zsh`。
> 但如果你用的是 `bash` 而不是 `zsh`，就得自己 source（bash 读的是 `.bashrc`，没有这两行）。

---

## 4. 🐳 P1 —— 起仿真（Gazebo）

```zsh
roslaunch rm_description_for_task hero.launch
```

**判据**（两行都要出现）：

```
Spawn status: SpawnModel: Successfully spawned entity
[INFO] ...: Loaded gazebo_ros_control.
```

同时看 Gazebo 窗口底部：`Real Time Factor: 1.00`、`Sim Time` 在往上走。
**如果是 `0.000` 说明仿真暂停了**，点左下角 ▶，或

```zsh
rosservice call /gazebo/unpause_physics
```

---

## 5. 🐳 P2 —— 起控制器

```zsh
roslaunch hero_chassis_controller load_controller.launch
```

**判据**：

```
Controller Spawner: Loaded controllers: controller/hero_chassis_controller, controller/joint_state_controller
Started controllers: controller/hero_chassis_controller, controller/joint_state_controller
```

**这个窗口之后不要关、不要 Ctrl-C。** 它常驻（`rospy.spin()`），关掉就等于卸载控制器。

---

## 6. 🐳 P3 —— 验证

```zsh
rosservice call /controller_manager/list_controllers
```

应该有**两个 `running`**。

```zsh
rostopic echo -n1 /joint_states/velocity
```

四个值应该在 3.0 附近（±0.5 以内，波动来自库仑摩擦极限环）。

```zsh
rostopic echo -n1 /gazebo/model_states
```

`hero` 的 `position.z` 约 `0.144` 表示轮子正常着地。

---

## 7. 🐳 P4 —— 观察与调参

```zsh
rqt_plot '/joint_states/velocity[0]' '/joint_states/velocity[1]' '/joint_states/velocity[2]' '/joint_states/velocity[3]'
```

```zsh
rosrun rqt_reconfigure rqt_reconfigure
```

⚠️ **`[0]` 必须加引号**：zsh 会把 `[0]` 当文件名通配符，报 `no matches found`。
（一劳永逸：`echo 'unsetopt nomatch' >> ~/.zshrc && exec zsh`）

对应关系：

```
velocity[0] = left_back    velocity[2] = right_back
velocity[1] = left_front   velocity[3] = right_front
```

---

## 8. 对照「最终画面」检查表

| 画面元素 | 应该是什么 |
|---|---|
| Gazebo 3D 窗口 | 英雄底盘在原点，四轮在转，车往前开（会跑出边界，`reset_world` 拉回） |
| P1 | Gazebo 的 `[INFO]` 日志，停着不动 |
| P2 | `Started controllers: ...`，停着不动 |
| P3 | 提示符 `~/catkin_ws_backup`，随时敲命令 |
| P4 | `rostopic echo` 在刷四个数 / `rqt_plot` 四条曲线 |
| rqt_reconfigure | 4 组 `left_front / right_front / left_back / right_back` |
| Gazebo 自定义面板（`NAO... / Is... / Sel... / En... / Pose`） | 乱码是 rm_gazebo 的面板，忽略 |

车跑出画面了：

```zsh
rosservice call /gazebo/reset_world
```

---

## 9. 收工顺序（别乱关）

1. **P2** `Ctrl-C` → 等提示符回来（它会 stop + unload 控制器）
2. **P1** `Ctrl-C` **两次**（第一次关 gzclient，第二次关 gzserver）
3. 关掉 rqt 窗口

这样下次启动是干净的；反之如果直接关掉 P1，P2 的 spawner 会残留，下次就报 `already loaded`。

---

## 10. 冷启动常见故障对照

| 现象 | 原因 | 处理 |
|---|---|---|
| Gazebo 窗口不弹 / `cannot open display` | X11 cookie 没建，或建文件晚于 `docker start` | 重做第 1、2 步，注意顺序 |
| `command not found: rospack` / `roslaunch` | 进了 bash 或者 `.zshrc` 被改 | `source /opt/ros/noetic/setup.zsh` |
| `RLException: neither a launch file in package` | 没 source 工作区 devel | `source ~/catkin_ws_backup/devel/setup.zsh` |
| `command not found: lsd` | `.zshrc` 里 `alias ls='lsd'` 但容器没装 lsd | `unalias ls ll` 或改 `.zshrc` |
| spawner 报 `already loaded` | 上次没卸载干净 | `rosservice call /controller_manager/unload_controller "name: '<名字>'"` |
| 服务调用卡住不返回 | 仿真暂停 / 客户端残留 | 点 ▶；`pkill -9 -f rosservice` |
| 轮子不动、车不动 | 仿真暂停；或 PID 参数没加载 | 看 P1 有没有 `PID 参数缺失` |

---

## 11. 改完 C++ 之后的重载（这不是冷启动，别搞混）

```
🖼️ CLion 改代码
   ↓
🐳 P3  catkin build hero_chassis_controller
   ↓
🐳 P2  Ctrl-C，等提示符回来
   ↓
🐳 P1  Ctrl-C 两次 → roslaunch rm_description_for_task hero.launch
   ↓
🐳 P2  roslaunch hero_chassis_controller load_controller.launch
```

**只改 yaml 参数的话**，不需要动 P1（不用重启 gzserver），只做 P2 的 Ctrl-C + 重开。

**为什么改 C++ 必须重启 gzserver**：`controller_manager` 跑在 gzserver 进程里，`.so` 是在加载控制器时 `dlopen` 的；`catkin build` 生成的是新文件（新 inode），已运行的进程仍映射旧 inode，不重启就一直是旧代码。

验证用的自检：

```zsh
P=$(pgrep -x gzserver)
echo "内存: $(awk '/hero_chassis_controller.so/{print $5}' /proc/$P/maps | sort -u)"
echo "磁盘: $(stat -L -c %i ~/catkin_ws_backup/devel/.private/hero_chassis_controller/lib/libhero_chassis_controller.so)"
```

两个数字相同 = 跑的是最新代码。

---

## 附录：如果容器被删了，重建它

```bash
XAUTH=/tmp/.docker.xauth
touch $XAUTH
xauth nlist $DISPLAY | sed -e 's/^..../ffff/' | xauth -f $XAUTH nmerge -
chmod 777 $XAUTH
xhost +local:docker

docker run -it --name ros1_env \
  --gpus all \
  --env="DISPLAY=$DISPLAY" \
  --env="QT_X11_NO_MITSHM=1" \
  --env="XAUTHORITY=$XAUTH" \
  --env="NVIDIA_DRIVER_CAPABILITIES=all" \
  --env="NVIDIA_VISIBLE_DEVICES=all" \
  --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
  --volume="$XAUTH:$XAUTH" \
  --volume="/home/zhuyiqueshizheng/catkin_ws_backup:/root/catkin_ws_backup" \
  --cap-add=SYS_PTRACE \
  --user root \
  my_ros_gpu_base:v2 /bin/bash
```

> `--cap-add=SYS_PTRACE` 是 CLion attach 调试 `gzserver` 需要的，别漏。
> `--volume` 里的 `~/.zshrc`、`.p10k.zsh`、`.oh-my-zsh` 是当初用 `docker cp` 拷进去的，重建容器后要重新拷一次。
