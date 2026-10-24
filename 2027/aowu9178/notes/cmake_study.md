# cmake、catkin
一些有用的网站链接：
1. [知乎：CMake 相关](https://zhuanlan.zhihu.com/p/476697014)
2. [菜鸟教程：CMake 基础](https://www.runoob.com/cmake/cmake-basic.html)
3. [The Emperor Has No Clothes](https://www.mihaileric.com/The-Emperor-Has-No-Clothes/)
4. [ROS catkin Tutorials](https://wiki.ros.org/catkin/Tutorials)
## 关于cmake,makefile和catkin关系：
    Makefile：告诉 make 每一步怎么编译、链接。
    make：照着 Makefile 干活。
    CMake：人写 CMakeLists.txt，它生成 Makefile。
    Catkin：组织 CMake 和 make 来管理整个 ROS 工作空间。

    **总的来说**Catkin 管 ROS 工作空间 → 调用 CMake 生成 Makefile → 调用 make 按 Makefile 编译。

 ## 关于自己对用大模型帮助学习这一课题的思考
 首先值得肯定的是大模型确实是一个非常有用的工具。可以将它视为类似浏览器的检索工具，快捷查找相关资料，
 也可以将它视为老师，询问它不懂的地方，也可以向它发送你的理解，它可以根据你的理解纠正你可能的一些错误。
 以上是我总结出来的一些用大模型辅助自己学习的方法，在本次学习中我有用到这些方法学习。
 **要注意的是**利用大模型直接完成作业是不太可取的，因为在这个过程中我们并不理解其中的运作过程，你可以让
 大模型教你一步步做，但一定要自己弄清楚每一步的意义是什么。
