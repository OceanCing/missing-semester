# ROS1 C++ 学习
个人感觉，学了ros c++再去学ros会轻松一些

## 先理解ROS1的基本运行模型

ROS 程序不是一个单独跑起来的 main 函数，而是运行在 ROS Master 管理下的节点（Node）

- Master：启动任何节点前，通常需要先运行 `roscore`
- Node（节点）：一个可执行程序就是一个节点，每个节点通过 Master 注册自己
- Topic（话题）：节点之间单向、异步传输数据（发布/订阅）
- Service（服务）：节点之间同步、请求/响应式通信

## 创建功能包（之前笔记有）

```bash
cd ~/catkin_ws/src
catkin_create_pkg my_ros_cpp roscpp std_msgs
```

## 一个简单的ROS 1 C++ 程序
在`包/src/` 下创建 `hello.cpp` ：
```cpp
#include "ros/ros.h"

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "hello_node");  // 初始化节点，名字叫 hello_node
    ros::NodeHandle n;                     // 节点句柄，ROS 通信的入口
    ROS_INFO("Hello ROS 1 C++!");          // ROS 的日志输出，替代 printf/cout
    return 0;
}
```

**知识点：**

**一、main函数里的两个参数：argc \ argv[]**
- 下面代码里的 ros::init 会用到它
- `argc`：argument count，命令行参数的个数。它是一个整数，至少为 1（因为程序名本身算一个）
- `argv`：argument vector，命令行参数的字符串数组。argv[0] 是程序自身的名字/路径，argv[1] 是第一个参数，argv[2] 是第二个……
例如在终端执行：
```bash
./my_program hello world
```
此时：
- argc == 3

- argv[0] == "./my_program"

- argv[1] == "hello"

- argv[2] == "world"

注意 `argv` 里存的是 C 语言字符串（char*），不是 `std::string`

- `char *argv[]` 与 `char **argv` 是完全等价的，`char *argv[]` 只是更直观地表达这是一个字符串数组

- **在 ROS 里的具体用途**
ROS 节点从命令行接收参数，比如：
```bash
rosrun my_pkg talker _rate:=10 _name:=robot1
```
这些参数（`_rate:=10` 这种）就是通过 `argv` 传进来的。`ros::init` 的职责之一就是解析 `argc` 和 `argv`，把它们注册到 ROS 的参数系统里

`ros::init` 会：
1. 从 `argv` 里读取出 ROS 相关的参数（如 `__name:=`、`__master:=`、`_param:=`）；

2. 把这些参数从 argv 中摘除（会修改 argc/argv），剩下的非 ROS 参数留给你自己处理；

3. 设置节点名等内部状态。

因此 `ros::init` 的签名里要求这两个参数，就是为了让 ROS 能接入节点启动时的命令行配置

## c++当中的流

**流的本质是：字节的流动**

流有两个方向：

1. 输入流（Input Stream）：数据从外部（键盘、文件、网络）流进你的内存。

2. 输出流（Output Stream）：数据从你的内存流出到外部（屏幕、文件、网络）

**ros里常用的流**

1. 日志输出：`ROS_INFO_STREAM(...)`
```cpp
ROS_INFO_STREAM("Current speed: " << vel << " m/s, Target ID: " << id);
//类比c++中的cout
```

此外，输出日志还可以有c语言风格的写法：`ROS_INFO`
```cpp
std::string s = "hello";
ROS_INFO("%s", s.c_str());
//可类比c语言当中的printf
//注意：%s要的是char*类型，而s是std::string类型的，所以需要 c_str() 进行类型转化，c_str()就是把std::string转换成char*的
```
2. `stringstream`
    - 可以把各种类型的数据“流”进这个字符串里，也可以从这个字符串里“流”出各种类型的数据
    - 使用 `stringstream` 基本就是三步：

        1. 包含头文件 <sstream>

        2. 定义一个对象

        3. 像用 `cout / cin` 一样用它
    ```cpp
    #include <iostream>
    #include <sstream>
    #include <string>

    int main() {
        std::stringstream ss; // 1. 定义一个内存中的流

        // 2. 写入数据（像用 cout 一样）
        int age = 25;
        std::string name = "Tom";
        double score = 95.5;
        
        //把各种类型的数据“流”进ss
        ss << "Name: " << name << ", Age: " << age << ", Score: " << score;

        // 3. 提取为一个普通字符串
        std::string result = ss.str(); 
        //str()就是把流里的内容，提取/转换成一个普通的 std::string
        
        std::cout << result << std::endl; 
        // 输出: Name: Tom, Age: 25, Score: 95.5

        return 0;
    }
    ```

    - 在ros里的应用：`stringstream` 经常被用来拼话题名、日志信息和参数，尤其是在循环或者条件判断里


## 一些知识小整理

看这一段代码
```cpp
ros::NodeHandle n;

ros::Publisher pub = n.advertise<std_msgs::String>("chatter", 1000);

std_msgs::String msg;

msg.data = "hello";

pub.publish(msg);
```
**一、第一行代码**
- `ros::NodeHandle` 是类，所有通信（发布、订阅、服务）都要通过它来发起
- `n`是我创建的一个对象，名字是n（随便取的）

**二、第二行代码**
- `ros::Publisher` ： 类型（ROS 的发布者类）
- `pub` : 变量名，我起的（也可以叫 publisher、chatter_pub，随我）
- `n.advertise<...>(...)` ： 通过节点句柄 `n` 创建一个发布者，返回 `ros::Publisher` 对象
- `std_msgs::String`：声明这个发布者要发什么类型的消息

**三、第三行代码**
- `std_msgs::String` 是类型，msg是用这个类型声明的变量
- `std_msgs` ： 一个 ROS 功能包的名字（standard messages，标准消息包）（在创建包的时候就加进去了，看上述`创建功能包`）
- `String` ： 这个包里定义的一个类，对应 `std_msgs/String.msg` 文件
- `::` ： C++ 作用域解析符，`std_msgs` 这个命名空间里的 `String` 类"
- `msg` ： 起的变量名，类型是 `std_msgs::String`

类比一下
```cpp
std::string s;      // std 命名空间里的 string 类，变量叫 s
std_msgs::String msg;  // std_msgs 命名空间里的 String 类，变量叫 msg
```

**关键点**：`std_msgs::String` 是一个类，`msg` 是这个类的对象

这个类长什么样，大致等价如下
```cpp
namespace std_msgs {
    class String {
    public:
        std::string data;   // 对应 .msg 里的 "string data"
    };
}
```
所以第四行的 `msg.data`就是msg这个对象在类中的成员变量

**四、第五行代码**

- `pub.publish(msg)` 就是调用`pub`这个发布者对象的 publish 成员函数，把 msg 发到它绑定的话题上


## 节点句柄 （所有通信的入口对象）
有三种，各自决定了**后续创建的所有通信（话题、参数、服务）到底挂载在哪个“目录”下**

1. `ros::NodeHandle n;` （全局命名空间）
    - 站在系统的最根目录 `/` 下
    - 后续用它创建的话题、读取的参数，全都是相对于根目录 `/` 的。

        - `n.subscribe("chatter", ...)` ：  实际监听 `/chatter`

        - `n.param("speed", ...)` ： 实际读取 `/speed`

2. `ros::NodeHandle n("robot1");` （相对命名空间）
    - 站在 `/robot1` 这个子目录下
    - 后续用它创建的东西，全都会被自动加上 /robot1 的前缀
        - `n.subscribe("chatter", ...)` ： 实际监听 `/robot1/chatter`

        - `n.param("speed", ...)` ： 实际读取 `/robot1/speed`

    - **使用场景** ：假设我写了一个极其复杂的节点，它同时处理激光雷达和摄像头，还要把处理结果发出去。如果用全局命名空间的 `ros::NodeHandle  n`，所有话题都平铺在根目录下：`/scan`, `/image`, `/result`，非常的乱。这时候，可以用**相对命名空间**把代码组织得很清晰：
    ```cpp
    ros::NodeHandle n_lidar("lidar");
    ros::NodeHandle n_camera("camera");

    n_lidar.advertise<sensor_msgs::LaserScan>("scan", 10);    // 变成 /lidar/scan
    n_camera.advertise<sensor_msgs::Image>("image", 10);      // 变成 /camera/image
    ```

3. `ros::NodeHandle nh("~");` （私有命名空间）
    - 在当前目录下（假设说是`my_node`） （ `~` 符号代表“当前目录”）
    - 后续用它创建的东西，全都会被加上 `/my_node/` 的前缀。

        - `nh.subscribe("chatter", ...)` ： 实际监听 `/my_node/chatter`

        - `nh.param("speed", ...)` ： 实际读取 `/my_node/speed`
    - **使用场景：读取参数** （见下述通信函数 八）
        
## ROS 通信核心函数（`roscpp`里）
一、 `ros::init(argc, argv, "name")`

前面讲过，节点引导：解析 ROS 命令行参数、连 Master、确定节点名、初始化日志/信号/时间系统。每个进程调用一次，且必须在其他 ROS 调用之前

二、 `n.advertise<MsgType>(topic, queue_size)`

创建发布者，返回 `ros::Publisher`
```cpp
ros::Publisher pub = n.advertise<std_msgs::String>("chatter", 1000);
```
- `pub`是发布者
- 这样就创建了一个话题：开一个叫 `/chatter` 的话题，发 `std_msgs::String` 类型的消息

三、 `n.subscribe(topic, queue_size, callback)`

订阅一个话题，返回 `ros::Subscriber`
```cpp
ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
```
参数含义：
- "chatter"：话题名,订阅这个叫chatter的话题
- 1000：消息队列长度 消息先进的先出
- `chatterCallback`：回调函数，签名必须是 `void(const MsgType::ConstPtr&)`
**注意：** `subscribe` 只是订阅了回调函数，不会立刻执行回调。**真正触发回调**的是 `ros::spin()`或`ros::spinOnce()`

四、 `pub.publish(msg)`

把消息发出去（**发布者节点** 直接把数据传给了 **订阅者节点** ）

```cpp
std_msgs::String msg;
msg.data = "hello";
pub.publish(msg);
```
`publish` 是**非阻塞**的，放队列就返回，不保证对方收到

五、 `ros::spin()`

接收消息,调用回调

**阻塞循环**，不断检查有没有新消息/服务请求，有就触发调用对应**回调函数**，直到节点被关闭（`Ctrl+C` 或 `ros::shutdown()`）

**阻塞** 就是说会一直调用回调，后面的代码执行不到（没有消息就一直等）

**每收到一条数据，就自动调用一次回调函数**

六、 `ros::Rate rate()`

控制循环频率
```cpp
ros::Rate rate(10);   // 10 Hz，即一秒最快执行10次循环（如果回调函数代码太复杂太慢，它会变成每秒 5 次、3 次）
```
可用来发布消息（10hz ： 一秒最多发10次），也可以用接收消息（一秒最多接收10次）

七、 `ros::spinOnce()` + `rate.sleep()`

`ros::spinOnce()`也是接收消息,不过与`ros::spin()`的区别在于这个是**非阻塞**的

**非阻塞** 就是不只是等消息，还能干其他事 ： 在一个周期内（比如下面的0.1s）看一眼有没有消息，如果没有消息（列队为空），就瞬间返回，然后执行后面的代码；如果有消息，就先处理消息，处理完后剩下的时间休息或干其他事

假设循环前面 `ros::Rate rate(10)`，也就是0.1s (100ms)接收一次消息

在一次循环里（0.1s），`ros::spinOnce()`在0.1s内完成接收消息调用回调函数（假设花了20ms），那么`rate.sleep()`就会用剩下的时间（80ms）来休息（后面没有代码了）或干其他事（执行后面的代码），避免cpu占用飙升，程序卡死

所以订阅者的 `ros::Rate rate()` 、 `ros::spinOnce()` 、 `rate.sleep()` 一般是配套使用的
```cpp
ros::Rate rate(10);
while (ros::ok()) {
    ros::spinOnce();
    rate.sleep();
}
```
此外，发布者发布消息也可以休眠或干其他事情

```cpp
ros::Rate rate(10);

while (ros::ok()) {
    std_msgs::String msg;
    msg.data = "hello";
    pub.publish(msg);
    
    rate.sleep();
    ... //执行后面的代码
}
```

八、 `n.getParam("name", var) / n.param("name", var, default)`

用来**从 ROS 参数服务器读取参数**

1. `n.getParam("name", var)` —— 严格模式
    - **行为**：去字典(参数服务器)里找 `"name"` 这个键。如果找到了，把值赋给 `var`，并返回 `true` ; 如果没找到，`var` 保持原样，并返回 `false`
    - **注意** ： 这个函数不会帮你设置默认值。如果没找到，你必须自己处理
    - **用法** ：
    ```cpp
    int rate;
    bool success = n.getParam("my_rate", rate);

    if (success) {
        ROS_INFO("成功读取参数: %d", rate);
    } else {
        ROS_ERROR("没找到 my_rate 参数！我得用个默认值了。");
        rate = 10; // 自己兜底
    }
    ```
2. `n.param("name", var, default)` —— 宽容模式
    - **行为**：去字典里找 `"name"`。如果找到了，把值赋给 `var`；如果没找到，直接把 `default` 赋给 `var`。它不返回布尔值（因为一般不需要向上面那个一样判断）
    - **用法**：
    ```cpp
    int rate;
    // 如果 ROS 系统里配置了 my_rate，rate 就是配置的值；
    // 如果没配置，rate 自动变成 10。
    n.param("my_rate", rate, 10); 

    ROS_INFO("最终使用的速度是: %d", rate);
    ```
    
上面的 `n` 可以是全局的，也可以是私有的
1. 如果 `n` 是全局的 :
    ```cpp
    ros::NodeHandle n; // 定义时没有 "~"
    n.getParam("speed", var); 
    ```
    - 此时，`n` 是全局视角。它去参数服务器里找的键是 `/speed`
    - 如果在 `launch` 文件里写如下代码 ， 那函数能找到
    ```xml
    <param name="speed" value="1.5" /> <!-- 存在于 /speed -->
    ```
    - 但如果在 `launch` 文件里写如下代码 (私有参数的写法) ， 那函数就不能找到（返回 false 或用默认值）
    ```xml
    <node name="my_node" ...>
        <param name="speed" value="1.5" /> <!-- 存在于 /my_node/speed -->
    </node>
    ```

2. 如果 `n` 是私有的 ：
    ```cpp
    ros::NodeHandle n("~"); // 定义时带了 "~"
    n.getParam("speed", var);
    ```
    - 此时 `n` 是私有视角（假设节点名叫 `my_node`）。函数去参数服务器里找的键是 `/my_node/speed`

九、 `ROS_INFO` 

老熟人，打印日至宏，用法像c语言的 `printf`

十、 `n.serviceClient<SrvType>(name)` 与 `client.call(srv)`

**客户端**的函数

这个 `SrvType` 是由我自己定义的 比如 ： `my_pkg::AddTwoInts` （.srv放在 `包/srv/`下 只不过SrvType写作 `包::文件名去掉.srv后缀`）

怎么便出来的呢？分四步
1. **第一步：**在包目录的srv目录下（ `包/srv/` ）建一个`.srv`文件（ `AddTwoInts.srv` ）,内容是我自己写的：
```text
int64 a
int64 b
---
int64 sum
```
2. **第二步：**修改 CMakeLists.txt ：把写的这个 `.srv` 文件转换成 C++ 头文件
    1. 修改 `find_package` 。找到 `find_package(catkin REQUIRED COMPONENTS ...)` 这一段，在里面加上 `message_generation`
    ```cmake
    find_package(catkin REQUIRED COMPONENTS
    roscpp
    rospy
    std_msgs
    message_generation   # ★ 加上这一行，告诉 CMake：我要生成自定义消息/服务
    )
    ```
    2. 添加 `add_service_files`
    在模板里找到这一块（大概在中间位置）：
    ```cmake
    ## Generate services in the 'srv' folder
    # add_service_files(
    #   FILES
    #   Service1.srv
    #   Service2.srv
    # )
    ```
    需要把注释解开，并把文件名改成我自己的：
    ```cmake
    add_service_files(
    FILES
    AddTwoInts.srv       # 这里写我 srv/ 目录下的文件名
    )
    ```
    3. 添加 `generate_messages`（紧挨着上一步）

    在 `add_service_files` 的下面，通常还有一个 `generate_messages` 的注释。解开它：
    ```cmake

    generate_messages(
    DEPENDENCIES
    std_msgs             # 如果用了 ROS 自带的数据类型（比如 std_msgs），这里要声明
    )
    ```
    4. 修改 `catkin_package`

    继续往下找，有一个 `catkin_package()`。别人编译你的包时，需要知道你的包里导出了什么。
    ```cmake
    catkin_package(
    #  INCLUDE_DIRS include
    #  LIBRARIES my_pkg
    CATKIN_DEPENDS roscpp rospy std_msgs message_runtime   # ★ 必须加上 message_runtime
    #  DEPENDS system_lib
    )
    ```
3. **第三步：**修改 package.xml

找到 `<build_depend>` 和 `<exec_depend>` 部分，加上这两行：
```xml
<build_depend>message_generation</build_depend>
<exec_depend>message_runtime</exec_depend>
```
4. **第四步：**运行`catkin_make`

之后自动帮你生成了一段 `C++` 代码(是一个`.h`头文件)。这段代码放在了 `devel/include/my_pkg/AddTwoInts.h` 里。**写代码时要#include这个头文件**

自动生成的类，类比C++大概长这样：
```cpp
namespace my_pkg { // 你的包名变成了命名空间
    class AddTwoInts {
    public:
        // 自动根据你的 .srv 文件生成的结构体
        struct Request {
            long int a;
            long int b;
        } request;

        struct Response {
            long int sum;
        } response;
    };
}
```

**这个就可以理解成服务与客户之间的媒介！！客户给a和b，服务传sum，这样客户就能在这个类里得到sum的值了**

**下面解释代码部分**
```cpp
ros::ServiceClient client = n.serviceClient<my_pkg::AddTwoInts>("add_two_ints");

my_pkg::AddTwoInts srv;

srv.request.a = 1; srv.request.b = 2;

if (client.call(srv)) {
    ROS_INFO("sum = %ld", (long)srv.response.sum);
}
```

#### n.serviceClient<SrvType>(name)
```cpp
ros::ServiceClient client = n.serviceClient<my_pkg::AddTwoInts>("add_two_ints");
```
- `ros::ServiceClient` ：这是客户端的类型
- `client` ：变量名
- `n.serviceClient<SrvType>(name)` ： 通过节点句柄 n，向 ROS 系统要一个能连上某个服务的客户端 （`name`是服务的名字）

```cpp
my_pkg::AddTwoInts srv;
srv.request.a = 1; srv.request.b = 2;
```
- `a` 和 `b` 是yaml文件里定义的，经过上面的“四个步骤”后就会变成`my_pkg::AddTwoInts`这个“类”的成员变量（结构体）`request`里的成员变量

#### client.call(srv)

- `client.call(srv)` ： 客户端把 `srv.request` 打包成网络数据，发给服务端。然后，你的程序会卡在这一行（阻塞），直到服务端算完，把结果通过网络发回来，塞进 `srv.response` 里，`call` 才会返回
- `if (client.call(srv)) {...}` ： call 返回一个 bool 值
    - 如果服务端在，且算成功了，返回 `true`

    - 如果服务端挂了/没启动，返回 `false`

    - 所以必须用 `if` 判断一下。如国服务端关机，就直接进 `else`，别去读 `srv.response`

十一、 `n.advertiseService(name, callback)`

服务端注册

```cpp
bool add(my_pkg::AddTwoInts::Request &req, my_pkg::AddTwoInts::Response &res) {
    res.sum = req.a + req.b;
    return true;   // 返回 true 表示处理成功
}
ros::ServiceServer srv = n.advertiseService("add_two_ints", add);
ros::spin();   // 等客户端消息 调用回调
```
和话题里的 `n.subscribe(topic, queue_size, callback)` 差不多

十二、 `ros::ok()`

节点是否还该继续运行。`Ctrl+C` 、`ros::shutdown()` 后变 `false` 。`while (ros::ok())` 是标准写法

十三、 `ros::shutdown()`

主动请求关闭节点，会让 `ros::ok()` 变 `false` 、`spin()` 退出

## 回调函数
与不同函数的区别：
- 普通函数是我**主动调用**的
- 回调函数是 ROS 底层在收到消息时**自动调用**的 （由 `ros::spin()` 或 `ros::spinOnce` 来调用回调）

在 ROS 里，最常见的就是**消息回调**

### 话题的回调
```cpp
#include "ros/ros.h"
#include "std_msgs/String.h"

// 1. 定义回调函数
void chatterCallback(const std_msgs::String::ConstPtr& msg) { //注意msg_p是一个指针！访问成员变量的时候用 -> (msg_p->data)
    ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "listener");
    ros::NodeHandle n;

    // 2. 注册回调函数
    ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);

    // 3. 阻塞等待（每收到一条数据，就调用一次回调函数）
    ros::spin(); 

    return 0;
}
```

回调函数的参数：
- `const` ：老熟人，只读
- `&` ：老熟人，传应用，避免拷贝
- `ConstPtr` ：这是一个智能指针（`boost::shared_ptr` 或 `std::shared_ptr`） 图像、点云数据很大，用指针传递不拷贝数据

## 服务的回调

先把上面的类比图搬下来
```cpp
namespace my_pkg { // 你的包名变成了命名空间
    class AddTwoInts {
    public:
        // 自动根据你的 .srv 文件生成的结构体
        struct Request {
            long int a;
            long int b;
        } request;

        struct Response {
            long int sum;
        } response;
    };
}
```
下面代码能好懂一些
```cpp
bool add(my_pkg::AddTwoInts::Request &req, my_pkg::AddTwoInts::Response &res){
    res.sum = req.a + req.b; //这样就把sum传到了上面的类里，客户端可访问
    return true;   // 返回 true 表示处理成功
}
```
服务端的回调函数固定有两个参数：
1. **第一个参数（req）** ： 只读。ROS 底层收到客户端发来的网络数据后，把它反序列化成了一个 `Request` 对象(req)，传给你。你去里面读 `req.a` 和 `req.b`
2. **第二个参数（res）** ： 只写。作用就是把sum上传到 `Responde` 那个结构体里