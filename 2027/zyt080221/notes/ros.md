#Ros
- ##编译与工作空间管理
- `catkin_create_pkg <包名> <依赖1> <依赖2>` 创建新功能包
- `catkin_make`经典编译命令
- `catkin build`现代编译命令
- `source devel/ setup.bash`刷新环境变量
- `roscd <包名>`快速跳转到包目录
- `rospack find <包名>` 查找包的绝对路径
##核心节点与Launch运行
- `roscore`启动ros大管家
- `rosrun <包名> <节点名>` 运行单个节点
- `roslaunch <包名> <文件名.launch>`一键启动多个节点
- `rosnode list` 列出当前活着的节点
- `rosnode info <节点名>` 查看节点详细信息
- `rosnode kill <节点名>` 杀掉指定节点
##话题
- `rostopic list`列出所有活跃话题
- `rostopic echo <话题名>`打印话题数据
- `rostopic info <话题名>` 看话题是谁发的/收的
- `rostopic hz <话题名>`测试数据频率
- `rostopic pub <话题名> <类型> <数据>` 手动发指令
##服务
- `rosservice list` 列出所有服务
- `rosservice call <服务名>` 查看服务类型 
- `rosparam list` 列出参数
- `rosparam set <键> <值>`调用服务
- `rosparam get <键> <值>`读取参数
- `rosparam load <yaml文件>` 加载YAML文件
- `rosparam dump <yaml文件>` 导出参数到YAML
##可视化
- `rpt_graph`节点关系可视化
- `rviz`3D数据可视化
- `rosbag record -a`全量录包
- `rosbag info <包名.bag>`查看包信息
- `rosbag play <包名.bag>`回放数据包
##备忘
- `sudo docker start ros_course`唤醒容器
- `sudo docker exec _it ros_course bash`进入容器开新终端
- `xhost +local:docker`放行图形权限
- `sudo docker ps -a` 查看所有容器

