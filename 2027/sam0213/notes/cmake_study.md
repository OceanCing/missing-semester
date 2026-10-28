#cmake和catkin笔记
##编译原理
###预处理
1.define删除
2.处理#ifdef等预编译指令
3.#include头文件接入
4.行号接入
5.只保留#pragma
###编译
1.读取源程序
2.语法分析
3.语义分析
4.源代码优化：（不同编译方式优化程度不同）
5.中间代码将其分为前端和后端（源语言和目标语言（汇编）之间的代码）
6.目标代码（汇编生成）

###汇编
1.将其翻译成二进制机器语言
###链接
1.每个文件（头文件和各个。cpp文件）均有与个.o的文件，并将其合并
####静态链接

#####概念
1.虚拟内存：每个进程都有其虚拟地址，但不是物理内存真正的地址，其需要通过映射到物理内存，使得内存管理更为方便
#####.o文件包含：
1.ELF字节大小和文件类型等基本信息
2. .text机器码
3.rodata:只读数据
4.data:变量等信息（已初始化）
5..bss全局与静态变量（未初始化）
6. .symtab:函数，变量类型，大小等信息
7.rel.text:text中地址列表
8.rel.data:data中地址列表
#####过程
1.扫描文件，收集符号，并分配虚拟地址
2.将符号表和其定义链接，如果找不到，那就给连接器找别的文件有没有
3 重定位：合并代码，并根据rel.text地址表修改其内存指向（原来是虚拟内存）
####动态链接
1.形成共享库。so文件
2.编译源文件，并记录其对共享库的依赖
3.运行：读取库，写入虚拟空间，调用链接表查找库中符号地址，并回填至其依赖所记录的偏移量，获得其实际地址


##cmake
###public,private,interface的区别
####public
1.所有目标都可用，包括依赖的目标
####interface
1.除了依赖的目标不可用外，其余后续依赖依赖的目标后面都可以用
####private
1.依赖的目标可以使用，但后续依赖依赖的目标等等都不可以使用
####开头及其他基础语法
1.cmake_minimum_requires()//最小版本
2.add_executable()//创建可执行目标
3.project(项目 版本)
4.set()//varieties
5.message()//output
####链接库
1.add_library()
2.target_link_libraries()
3.target_include_directories()
4.list(APPEND)//list
5.option()//bool varieties

####安装
1.install(TARGET DESTINATION)


###拓展
####自定义模块：减少代码重复（相当于头文件？）
1.构建cmake目录
2.构建文件...cmake：function(name num )endfunction()
3.原来的cmakelists:include()
4.list(append 路径)//添加模块搜索路径
5.include(文件名)
6.添加目标后就可以使用模块中的函数了
####自定义命令
1.add_custom_command{
 TARGET(目标)
 COMMAND要执行的命令
 COMMENT输出
 }
2.add_custom_command{OUTPUT COMMAND DEPENDS}//只有output文件不存在或depend文件更新，才执行命令（COMMAND）

####自定义目标（不会在build中执行，而是需要在build时指定目标,同时是自定义命令入口）
1.add_custom_target(COMMAND DEPEND)
####自定义脚本（配置操作）
1.创建config.cmake
2.构建类型if(NOT CMAKE TYPE){set(CMAKE_BUILD_TYPE "Release")}
3.cmakelists中调用include(路径)
####查找
1.find_package()
2.target_link_libraries()

####交叉编译（跨平台编译）
-构建工具链文件（toolchain.cmake）
1.CMAKE_SYSTEM_NAME
2.CMAKE_SYSTEM_PROCESSOR
3.CMAKE_C_/CXX_COMPILE
4.通过set来设置这些
#####使用工具链文件交叉编译
1.mkdir build_arm
2.cmake DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake//指定工具链文件
3.make
####编译设置
1.set_target_properties(目标 PROPERTIES 。。。)
#####
1.COMPILE_OPTION编译选项
2.COMPILE_DEFINITIONS编译宏（=#define ...）
3.LINK_FLAGS设置链接器标志(?待理解)




##catkin工作空间
###编译及运行
1.catkin_make clean &&catkin_make
2.source opt/ros/noetic/setup.bash
3.rosrun .. _pkg .._node
###创建空间
1.mkdir .....catkin_ws
2.catkin_make
3.cd /src &&catkin_create_pkg ...._pkg  roscpp
4.cd ..
5.catkin_make
###与vscode联动
1.打开（下载dev container）
2.创建空间 输入路径

##大模型
_分为3个步骤：
1.读取文件
2.列出内容
3.输出内容
'毫无疑问，大模型能帮助自己学习，修改代码，辅助自己开发，但是他无法完全替代人类编程，在初学阶段，过度依赖大模型无法使自己拥有独立编程的能力'



##ros

###概念
1.node:目标，
2.topic:传输路径
3.message:传输内容
###案例
1.乌龟
'rosrun turtlesim turtle_node'
'rosrun turtlesim turtle_teleop_key'//键盘控制
-用乌龟画图：
1.发出信息
'#include<ros/ros.h>'
'#include<geometry-msgs/Twist.h>'
-int main(int argc,char **atgv){
 - ros::init(atgc,argv" .._node");//创建节点
  -ros::nodehandle nh;//创建对象
  -ros::publisher pub=nh.advertise<geometry-msgs::Twist.h>("/Turtle1/amd_vel",10)
-//10为队列长度，amd_vel为 topic名字（频道？），创建了一个pub的广播
-//乌龟画图
 - ros::Rate looprate(10)  //设置频率
     - while(ros::ok())
       - {
       - geometry_msgs::Twist.msg//msg为对象
        - msg.linear.x=10;//设置属性
        -pub.publish(msg);//广播信息

        -}
     -looprate sleep();
     -return 0;
-}'
-在cmakelist.txt中写入add_executable(目标 路径) target_link——libraries(目标 底层库${catkin_LIBRARIES})

2.接收信息：subscriber 代码部分：
'#incldue<ros/ros.h>'
'#include<turtlesim/Pose.h>'
'void posecallback(const turtlesim::PoseConstPtr&msg){ROS_INFO("LOCAL:X=% .2f,y=%.2f",msg->x,msg->y)}'
'ros::init....ros::nodehandle...ros subscriber sub=.....三件套'
'ros::spin（）'//卡在这里，有消息就接受
3.乌龟频道：
1./rosout
2./turtle1/amd_vel
3./turtle/pose
4./turtle/color_sensor

