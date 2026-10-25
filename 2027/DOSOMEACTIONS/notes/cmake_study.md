# cmake、catkin 与大模型使用 · 学习笔记

> 作者：DOSOMEACTIONS
> 日期：2026-10-25
>
> **这份笔记的写法**：左侧的章节框架由助教/大模型给出，但**每一段的回答都是我自己写的**。
> 凡是我亲手做过实验的地方，我都标了【实测】；凡是还没验证的，我标了【待验证】。

---

## 0. 我写这份笔记的方式（**必填，先写这段**）

【填】用 3–5 句话说明：

- 我在哪些地方真的跑过命令、看到过输出？
  （提示：编译链接四阶段、静态库、catkin_make、Topic/Service、remap/ns……）
- 哪些地方是我"问了大模型才知道"的？
- 有没有哪一段是 AI 直接生成、我照抄的？（**诚实写，老师要的是真实过程**）

```
使用了伪实战的方式，在打起的沙箱里面完成了cmake的基础编译，catkin的构成节点以及ros1的通信。问了 DeepSeek 才知道 build/ 和 devel/ 要写进 .gitignore
（因为里面是编译产物，而且路径是绝对路径，别人 clone 后会出错）

```

---

## 1. 编译链接的四个阶段

### 1.1 四条命令和产物大小

**用 `pid.cpp`（15 行 386 字节）跑一遍，填数字：**

```bash
cd /tmp && mkdir -p stages && cd stages
cp ~/cmake-lib-demo/src/pid/pid.cpp . && cp -r ~/cmake-lib-demo/include .
g++ -E -I include pid.cpp -o pid.i && stat -c%s pid.i
g++ -S -I include pid.cpp -o pid.s && stat -c%s pid.s
g++ -c -I include pid.cpp -o pid.o && stat -c%s pid.o
cp ~/cmake-lib-demo/src/main.cpp . && g++ -I include pid.cpp main.cpp -o app && stat -c%s app
```

| 阶段 | 产物 | 我量到的大小（B）|
|---|---|---|
| 预处理 | `pid.i` | 847 |
| 编译 | `pid.s` | 2604 |
| 汇编 | `pid.o` | 1752 |
| 链接 | `app` | 16688 |

【填】一句话：这四个阶段分别把代码变成了什么？

```
① 预处理：展开 #include 和宏，得到纯 C++（没有 # 开头的行）
② 编译：C++ → 汇编指令（人的可读性下降，机器的可读性上升）
③ 汇编：汇编 → 机器码，打包成 ELF 目标文件（relocatable）
④ 链接：多个 .o + 库 → 可执行文件（地址定下来，外部符号填好）
386 (源码)  →  847 (.i)      变大：头文件被展开进来了
847        →  2604 (.s)      最大：汇编比源码啰嗦得多（一条语句变十几条指令）
2604       →  1752 (.o)      变小：汇编被编码成二进制，密度高
1752       →  16688 (app)    暴涨 10 倍：链接把启动代码、libc/libstdc++ 的
                             重定位信息、符号表都塞进来了
```

### 1.2 `file` 命令输出的两个关键词

```bash
file pid.o      # ELF 64-bit LSB relocatable, ...
file app        # ELF 64-bit LSB pie executable, dynamically linked, ...
```

【填】这两个词我查到的意思（各一句）：

```
relocatable        意思是：

dynamically linked 意思是：

```

### 1.3 我的认知修正

【填】我原来以为 __，实际是 __：

```
纯 cmake 也能编译链接（我在 plain_cmake 里跑通过），
catkin 额外做的是：多包依赖排序、从 msg/srv 生成代码、环境管理
```

---

## 2. 静态链接 vs 动态链接

### 2.1 我机器上的两个真实例子

```bash
ls -la ~/cmake-lib-demo/build/debug/libpid.a     # 静态库
ar t  ~/cmake-lib-demo/build/debug/libpid.a      # 里面装的是什么？
```

【填】

| | 静态库 | 动态库 |
|---|---|---|
| 文件 | `libpid.a` | `/opt/ros/noetic/lib/libroscpp.so` |
| 大小（B）| 5242 | 1768632 |
| 我怎么确认它是哪种 | `ar t` 输出：`pid.cpp.o` | `file` 输出：ELF 64-bit LSB shared object |

### 2.2 `ldd` 的两个不同结果（重点）

同一个 `tracker`，在**宿主机**和**容器里**跑 `ldd`，结果不一样：

```bash
ldd ~/rm_ws/devel/lib/tracker_pkg/tracker | grep roscpp          # 宿主机
rosbox   # 然后
ldd /root/rm_ws/devel/lib/tracker_pkg/tracker | grep roscpp      # 容器
```

【填】宿主机上看到：

```
libroscpp.so => /opt/ros/noetic/lib/libroscpp.so (0x0000783bbe511000)
	libroscpp_serialization.so => /opt/ros/noetic/lib/libroscpp_serializatio
```

【填】容器里看到：

```
	libroscpp.so => not found
	libroscpp_serialization.so => not found

```

【填】为什么不一样？（提示：动态链接器去哪找 `.so`？`source` 改了什么环境变量？）

```



```

### 2.3 一句话比喻

静态链接像自带干粮，动态链接像到地方再买饭。

```
（未完成）


```

---

## 3. CMake 基础

### 3.1 最小可用的三行

【填】写出一个能编译单个源文件的最小 `CMakeLists.txt`：

```cmake
（在这里写三行）


```

### 3.2 target 是什么

【填】一句话：为什么说"CMake 里一切都是围绕 target 的"？举你写过的两个 target 为例。

```
因为 CMake 的输出物（库、可执行文件）都是 target，所有配置都是给某个 target 加属性，
而不是全局设置。我写过两个：pid（静态库）和 hello（可执行文件），
target_link_libraries(hello PRIVATE pid) 就是把这两个 target 连起来。


```

### 3.3 PUBLIC / PRIVATE / INTERFACE（我踩过坑）

**我把 `target_include_directories(pid PUBLIC include/)` 改成 `PRIVATE` 之后，真实报错：**

```
/tmp/.../tests/test_pid.cpp:1:10: fatal error: pid/pid.hpp: 没有那个文件或目录
/tmp/.../src/main.cpp:1:10: fatal error: pid/pid.hpp: 没有那个文件或目录
```

【填】我的理解：

```
因为main路径Include了那个头，但是因为是private没有传播到这里来，
导致了文件目录没有被包进来，造成了头文件的缺失
```

【填】三个关键字一句话区分：

```
PUBLIC     = 我用，链接我的人也能用（会传播）
PRIVATE    = 只有我自己用（不传播）
INTERFACE  = 我不用，但链接我的人要用
```

### 3.4 缓存变量（没做过就写"没做"）

【填】

```
没做这个实验（时间不够，留待以后补）。


```

---

## 4. catkin：在 CMake 上加了什么

### 4.1 `catkin_make` = cmake + make

【填】从它自己的输出里抄两行（`#### Running command:` 开头的）：

```
① Running command: "cmake /root/rm_ws/src ..." in "/root/rm_ws/build"

② Running command: "make -j24 -l24" in "/root/rm_ws/build"
```

### 4.2 工作区三层

【填】`src/` `build/` `devel/` 各是什么、要不要进 git（一句话）：

```
src/ 是我写的源码（要进 git）；build/ 和 devel/ 都是 catkin_make 生成的（不进 git）。
devel/ 里放编译产物和 setup.bash。


```

### 4.3 一个包的五个文件

【填】文件名 + 一句话作用：

```
1. `package.xml`             →  依赖声明（给 catkin 和 rosdep 看）
2. `CMakeLists.txt`          →  编译规则（target、依赖、代码生成）
3. `msg/Detection.msg`       →  消息字段定义
4. `src/tracker.cpp`         →  节点代码
5. `launch/tracker.launch`   →  启动配置（名字、remap、参数）
```

### 4.4 用自定义 msg 必须写的 5 处

```cmake
# ①
find_package(catkin REQUIRED COMPONENTS roscpp std_msgs message_generation)

# ②
add_message_files(FILES Detection.msg)

# ③
generate_messages(DEPENDENCIES std_msgs)

# ④
catkin_package(CATKIN_DEPENDS roscpp std_msgs message_runtime)

# ⑤
add_dependencies(tracker ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
```

**我做的实验：加一个 `.msg` 但不写进 `add_message_files`。**

【填】现象和我的解释：

```
他不会报错，当做什么都没发生就过去了，但是这个msg里面的东西就不会被加进去了，
在你的文件夹里不等于他看到了
```

---

## 5. ROS 工程实践

### 5.1 【实测】Topic vs Service

【填】把你在实验里看到的填进去：

| | Topic | Service |
|---|---|---|
| 我的验证命令 | `rostopic pub` | `rosservice call` |
| 敲下去终端的行为 | 立刻返回 | 卡住等响应 |
| 能不能"偷看" | 能（`rostopic echo`）| 不能（没这个命令）|
| 有没有“频率” | 有（`rostopic hz`）| 没有 |
| 一句话区别 | 广播，发出去不管谁收 | 打电话，必须等结果 |

### 5.2 ⭐ 名字系统导致的"静默失败"

【实测】我跑过：

```bash
rosrun demo_pkg talker   /chatter:=/my_chatter     # 发 /my_chatter
rosrun demo_pkg listener __ns:=/robot1             # 收 /robot1/chatter
```

【填】`rostopic info` 的输出说明什么：一个publisher和只有一个subscriber意味着丢包了，信是中断的

```
/my_chatter        Publishers: talker   Subscribers: NaN
/robot1/chatter    Publishers: NaN   Subscribers: listener
```

【填】为什么会这样？"remap"和"命名空间"分别改了什么？

```
remap 改的是某一个名字，namespace 改的是节点名 + 所有相对名的前缀。
talker 用 remap 发 /my_chatter，listener 用 ns 收 /robot1/chatter，
两个名字不同，master 不撮合，所以收不到。

```

【填】最终怎么让它们连上的：

```bash
:=>
```

### 5.3 launch 文件解决什么问题

【填】用你自己的话（提示：想想你不写 launch 时要敲几条命令）：

```
不写 launch 的话，每个节点都要手敲一条 rosrun，有 remap 和 ns 时还要记住那些参数。
launch 把这些写进一个 XML，一条 roslaunch 全起来，而且名字配置集中在一个地方，不会漏。


```

---

## 6. 我踩过的坑（错题集，至少 5 条）

> 下面五条都是我这次真实撞过的，报错原文是我当时从终端里看/后来翻回来的。

---

### 坑 1 · 忘了把 filter.cpp 加进 add_library → 链接期报 undefined reference

- **场景：** 给 cmake-lib-demo 加一个低通滤波器，`filter.cpp` 和 `filter.hpp` 都写好了、也放进 `src/pid/` 了，但构建失败。
- **我敲的：**

  ```bash
  cmake --build --preset debug
  ```

- **现象：**

  ```
  /usr/bin/ld: CMakeFiles/hello.dir/src/main.cpp.o: in function `main':
  main.cpp:(.text+0x2f6): undefined reference to `lowpass(double, double, double)'
  collect2: error: ld returned 1 exit status
  ```

- **原因：** `add_library(pid STATIC ...)` 里只列了 `pid.cpp`，没列 `filter.cpp`。
  我用 `ar t build/debug/libpid.a` 一看，只输出 `pid.cpp.o` —— 证明 `filter.cpp` 根本没被编译。
- **正确做法：** 把 `src/pid/filter.cpp` 加进 `add_library`，然后**重新配置**（因为改了 CMakeLists）再编译：

  ```bash
  cmake --preset debug && cmake --build --preset debug
  ```

- **教训：** `undefined reference` 是**链接期**错误，第一步应该查"库里有这个符号吗"，
  而不是去查头文件路径。文件在磁盘上 ≠ 被编译。

---

### 坑 2 · 解决冲突时只删了 `<<<<<<<` 符号，留下残字

- **场景：** `git pull --rebase` 撞上 `TASK.md` 冲突，我用编辑器手动合并。
- **我敲的：** 在 vim 里把冲突标记的前缀符号删掉了：

  ```
  <<<<<<< HEAD
  8. TASK.md 会更新（第二课已加入），PR 可能冲突，必须正确处理
  ||||||| parent of 94cd54d (Update notes on conflict handling.)
  8. TASK.md 会更新，可能出现 Conflict
  =======
  8. TASK.md 会更新，可能出现 Conflict（我已学会 rebase 处理）
  >>>>>>> 94cd54d (Update notes on conflict handling.)
  ```

- **现象：** `grep -c "<<<<<<<" TASK.md` 输出 `0`，看起来干净了。但文件实际长这样：

  ```
  HEAD                                     ← 残留
  8. TASK.md 会更新（第二课已加入）...
  parent of 94cd54d (Update notes...)      ← 残留
  8. TASK.md 会更新，可能出现 Conflict
  8. TASK.md 会更新，可能出现 Conflict（我已学会...）
  94cd54d (Update notes on conflict handling.)   ← 残留
  ```

- **原因：** 冲突标记是**整行**（`<<<<<<< HEAD` 整行），我只删了 `<<<<<<<` 这几个符号，
  把 `HEAD` 三个字母留下了。
- **正确做法：** 删**整行**，并用更可靠的检查：

  ```bash
  grep -c "<<<<<<<" 文件                              # 这个会骗你
  grep -n -E "^HEAD$|^parent of|^[0-9a-f]{7} [(]" 文件   # 这个才查得出残留
  wc -l 文件                                          # 行数对不上也是信号
  ```

- **教训：** `grep "^<<<<<<<"` 查不出残字。删标记要删整行，验证要查残留关键字。

---

### 坑 3 · `ros1` 每次新建容器，刚起的 roscore 下一分钟就没了

- **场景：** 在容器里跑通了一套 Topic 实验，做了别的事，回来查话题。
- **我敲的：**

  ```bash
  rostopic info /my_chatter
  ```

- **现象：**

  ```
  ERROR: Unable to communicate with master!
  ```

  五分钟前还在正常用。
- **原因：** `ros1` 启动器内部是 `docker run --rm -it`，**每次执行都是全新容器**。
  我中途退出过一次、又 `ros1` 进来，之前的 roscore 和 talker 都随旧容器消失了。

  查证：

  ```bash
  docker ps --format "{{.ID}} {{.Status}}"
  # 56ba43c2172c  Up 53 seconds      ← 只有 53 秒！说明是刚建的
  ```

- **正确做法：** 要连续做实验就用**持久容器**（`docker exec` 进同一个）：

  ```bash
  docker run -d --name rosbox --net=host -v ~/rm_ws:/root/rm_ws ... sleep infinity
  docker exec -it rosbox bash
  ```

- **教训：** 看 `docker ps` 的 `Up` 时间 —— 只有几十秒就说明是新容器。
  容器是"用完即焚"的，状态不会跨容器保留。

---

### 坑 4 · 名字不匹配导致"静默失败"（不报错，只是收不到）

- **场景：** 想让 listener 单独起一个命名空间，看它还能不能收到 talker 的数据。
- **我敲的：**

  ```bash
  rosrun demo_pkg talker   /chatter:=/my_chatter     # talker 改名
  rosrun demo_pkg listener __ns:=/robot1             # listener 加命名空间
  ```

- **现象：** listener 一条都收不到，**但没有任何报错**。

  查证（`rostopic info` 真实输出）：

  ```
  $ rostopic info /my_chatter
  Type: std_msgs/String

  Publishers:
   * /talker (http://...:34559/)
  Subscribers: None                 ← talker 在发，没人收

  $ rostopic info /robot1/chatter
  Type: std_msgs/String

  Publishers: None                  ← 有人在收，没人发
  Subscribers:
   * /robot1/listener (http://...:40167/)
  ```

- **原因：** remap 改的是"某一个话题名"，命名空间改的是"节点名 + 所有相对名的前缀"。
  两个机制作用在不同节点上，算出来的**最终名字不一样**（`/my_chatter` vs `/robot1/chatter`），
  master 只做字符串匹配，名字不等就不撮合。
- **正确做法：** 让两边最终名字相同：

  ```bash
  rosrun demo_pkg listener chatter:=/my_chatter
  ```

- **教训：** 排查 ROS 通信问题，第一步永远是 `rostopic list` 对名字，
  第二步是 `rostopic info` 看 `Subscribers` 是不是 `None`。
  **没反应不等于没问题，可能是名字对不上。**

---

### 坑 5 · 把 PUBLIC 改成 PRIVATE，头文件路径没传播出去

- **场景：** 想试试 `target_include_directories` 的可见性到底影响什么。
- **我敲的：** 把 `target_include_directories(pid PUBLIC include/)` 改成 `PRIVATE`
- **现象：**

  ```
  /tmp/.../tests/test_pid.cpp:1:10: fatal error: pid/pid.hpp: 没有那个文件或目录
  /tmp/.../src/main.cpp:1:10: fatal error: pid/pid.hpp: 没有那个文件或目录
  ```

  注意：报错的是 `main.cpp` 和 `test_pid.cpp`，**不是** `pid.cpp`。
- **原因：** `PRIVATE` 只对 `pid` 这个 target 自己生效。`pid.cpp` 编译时 `-I include/` 是有的，
  所以它能编过；但 `hello` 和 `test_pid` 链接 `pid` 时，拿不到这个路径。
- **正确做法：** 改回 `PUBLIC` —— 因为头文件是"库的公开 API"，链接库的人也需要它。
- **教训：** `PUBLIC` = 我用，链接我的人也用（传播）；`PRIVATE` = 只有我用。
  **报错出现在"消费者"而不是"提供者"身上时，先想依赖传播。**

---

## 7. 用大模型学习的思考（来自 Emperor 那篇 + 我的经历）

### 7.1 那篇文章的核心论点（**用我自己的话转述**）

【填】不要抄原文，用 3 句话说清它想证明什么：

```
那篇文章说 AI 编程助手不是魔法，核心是一个循环：
用户发消息 到LLM 决定调用哪个工具 ， 程序在本地执行 最后结果回给 LLM。
其中最重要的一句是「LLM 从不直接碰文件系统，它只是请求」。

```

### 7.2 我和大模型交互的三个真实片段

【填】举三个具体例子（越具体越好）：

**片段 1**

- 我问了什么：
- 它怎么答：
- 我后来发现：

**片段 2**

- 我问了什么：
- 它怎么答：
- 我后来发现：

**片段 3**

- 我问了什么：
- 它怎么答：
- 我后来发现：

【提示】可以写：
- "PUBLIC 和 PRIVATE 有什么区别" → 它给了一张表 → 但我真正懂是**改了之后编译报错**那次
- "预期不对啊" → 它没有直接给答案，而是问我"输出和你想的差在哪"
- "catkin_make 到底做了什么" → 它让我去看输出里的两行 Running command

### 7.3 我的判断：什么时候该信它，什么时候不该

【填】至少写两条"该信"和两条"不该信"：

**该信的时候：**

1.
2.

**不该信的时候：**

1.
2.

### 7.4 我以后打算怎么用它

【填】

```
先用它把「为什么」问清楚（比如这次搞懂了 PUBLIC 为什么会传播、动态链接器去哪找 .so），
再自己跑一遍验证。
不把它产出的笔记和代码当最终答案，只当草稿和讲解。


```

---

## 8. 还没搞懂的（诚实清单）

【填】列 2–5 条你现在还是不懂的（**写出来比装作懂更有价值**）：

1. `PUBLIC` 属性传播在 CMake 内部到底是怎么实现的
2. `catkin_package()` 在别人 `find_package` 我的包时具体起了什么作用
3. Action 和 Topic / Service 在底层是什么关系

---

## 附录 · 自查（交之前过一遍）

- [ ] 第 0 节写了，而且诚实
- [ ] 1.1 的数字是**我自己跑出来的**，不是抄的
- [ ] 3.3 我贴了真实的报错原文
- [ ] 4.4 我至少做了「加 .msg 不写进 CMakeLists」这个实验
- [ ] 5.2 我贴了真实的 `rostopic info` 输出
- [ ] 第 6 节至少 5 条坑，每条都有"原因"和"教训"
- [ ] 第 7 节是我自己的话，不是 AI 的
- [ ] 全文没有留下未填的占位符（空着的地方会被看出没写完）
- [ ] 文件名是 `notes/cmake_study.md`（TASK.md 指定）
