# 第二次作业学习笔记

> 下面是略微抽象的一节，关于这类原理解析，网上有许多相关的解析资料，这里不罗列太多知识点，只写一下框架和思考和当前的学习程度

## 1-程序详细编译过程和相关计算机系统原理

- 参考文献链接:[程序详细编译过程](https://zhuanlan.zhihu.com/p/476697014)

### 1.文章结构

大概是可以分成 **程序编译的整体流程**->**链接的概念和意义**->**虚拟内容存和虚拟空间**->**静态链接的详细过程**->**可执行文件加载**->**动态链接和共享库**

### 2.程序编译的整体流程

这里放一个流程图

![image-20261024201718405](images/image-20261024201718405.png)

**整个编译过程是由编译驱动程序来执行，从用高级语言写好的文本文件到可执行文件的过程**

**形成可执行文件前还有一个重要一环，就是将汇编出来的二进制魔表文件链接，而在链接这里可以体现计算机对物理硬件资源管理的抽象**

> 以前不了解，只知道汇编好像是一门语言，现在这里也是编译的一个阶段，由汇编器执行

### 3.链接的概念和意义

链接是将许多代码和数据段给拼接，实现一种**熵减**的过程，由链接器执行，在程序诞生的不同时期都可以发生，最后得到一个**单一文件**

其实就是当程序太大，文件太多时，为了**减少资源浪费和易于修改和管理**而诞生的技术

### 4.虚拟内存和虚拟地址空间

![image-20261024204609671](images/image-20261024204609671.png)

> 这里是关于链接的一个计算机体系的底层背景知识，解析连接器在执行时，需要为代码和数据确定虚拟地址，因此插入这里的知识

**虚拟内存**相当于一个机制，在实际物理内存和虚拟出来的进程存储间实现某种操作映射机制，套了一个层，每一个进程拥有独立的空间，因此不用处理和协调其他进程在实际物理内存中的位置（对他们自己来说），虚拟内存帮他们处理，每个进程自己看起来像是拿到一个连续的地址空间，实际上是虚拟出来的

> 哪有什么岁月静好，只不过是有虚拟内存在负重前行

### 5.静态链接详细过程

![image-20261024210054275](images/image-20261024210054275.png)

先是解析了目标文件的结构，也就是在Linux中的ELF格式文件，这些文件带一些命令行参数输入进**静态链接器**然后产生完全链接的，可以加载和运行的**可执行目标文件**

*那么链接器拿到这些文件后怎么链接呢*

#### 1.空间和地址分配

将文件的所有符号定义和引用经过一些操作的到合并后的**可执行文件和进程虚拟地址空间**的映射关系，这里就用到了虚拟内存的机制，实现还没有执行文件就以及得到了地址空间

#### 2.符号解析和重定位

每个可重定位目标模块都有符号表，根据一些解析机制和处理重定义的符号机制以及接下来的重定位，实现为每个符号分配运行时的地址，相当于就是一开始很多ELF格式的文件，很乱，现在全部解析将ELF中的区合并重组成一个有序的文件，**程序中的每条指令和全局变量都有唯一的运行时内存地址了。**

链接完成后得到一个可执行的目标文件

### 6.可执行文件的加载

拿到可执行文件后，从链接阶段进入**运行阶段**

步骤如下

- shell调用加载器
- 加载器吧可执行文件映射到进程内存
- 跳转都啊程序入口点开始执行

### 7.动态链接和共享库

![image-20261024211000857](images/image-20261024211000857.png)

上面解析的是静态库，但是静态库的缺点是浪费内存空间，当两个程序共享一个模块，会因为复制两份而浪费空间（*有点类似C++里的引用传递或者指针传地址,而不使用直接将类或字符串直接复制进函数*），因此诞生了**共享库**

它是在运行是可以和已经在内存中的程序链接起来，由**动态链接器**来执行

它和静态库区别是，它没有像静态库那样是**复制和嵌入**其他模块，而是先获取了库文件的**重定位和符号表**的信息,然后执行重定位，最后实现得到固定的共享库位置，实现动态链接

## 2-CMake流程

> 我之前学过部分的CMake,但是由于学的教程太老，导致有一些语法以及淘汰，后续也学了一下现代的语法，正好可以拿出来讲，这部分还是讲讲对CMake的理解，不罗列太多知识

先放文章的链接：[CMake基础](https://www.runoob.com/cmake/cmake-basic.html)

### 我自己的理解

这个CMake相当于一个上层的构建工程的写指令的工具，有点类似终端和电脑本机的关系，中间很繁杂的文件包含，找路径，编译，链接等过程的执行工具被封装好，可以说是工具链的启动总操作台。

**那么CMake主要做什么呢**

大概是，找到需要用的哪些软件包，功能包，依据一些我们制定的规范（版本，工具，路径），执行工具链，复现上面的编译过程，最后得到构建好的工程（生成库文件，可执行程序等）以及实现产出（安装）

**刚刚说的总操作台，那就是CMakeLists.txt了**

这边规定好工程名，工具版本，定义些变量，规定生成物的路径，安装路径，添加各种可执行文件或者库（使用哪些源文件），因此在这边直接是定义了整个工程的结构和内容

**说说工作区结构**

类似

Demo/
├── CMakeLists.txt          顶层：project、add_subdirectory
├── include/
│   └── demo.h              对外头文件
├── src/
│   ├── CMakeLists.txt      本目录怎么编
│   ├── main.cpp
│   └── demo.cpp
├── tests/
│   ├── CMakeLists.txt
│   └── test_demo.cpp
└── build/                  cmake 生成的目录

一般是在build里使用`cmake ..`来加载出中间的构建文件，重点是**makefile**，然后直接原地`make`执行makefile里的自动化构建步骤，工程就构建好了

所以相当于cmake是在makefile上的一层，最后还是要执行makefile

## 3-AI怎么被调用完成任务的

> Agent 的核心并不只是“强大的模型”或“复杂的提示词”，而是一个受控的闭环：模型提出动作，程序执行动作，执行结果再影响模型的下一步决策。

### 一般的结构

harness-agent-LLM

### agent是什么

很显然，agent中文意思是代理，那么很显然这里的代理是**`代替LLM处理`**的意思，嵌入它的架构是怎样呢，一般是我提出一个任务，然后发到LLM，它来判断下一步做什么，然后输出结构化调用工具，到这里LLM就停了，它本身只能输出内容，不能和物理世界交互，然后这里就到一些程序在本地执行工具链，得到的结果后返回给LLM，然后继续推理，如此循环，维持这个循环的东西可以说是agent

#### 整个运转配合流程

![image-20261024220138530](images/image-20261024220138530.png)

上面是加入了harness的三者运转关系

harness相当于一个运转的环境，提供了上下文，日志，工具，权限，沙箱环境等

agent相当于做出每一次**选择**的决策机构，下一步做什么？调用哪些工具，还是给harness校验

LLM相当于产出内容和推理的大脑，它不直接改任何文件（也没这个实力）

我自己的比喻是：

{大脑（LLM）-- 脊髓(agent) -- 身体(harness) } ---->要执行的对象（改文件，生代码等）

## 4-catkin 工作空间

参考文献[ros wiki](https://wiki.ros.org/catkin/Tutorials)

这里嵌入一个分析文档

<details>
<summary>ROS工作区</summary>
<p>先给结论：ROS 1/catkin 工作区的文件系统不是一张必须死记的目录表，而是三个功能区：</p>
<pre><code>src/    放人写的源码
build/  放编译过程产生的中间文件
devel/  放编译完成后能被 ROS 运行的产物
</code></pre>
<p>你当前的工作区正好可以直接拿来观察：</p>
<pre><code>/home/lichen/DX_test/catkin_test/
├── .catkin_workspace
├── src/
│   ├── hero_chassis_controller/
│   ├── simple_chassis_controller/
│   ├── navigation/
│   ├── rm_description/
│   ├── rm_template/
│   └── rm_control/
│       ├── rm_common/
│       ├── rm_gazebo/
│       └── rm_msgs/
├── build/
├── devel/
├── .catkin_tools/
└── run_ros_sim_gui.sh
</code></pre>
<h2>当前层对象：整个 catkin 工作区</h2>
<p>工作区本身只是一片目录。让它成为“ROS 工作区”的关键，是源码、构建过程和运行环境被有目的地分开。</p>
<h3><code>src/</code> 是源码区</h3>
<p><code>src/</code> 本身没有程序逻辑。它主要负责存放 ROS 功能包和其他源码。</p>
<p>这里有一个很重要的实际规则：</p>
<blockquote>
<p>功能包不是由目录深度决定的，而是由 <code>package.xml</code> 标记的。</p>
</blockquote>
<p>例如你当前工作区中存在这些真正的包：</p>
<pre><code>src/hero_chassis_controller/package.xml
src/simple_chassis_controller/package.xml
src/navigation/package.xml
src/rm_description/package.xml
src/rm_description_for_task/package.xml
src/rm_template/package.xml
src/rm_control/rm_common/package.xml
src/rm_control/rm_gazebo/package.xml
src/rm_control/rm_msgs/package.xml
</code></pre>
<p>所以：</p>
<pre><code>src/rm_control/
</code></pre>
<p>只是一个用来分类的普通目录，不是 ROS 包；真正的包是它下面的 <code>rm_common</code>、<code>rm_gazebo</code> 和 <code>rm_msgs</code>。</p>
<p>这也说明 <code>src/package_name/</code> 不是不可打破的固定格式。catkin 会在 <code>src/</code> 下递归寻找带 <code>package.xml</code> 的目录。</p>
<p><code>src/</code> 里还可能存在：</p>
<ul>
<li><code>.git/</code>：Git 仓库信息，不是 ROS 概念；</li>
<li><code>.idea/</code>：CLion 工程配置，不是 ROS 概念；</li>
<li><code>cmake-build-debug/</code>：IDE 或 CMake 生成目录，不是 ROS 包；</li>
<li>文档、测试和模型文件：由具体包使用，不是工作区统一规定的运行机制。</li>
</ul>
<p>因此，看到 <code>src</code> 中某个目录时，应当先问：“这个目录有没有 <code>package.xml</code>？”而不是默认每一层都是 ROS 包。</p>
<h3><code>build/</code> 是编译现场</h3>
<p>执行：</p>
<pre><code>cd /home/lichen/DX_test/catkin_test
catkin_make
</code></pre>
<p>catkin 会为每个包在 <code>build/</code> 下建立构建目录，例如：</p>
<pre><code>build/hero_chassis_controller/
build/simple_chassis_controller/
build/rm_control/
</code></pre>
<p><code>build/</code> 中常见的实体是：</p>
<ul>
<li><code>CMakeCache.txt</code>：CMake 保存的编译器、依赖目录等配置；</li>
<li><code>Makefile</code>：后续增量编译使用的规则；</li>
<li><code>CMakeFiles/</code>：目标、对象文件、依赖关系等中间结果；</li>
<li><code>catkin_generated/</code>：catkin 为这个包生成的 CMake 文件。</li>
</ul>
<p>这些文件不是用来手写的，也不是程序运行时主要读取的配置。它们主要服务于“把源码编译成产物”。</p>
<p>可以把它理解成厨房操作台：锅、半成品和工具都在这里，但做好的程序不从这里启动。</p>
<h3><code>devel/</code> 是可以使用的开发环境</h3>
<p>编译完成后，真正可以运行或导出的东西主要出现在 <code>devel/</code>。</p>
<p>你当前工作区里能找到：</p>
<pre><code>devel/lib/libhero_chassis_controller.so
devel/lib/hero_chassis_controller/wasd_teleop
devel/include/hero_chassis_controller/HeroChassisConfig.h
devel/share/hero_chassis_controller/cmake/*.cmake
</code></pre>
<p>它们分别不是同一种东西：</p>
<ul>
<li><code>libhero_chassis_controller.so</code> 是共享库，里面装着控制器类，自己没有 <code>main()</code>，不会自行启动；</li>
<li><code>wasd_teleop</code> 是可执行程序，有 <code>main()</code>，启动后成为一个 ROS 节点进程；</li>
<li><code>HeroChassisConfig.h</code> 是从 <code>cfg/HeroChassis.cfg</code> 生成的 C++ 头文件；</li>
<li><code>devel/share/.../cmake</code> 是给其他包或 CMake 查找这个包时使用的导出信息。</li>
</ul>
<p><code>devel/setup.bash</code> 把当前工作区接入当前终端环境：</p>
<pre><code>source /home/lichen/DX_test/catkin_test/devel/setup.bash
</code></pre>
<p>执行后，ROS 工具才能在当前终端里找到这套开发空间中的包、程序、库和生成头文件。</p>
<p>典型启动过程就是：</p>
<pre><code>catkin_make
    ↓
build/ 中完成编译
    ↓
devel/ 中出现程序或库
    ↓
source devel/setup.bash
    ↓
rosrun / roslaunch 根据环境找到并启动程序
</code></pre>
<p>例如：</p>
<pre><code>rosrun hero_chassis_controller wasd_teleop
</code></pre>
<p>实际被启动的对象是：</p>
<pre><code>/home/lichen/DX_test/catkin_test/devel/lib/hero_chassis_controller/wasd_teleop
</code></pre>
<p>而不是 <code>src/wasd_teleop.cpp</code>。</p>
<p><code>.catkin_workspace</code> 是一个普通标记文件，告诉 catkin 工具“这里是工作区根目录”。<code>.catkin_tools/</code> 是另一种 catkin 工具留下的配置痕迹；当前工作区的 <code>build/.built_by</code> 标记得很清楚，是用 <code>catkin_make</code> 构建的。</p>
<h2>下一层对象：<code>hero_chassis_controller</code> 包内部</h2>
<p>现在切换解析对象：<strong>单个 ROS 功能包及其文件怎样变成运行产物</strong>。</p>
<p>这个包的源码目录如下：</p>
<pre><code>hero_chassis_controller/
├── package.xml
├── CMakeLists.txt
├── src/
│   ├── hero_chassis_controller.cpp
│   └── wasd_teleop.cpp
├── include/
├── cfg/
│   └── HeroChassis.cfg
├── config/
│   └── controllers.yaml
├── launch/
│   └── run_simulation_and_controller.launch
├── hero_chassis_controller_plugins.xml
└── README.md
</code></pre>
<h3><code>package.xml</code>：这个目录的身份和依赖声明</h3>
<p>package.xml (package.xml) 让 catkin 识别：</p>
<pre><code>这是一个名为 hero_chassis_controller 的 ROS 包。
</code></pre>
<p>它还声明了：</p>
<ul>
<li>构建依赖，例如 <code>roscpp</code>、<code>pluginlib</code>、<code>controller_interface</code>；</li>
<li>运行依赖，例如 <code>geometry_msgs</code>、<code>nav_msgs</code>；</li>
<li>构建工具 <code>catkin</code>；</li>
<li>外部工具应该去哪里找插件描述文件。</li>
</ul>
<p>这最后一点落到了实际文件：</p>
<pre><code>&lt;export&gt;
  &lt;controller_interface
      plugin=&quot;${prefix}/hero_chassis_controller_plugins.xml&quot;/&gt;
&lt;/export&gt;
</code></pre>
<p>这里的 <code>${prefix}</code> 最终指向包目录。<code>pluginlib</code> 读取 <code>package.xml</code> 后，知道还要继续看 hero_chassis_controller_plugins.xml (hero_chassis_controller_plugins.xml)。</p>
<h3><code>CMakeLists.txt</code>：源码文件到产物的配方</h3>
<p>CMakeLists.txt (CMakeLists.txt) 中有两个最关键的构建目标：</p>
<pre><code>add_library(
    ${PROJECT_NAME}
    SHARED
    src/${PROJECT_NAME}.cpp
)
</code></pre>
<p>它把：</p>
<pre><code>src/hero_chassis_controller.cpp
</code></pre>
<p>构建成：</p>
<pre><code>devel/lib/libhero_chassis_controller.so
</code></pre>
<p>另一个是：</p>
<pre><code>add_executable(
    wasd_teleop
    src/wasd_teleop.cpp
)
</code></pre>
<p>它把：</p>
<pre><code>src/wasd_teleop.cpp
</code></pre>
<p>构建成：</p>
<pre><code>devel/lib/hero_chassis_controller/wasd_teleop
</code></pre>
<p>此外：</p>
<pre><code>generate_dynamic_reconfigure_options(
    cfg/HeroChassis.cfg
)
</code></pre>
<p>把配置描述文件生成：</p>
<pre><code>devel/include/hero_chassis_controller/HeroChassisConfig.h
</code></pre>
<p>所以源码和产物之间的对应关系是：</p>
<table>
<thead>
<tr>
<th>源码实体</th>
<th>构建动作</th>
<th>产物</th>
<th>运行时由谁使用</th>
</tr>
</thead>
<tbody>
<tr>
<td><code>src/hero_chassis_controller.cpp</code></td>
<td><code>add_library(... SHARED)</code></td>
<td><code>libhero_chassis_controller.so</code></td>
<td>被 <code>controller_manager</code> 通过 pluginlib 加载</td>
</tr>
<tr>
<td><code>src/wasd_teleop.cpp</code></td>
<td><code>add_executable(...)</code></td>
<td><code>wasd_teleop</code></td>
<td>通过 <code>rosrun</code> 启动成节点</td>
</tr>
<tr>
<td><code>cfg/HeroChassis.cfg</code></td>
<td><code>generate_dynamic_reconfigure_options()</code></td>
<td><code>HeroChassisConfig.h</code></td>
<td>插件编译和动态参数功能使用</td>
</tr>
<tr>
<td><code>launch/*.launch</code></td>
<td>不编译成二进制</td>
<td>保持为源码文件</td>
<td><code>roslaunch</code> 读取并执行</td>
</tr>
<tr>
<td><code>config/controllers.yaml</code></td>
<td>不编译成二进制</td>
<td>保持为源码文件</td>
<td>被 launch 或 ROS 组件加载</td>
</tr>
</tbody>
</table>
<p>插件 XML 中的内容也落到了真实库：</p>
<pre><code>&lt;library path=&quot;lib/libhero_chassis_controller&quot;&gt;
  &lt;class
      name=&quot;hero_chassis_controller/HeroChassisController&quot;
      type=&quot;hero_chassis_controller::HeroChassisController&quot;
      base_class_type=&quot;controller_interface::ControllerBase&quot;/&gt;
&lt;/library&gt;
</code></pre>
<p>它表达的不是“控制器会自动运行”，而是：</p>
<pre><code>当 controller_manager 要创建
hero_chassis_controller/HeroChassisController 时，
打开 libhero_chassis_controller.so，
从里面实例化 hero_chassis_controller::HeroChassisController。
</code></pre>
<p>因此这个包实际上产生了两类运行实体：</p>
<pre><code>独立节点进程：wasd_teleop
被加载的插件：libhero_chassis_controller.so
</code></pre>
<p>这两者不能用同一套理解方式。一个是操作系统能够直接执行的程序；另一个是被其他进程加载进地址空间的库。</p>
<h2>上一层对象：终端环境和 ROS 覆盖空间</h2>
<p>现在切换解析对象：<strong>运行这套工作区的 shell 与 ROS 环境</strong>。</p>
<p>工作区本身不会自动生效。<code>catkin_make</code> 只生成文件；当前终端还必须知道去哪里找它们。</p>
<p><code>source devel/setup.bash</code> 做的是把当前终端的环境路径调整为包含这个工作区。之后：</p>
<ul>
<li><code>rospack</code> 能找到源码区的包；</li>
<li><code>rosrun</code> 能找到 <code>devel/lib/...</code> 中的可执行程序；</li>
<li>其他包能找到这里导出的库和生成头文件；</li>
<li>C++ 程序能找到这个工作区里的共享库。</li>
</ul>
<p>于是实际关系是：</p>
<pre><code>/opt/ros/noetic 等系统 ROS 环境
        ↓
catkin_test/devel 作为当前开发覆盖层
        ↓
当前 shell 环境
        ↓
rospack / rosrun / roslaunch
        ↓
具体节点或插件被启动、加载
</code></pre>
<p>多个 ROS 工作区可以形成覆盖关系。先 <code>source</code> 哪个环境，哪个环境中同名包和库就可能优先被找到。这一层解释了“为什么文件明明编译了，但终端运行不到”：程序可能存在于 <code>devel</code>，只是当前终端没有接入对应的开发空间。</p>
<p>这里停在 shell 和覆盖空间层，不再继续拆 bash 的 <code>source</code> 如何执行脚本，也不继续追 ROS Master 和节点发现协议。</p>
<h2>最后用一句话落回实体</h2>
<p>当前工作区的真实运行链是：</p>
<pre><code>package.xml 标记包
    ↓
CMakeLists.txt 描述怎样编译
    ↓
catkin_make 在 build 中编译
    ↓
把可执行程序、共享库、生成头文件写入 devel
    ↓
source devel/setup.bash 把 devel 接入终端
    ↓
rosrun 启动 wasd_teleop
或 roslaunch 启动仿真，并由 controller_manager 加载 libhero_chassis_controller.so
</code></pre>
<p>因此，学习这个文件系统时真正需要抓住的不是“每个隐藏目录都有什么”，而是：</p>
<ul>
<li><code>package.xml</code> 怎样划定包；</li>
<li><code>CMakeLists.txt</code> 怎样把源码映射成产物；</li>
<li><code>build</code> 和 <code>devel</code> 为什么必须分开；</li>
<li><code>setup.bash</code> 怎样把产物接入 ROS 环境；</li>
<li><code>rosrun</code> 启动的是可执行文件，pluginlib 加载的是共享库；</li>
<li>大量 IDE、CMake、Git 目录并不属于 ROS 的核心文件系统语义</li>
</ul>
</details>


接下来还是用理解性的框架语言来说

> 实际是我快饿死了

这个工作空间和cmake有点类似，因为catkin其实就是在cmake之上的一层ros工程的构建工具

这里可以嵌入[ros文件系统](https://www.autolabor.com.cn/book/ROSTutorials/chapter1/15-ben-zhang-xiao-jie/151-roswen-jian-xi-tong.html)的解析

![image-20261024222150320](images/image-20261024222150320.png)

这个catkin 可以用来构建类似的工程，因为类似package.xml这些文件cmake是不会去读的，还有catkin可以实现对ros功能包的一些声明依赖关系的操作，总之是涉及ros层面的文件管理

**工作区的构建**

一般的顺序是，先创建一个顶层的workplace,然后在创src，在src里`catkin_init_workspace`，然后回到顶层执行`catkin_make`然后这个工作空间就初始化好了

这还没完

ros很大的功能实现是依赖功能包

然后进到src，使用`catkin_create_pkg` pkg_name就可以初步创建一个包了

默认有CMakeList.txt和package.xml，这个时候就是构建ros工程的好机会了！

> 我猜师兄师姐不会看到这里，偷偷累一下，我去手敲了3000字累麻了，连续搞了4个半小时，先吃个饭了