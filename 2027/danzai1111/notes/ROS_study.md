##ROS
- 一套构建机器人软件的框架，可以实现节点管理，通信
- 可理解为不同程序各自分工最后通过ros交换数据
#节点
- 负责一个部分的程序
1.Publisher 负责发送消息的节点
2.Subscriber 负责接受消息的节点
#Topic(其中一种通信方式)
- 实现不同程序交换数据
#Message
- 节点间传递的数据
#ROS Master
- 负责帮助节点找到彼此

#部分命令
- roscore 启动ros1服务
- rosnode list 列出当前可用节点
- 
