# Makefile

编译工具，简化编译过程

## 编译过程

### 1.预处理

-o 指定文件输出叫什么名字

1. 预处理器（cpp）将所有的#define删除，并且展开所有的[宏定义](https://zhida.zhihu.com/search?content_id=194063567&content_type=Article&match_order=1&q=宏定义&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3OTMxMTE3NzYsInEiOiLlro_lrprkuYkiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxOTQwNjM1NjcsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.bsS6YaheufT2GjBkLM7DoAZljhZrjSKaMwwot6jUlio&zhida_source=entity)。
2. 处理所有的条件[预编译指令](https://zhida.zhihu.com/search?content_id=194063567&content_type=Article&match_order=1&q=预编译指令&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3OTMxMTE3NzYsInEiOiLpooTnvJbor5HmjIfku6QiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxOTQwNjM1NjcsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.lIdwJrIbRhkigsvHJFFeqylsOYREnnJNLpuBZuno0i0&zhida_source=entity)，比如#if、#ifdef、#elif、#else、#endif等。
3. 处理#include预编译指令，将被包含的文件直接插入到预编译指令的位置。
4. 删除所有的注释。
5. 添加行号和文件标识，以便编译时产生调试用的行号及[编译错误](https://zhida.zhihu.com/search?content_id=194063567&content_type=Article&match_order=1&q=编译错误&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3OTMxMTE3NzYsInEiOiLnvJbor5HplJnor68iLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxOTQwNjM1NjcsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.9G0h0Cum5zuKumldGy6lwAKlEPqgt2IFAALYPsvhzfw&zhida_source=entity)警告行号。
6. 保留所有的#pragma编译器指令，因为编译器需要使用它们。
7. 使用`gcc -E hello.c -o hello.i`命令来进行预处理， 预处理得到的另一个程序通常是以.i作为文件扩展名。

### 2.编译

c语言编译成低级语言

编译器（ccl）将预处理完的文本文件hello.i进行一系列的词法分析、语法分析、[语义分析](https://zhida.zhihu.com/search?content_id=194063567&content_type=Article&match_order=1&q=语义分析&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3OTMxMTE3NzYsInEiOiLor63kuYnliIbmnpAiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxOTQwNjM1NjcsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.lDQBj5vrPdEbcwSiZgncQZ9QR7-jH1A-PiGlx6MSX3Y&zhida_source=entity)和优化，翻译成文本文件hello.s，它包含一个汇编语言程序。如下所示

1. [词法分析](https://zhida.zhihu.com/search?content_id=194063567&content_type=Article&match_order=3&q=词法分析&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3OTMxMTE3NzYsInEiOiLor43ms5XliIbmnpAiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxOTQwNjM1NjcsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MywiemRfdG9rZW4iOm51bGx9.93cfLMNpIjJlfpoT19iMklZHxB67uEBIiZg8SxVM48c&zhida_source=entity)：扫描器（Scanner）将源代的字符序列分割成一系列的记号（Token）。lex工具可实现词法扫描。
2. 语法分析：语法分析器将记号（Token）产生[语法树](https://zhida.zhihu.com/search?content_id=194063567&content_type=Article&match_order=1&q=语法树&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3OTMxMTE3NzYsInEiOiLor63ms5XmoJEiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxOTQwNjM1NjcsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.4emMngoQMHnAGsdMyIvZ7C1s8vJe990aU4MZWOuEKaQ&zhida_source=entity)（Syntax Tree）。yacc工具可实现语法分析(yacc: Yet Another Compiler Compiler)。
3. 语义分析：静态语义（在编译器可以确定的语义）、动态语义（只能在运行期才能确定的语义）。
4. 源代码优化：源代码优化器(Source Code Optimizer)，将整个语法书转化为中间代码（[Intermediate](https://zhida.zhihu.com/search?content_id=194063567&content_type=Article&match_order=1&q=Intermediate&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3OTMxMTE3NzYsInEiOiJJbnRlcm1lZGlhdGUiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxOTQwNjM1NjcsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.bDCQj9frLkUGzHpwazE68i_5erBGVd6nwAxGLiMgUBM&zhida_source=entity) Code）（中间代码是与目标机器和运行环境无关的）。中间代码使得编译器被分为前端和后端。编译器前端负责产生机器无关的中间代码；编译器后端将中间代码转化为目标机器代码。
5. [目标代码](https://zhida.zhihu.com/search?content_id=194063567&content_type=Article&match_order=2&q=目标代码&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3OTMxMTE3NzYsInEiOiLnm67moIfku6PnoIEiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxOTQwNjM1NjcsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MiwiemRfdG9rZW4iOm51bGx9.6W_bpJ7RVQ5zoV_z4j42OKXaqGhi7coDNzMOA_tSL8I&zhida_source=entity)生成：[代码生成器](https://zhida.zhihu.com/search?content_id=194063567&content_type=Article&match_order=1&q=代码生成器&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3OTMxMTE3NzYsInEiOiLku6PnoIHnlJ_miJDlmagiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxOTQwNjM1NjcsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.JOWDA_tk_14FekoXaSn2lICYP85FJfoY9UXN4x178BQ&zhida_source=entity)(Code [Generator](https://zhida.zhihu.com/search?content_id=194063567&content_type=Article&match_order=1&q=Generator&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3OTMxMTE3NzYsInEiOiJHZW5lcmF0b3IiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxOTQwNjM1NjcsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.ZVfWeIBccgEdkwHP_WOJp7OGPK0ewvHYL4I5RcVbFcY&zhida_source=entity)).
6. 目标代码优化：目标代码优化器(Target Code Optimizer)。

### 3.汇编

汇编器（as）将hello.s翻译成机器语言指令，把这些指令打包成一种叫做**可重定位目标程序**的格式，并将结果保存在目标文件hello.o中，hello.o是一个[二进制文件](https://zhida.zhihu.com/search?content_id=194063567&content_type=Article&match_order=2&q=二进制文件&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3OTMxMTE3NzYsInEiOiLkuozov5vliLbmlofku7YiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxOTQwNjM1NjcsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MiwiemRfdG9rZW4iOm51bGx9.1QTweB1_AolKZSSarIhZrmjpiH43hlIDvYGnmI7bC1c&zhida_source=entity)。

### 4.链接

将所有需要的文件链接起来

链接（linking）是将各种代码和数据片段收集并组合成为一个单一文件的过程，这个文件可被加载（复制）到内存并执行。链接可以执行于**编译时（compile time）**，也就是在源代码被翻译成[机器代码](https://zhida.zhihu.com/search?content_id=194063567&content_type=Article&match_order=2&q=机器代码&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3OTMxMTE3NzYsInEiOiLmnLrlmajku6PnoIEiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoxOTQwNjM1NjcsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MiwiemRfdG9rZW4iOm51bGx9.utDTXa_pK9J3sFBXcTlL4NSNGVCztwYfLU7tO4mnv7A&zhida_source=entity)时；也可以执行于**加载时（load time）**，也就是在程序被**加载器（loader）**加载到内存并执行时；甚至执行于**运行时（run time）**，也就是由应用程序来执行。链接是由叫**链接器（linker）**的程序自动执行的。

## 增量构建

更改了部分文件后，只需要编译更改文件后再链接就可以实现代码功能，省去了对重复文件的编译

## 作用

自动编译说明书，告诉make工具哪些需要编译，使用什么工具和指令

# CMake工具

makefile生成工具，一种过程式语言。具有跨平台的优势

## 流程

```
cmake_minimum_required(VERSION 3.16)//最低版本限制
project(add_demo)//名字
add_executable(demo main.cpp add.cpp)//可执行程序，编译用到的源代码
```

## CMakeLists.txt 

CMakeLists.txt 是 CMake 的核心配置文件，每个 CMake 项目至少需要一个 CMakeLists.txt 文件。

CMake 通过读取文件中的指令来了解项目的结构、源文件列表和编译要求。

## 基本语法

### 指定 CMake 最低版本

该指令必须放在 CMakeLists.txt 的最顶部

`cmake_minimum_required(VERSION 3.10)`

### 定义项目名称和语言

`project(MyProject CXX)`

### 添加可执行文件

`add_library(<目标名> [STATIC | SHARED | MODULE] <源文件>...)`

STATIC：静态库（.a / .lib），编译时直接嵌入可执行文件
 SHARED：动态库（.so / .dll），运行时加载
不指定类型时，由 BUILD_SHARED_LIBS 变量决定

### 链接库到目标

`target_link_libraries(<目标> <库>...)`

### 设置变量

`set(<变量名> <值>...)`

定义一个普通变量，后续通过 ${变量名} 引用

### 为目标指定头文件路径

```
target_include_directories(<目标>[BEFORE | AFTER][SYSTEM][PUBLIC | PRIVATE | INTERFACE]<路径>...)
```

 PUBLIC：当前目标和依赖它的目标都能使用该路径
 PRIVATE：只有当前目标能使用
 INTERFACE：只有依赖当前目标的其他目标能使用

### 设置安装规则

```
install(TARGETS <目标>...
     [RUNTIME DESTINATION <可执行文件安装路径>]
     [LIBRARY DESTINATION <动态库安装路径>]
     [ARCHIVE DESTINATION <静态库安装路径>]
     [INCLUDES DESTINATION <头文件安装路径>])
```

### 条件语句

`if(<条件>) ... elseif(<条件>) ... else() ... endif()`

### 自定义命令

```
dd_custom_command(
      TARGET <目标>
      PRE_BUILD | PRE_LINK | POST_BUILD
      COMMAND <命令> [参数...]
      [COMMENT <说明>]
      [VERBATIM])
```

 PRE_BUILD：在编译前执行
 PRE_LINK：在链接前执行
 POST_BUILD：在构建完成后执行

## 变量

### 普通变量

在 CMakeLists.txt 中定义，作用域为当前目录及其子目录

### 缓存变量

在 CMakeCache.txt 文件中，多次 CMake 运行之间保持值不变。

# AI-如何实现工作流程

## 心理模型介绍

1. **你发送**一条消息（“创建带有 hello world 函数的新文件”）
2. **LLM**决定需要一个工具，并以结构化工具调用（或多个工具调用）进行响应
3. **你的程序**是在本地执行这个工具调用（实际上创建文件）
4. **结果**会被反馈回LLM
5. **LLM**利用这种上下文继续或做出回应

## 所需工具

- **读取文件**以便LLM看到你的代码
- **列出文件**以便导航你的项目
- **编辑文件**这样它就能发出创建和修改代码的指令

## 功能实现部分

### 搭建脚手架

```
import inspect
import json
import os

import anthropic
from dotenv import load_dotenv
from pathlib import Path
from typing import Any, Dict, List, Tuple

load_dotenv()

claude_client = anthropic.Anthropic(api_key=os.environ["ANTHROPIC_API_KEY"])
```

端子颜色可让输出可读

```
YOU_COLOR = "\u001b[94m"
ASSISTANT_COLOR = "\u001b[93m"
RESET_COLOR = "\u001b[0m"
```

```
file.py``/Users/you/project/file.py
```

用于解析文件路径的工具

```
def resolve_abs_path(path_str: str) -> Path:
    """
    file.py -> /Users/you/project/file.py
    """
    path = Path(path_str).expanduser()
    if not path.is_absolute():
        path = (Path.cwd() / path).resolve()
    return path
```

