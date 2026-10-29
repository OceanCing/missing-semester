# 第二课笔记

## Cmake用法

### Cmake可以将文件内的两个程序连起来形成单独一个exe 以下面为例

**首先声明与创建**

```bash
mkdir -p /c/Users/Max/Projects/cpp-link-demo
cd /c/Users/Max/Projects/cpp-link-demo
```

**然后声明add函数**

```cpp
// math_utils.h
#pragma once
int add(int a, int b);
```

**第一个文件 实现add函数**

```cpp
// math_utils.cpp
#include "math_utils.h"
int add(int a, int b) {
    return a + b;
}
```

**第二个文件 调用并输出**

```cpp
// main.cpp
#include <iostream>
#include "math_utils.h"
int main() {
    std::cout << "3 + 4 = " << add(3, 4) << std::endl;
    return 0;
}
```

**链接**

```cmake
cmake_minimum_required(VERSION 3.16)
project(cpp_link_demo LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(demo main.cpp math_utils.cpp)
```

**进行Cmake构建**

```bash
mkdir -p build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```

**最后就运行**

```bash
./demo.exe
```

### 总结出来格式

```cmake
cmake_minimum_required(VERSION 3.16)
project(my_project LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(demo
    main.cpp
    math_utils.cpp
)
target_include_directories(demo PRIVATE include)
target_link_libraries(demo PRIVATE some_library)
```
