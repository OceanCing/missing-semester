# 第三课笔记

## 第一个终端

1. `cd` 切换并进入一个目录
2. `./run.sh` 开始运行
3. `roscore` 开始进行 master

## 第二个终端

1. `cd` 切目录
2. `./run.sh` 也开始运行
3. `rosrun turtlesim turtlesim_node` （比如说小乌龟的 node）

## 第三个终端

1. `cd`
2. `./run.sh` 三个终端都是一样的容器
3. `rosrun turtlesim turtle_teleop_key` 键盘信号转换为速度指令

## 总结

后两个终端一个增加了小海龟的图面 一个给键盘指令赋予速度的信号 都相当于给这个环境加上模组

前一个终端 则是提供了这个容器 充当一个支撑环境的作用
