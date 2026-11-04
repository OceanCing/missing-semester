# ROS

## 核心概念
### node-节点
 一个个可以独立运行的程序 执行具体任务的基本单位

    一个节点通常是一个独立运行的进程，可以单独启动和停止，调试
![alt text](image.png)
### Topic-话题
##### 一个话题的发布者和订阅者都是节点
![alt text](image-1.png)

### Message-信息
话题是通信通道，信息是通道里传输的数据格式，==不是数据本身==
##### ROS1支持的消息类型
 - 字符串 std_msgs/string
 - 三维点坐标 geometry_msgs/Point
 - 图像数据 sensor_msgs/Twist
 - 线速度&角速度等运动信息

### Service-服务
    请求-应答（Request / Response）一问一答，同步调用
![alt text](image-3.png)

### Action-动作