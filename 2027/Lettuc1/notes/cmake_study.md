# cmake学习笔记
## 首先，你需要一个CMakeLists.txt文件
CMakeLists.txt是CMake的核心配置文件，每个CMake项目至少需要一个CMakeLists.txt文件。
指令格式：命令名（参数列表）
### 怎么写CMakeLists.txt
- 指定最低版本：
```javascript
cmake_minimum_required(VERSION 3.10)
```
- 定义项目名称和语言：
```javascript
project(name language(eg:CXX))
```
- 把某文件编译成一个可执行文件：
```javascript
add_executable(wtf wtf.cpp) 
```
ps:把wtf.cpp编译成叫wtf的可执行文件

## 然后，生成Makefile
在终端中的**同一个文件夹**下，输入cmake .（. 表示当前目录）
## 然后开始编译并运行
输入
```javascript
make
```
编译，然后输入
```javascript
./nameofthefile
```
在斜杠后面加文件名运行，例如wtf
# catkin学习笔记
## 首先创建一个工作空间（Workspace
工作空间里有：src（package源代码包），build（cmake&catkin缓存和中间文件），devel（目标文件，就是开发空间，放置开发过程中的可执行文件和库）
```bash
makedir catkin_ws #建一个workspace
cd /catkin_ws&mkdir src #在workspace里面建一个src文件夹
cd src
catkin_init_workspace #初始化工作空间
cd ..
catkin build
```
## 然后需要一个package功能包
```bash
cd ~/catkin_ws/src/
std_msgs rospy roscpp #创建名叫test_pkg的功能包，同时指明依赖的语言（这里分别是python和cpp）
catkin_create_pkg test_pkg std_msgs
rospy roscpp
```
