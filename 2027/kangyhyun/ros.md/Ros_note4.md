# 在重建hero_chassis_controller时学习到的东西

#一、catkin build时

**我在以`rm_template`为模板建立`hero_chassis_controller`时只修改了`package.xml`和`CmakeList.txt`中的name，但忽略了其中的node节点未删除，以至于Cmake配置时报错。**

##catkin build阶段理解

1. 扫描 ROS包
2. 运行 CMake配置 
3. 编译C++ 源文件 
4. 生成 .so 库并安装插件 XML

此时我的错误`CMake Error at /root/chassis_ws/src/hero_chassis_controller/CMakeLists.txt:74 (add_executable):`是第二阶段出错，即CMake 执行 CMakeLists.txt 第 74 行的 add_executable() 时出错。
add_executable明确要求编译`src/hero_chassis_controller_node.cpp`，但是这个文件不存在，所以camke无法找到源文件，以至于Cmake无法创建名为 hero_chassis_controller 的可执行程序。而CMake 配置没有完成，因此无法生成 Makefile 或 Ninja 构建文件。catkin 随后直接停止，不会进入真正的 C++ 编译阶段。

##那么为什么会产生这样的原因呢？
我是从 `rm_template` 复制出` hero_chassis_controller `的。模板中通常同时包含两种 target：
控制器插件库：`add_library(...)`
可选的测试节点：`add_executable(... src/xxx_node.cpp)`

我只保留了控制器库的源文件即：`src/hero_chassis_controller.cpp`没有保留node节点的cpp文件，但CmakeList.txt中仍然保留了将节点文件转化为可执行程序的指令
```
add_executable(hero_chassis_controller
  src/hero_chassis_controller_node.cpp)
```
Cmake不会因为文件不存在就跳过，而是直接报错。

##那么为什么不需要保留node节点呢？

那我们需要先知道**控制器插件和普通 ROS 节点的区别**
hero_chassis_controller 不是一个普通 ROS 节点。它不需要自己的 main() 函数，而是作为插件被 controller_manager 加载。

**普通的ros节点**通常是：
1. 可执行程序 
2. main（） 
3. ros：：init（） 
4. ros：：spin
**控制器插件**通常是：
1. 加载`libhero_chassis_controller.so`  
2. 创建`class HeroChassisController`
3. `pluginlib 加载`
4. `controller_manager 调用 init() 和周期性调用 update()`

真正运行 main() 和 ros::spin() 的是 controller_manager，不是控制器。
所以控制器插件只需要生成共享库：`devel/lib/libhero_chassis_controller.so`
不需要：`devel/lib/hero_chassis_controller/hero_chassis_controller`

这也是为什么` simple_chassis_controller `的 `CMakeLists.txt` 中只有` add_library()`去生成库，没有为控制器添加` add_executable()`去把一个cpp文件编译成小程序。

##插件注册链路
###控制器被ROS识别需要完整的链路
package.xml
  ↓
导出 hero_chassis_controller_plugins.xml
  ↓
插件 XML 指向 libhero_chassis_controller.so
  ↓
.so 中包含 PLUGINLIB_EXPORT_CLASS(...)
  ↓
pluginlib 找到 HeroChassisController
  ↓
controller_manager 创建并调用控制器

**简单理解controller_manager与hero_chassis_controller的关系**
将控制器想成一个电机驱动模块,而controller_manager是主机,负责运行程序.hero是插上去的模块,负责提供init()和update().

#对于rm_template的解读
tree框架
.
├── CMakeLists.txt
├── LICENSE
├── README.md
├── config
│   └── default.yaml
├── doc
│   └── example.jpg
├── include
│   └── ros_package_template
│       ├── Algorithm.hpp
│       └── RosPackageTemplate.hpp
├── launch
│   ├── ros_package_template.launch
│   └── ros_package_template_overlying_params.launch
├── package.xml
├── src
│   ├── Algorithm.cpp
│   ├── RosPackageTemplate.cpp
│   └── ros_package_template_node.cpp
└── test
    ├── AlgorithmTest.cpp
    └── test_ros_package_template.cpp
    

##Algorithm.cpp(hero...里面不需要出现)
1. **本质**:Algorithm.cpp 不是底盘控制器的一部分，也不是 ros_control 必须的文件。它是 rm_template 自带的一个示例算法。
###其实现的功能
- 先收集若干个数据
- 保存起来
- 计算数据平均值
###出现原因
- rm_template 实际上是一个普通的 ROS 包模板，Algorithm.cpp是纯算法，不涉及 ROS
- RosPackageTemplate.cpp是一个ROS 节点，负责订阅话题、调用算法.
- 让我们学会将纯数学算法和 ROS 通信分离

##比较rm_template和simple_chassis_controoller的区别
rm_template 是通用 ROS 包模板
包含：
- Algorithm.cpp          示例算法
- RosPackageTemplate.cpp 普通 ROS 节点
- ros_package_template_node.cpp 节点入口
- 温度话题 + 求平均值 service

simple_chassis_controller 去掉了这些普通节点示例，换成了：
- SimpleChassisController.h/cpp
- controller_interface::Controller<EffortJointInterface>
- getHandle(...) 获取四个轮子的 JointHandle
- update() 中直接 setCommand()
- plugin XML
- controllers.yaml
- 仿真 launch
 
相较于rm_template,simple更像是在rm_template的目录和构建风格的基础删减了普通节点和示例算法后，又加入了ros_control后形成的插件。

rm_template为hero提供了包的骨架，README，CMake，package的基本格式。
simple提供了控制器插件的结构，jointHandle用法，pluginlib方式。
