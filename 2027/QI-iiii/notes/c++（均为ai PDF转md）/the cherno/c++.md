类型 + 名称 = 数值

```cpp
int a = 10;
```

2. 注释

```cpp
// 单行注释
/*
    多行注释
*/
```

3. 定义

```cpp
#define
const int n = [无法辨认];
```

4. 基本数据类型

整数类型：`short`、`int`、`long`、`long long`

```text
short      2
int        4
long       4
long long  8
```

5. `sizeof`，类型 / 变量 / 函数

```cpp
float  a = 3.14f;
double a = 3.14;
```

浮点数：`float` 4 字节，`double` 8 字节

科学计数法

```text
3e-2 = 3 × 10⁻²
```

9. 字符串 `String`

10. 条件

11. 预处理标志

```cpp
#include <iostream>
```

引入 `input output stream`

12. `main` 为程序入口函数

```cpp
std::cout << "Hi" << std::endl;
```

`std::cout`：本行输出

`std::endl`：换行，刷新

```cpp
std::cin.get();
```

接收输入

16. `debug` 调试，`release` 发布

17. `head file`，预处理器在 `cpp file` 中进行编译，生成 `obj`

18. 单独编译 `Ctrl + F7`

Compile

19. 函数

```cpp
void Log(const char* message)
```

`void` 无返回值

`const char* message`：整型变量，字符类型

17. compiling

```text
cpp -> obj
```

`translation unit`

```text
pre-process -> tokenizing -> parsing
```

18. `if` 条件 A

```cpp
if (A)
{
}
else if (B)
{
}
else if (C)
{
}
else
{
}
```

else：剩余

19. `using namespace std`

以后就可以直接用 `cout`，不用写 `std::cout`

20. `static`

静态变量

整型变量、函数返回值都在函数内置空间中

23. `bool = 1 byte`

```cpp
unsigned int
```

无符号变量

22. `function`

更简便

```text
a    值
&a   地址
p    指针地址
*p   指针值
```

23. `head files`

头文件

```cpp
#pragma once
```

只需一次

```cpp
#ifndef [无法辨认]
#define [无法辨认]
#endif
```

24. `[]` 数组 / `<>` 头文件、模板 / `()` 函数 / `''` 字符

25. Breakpoints

```text
Autos  自动变量
Locals 局部变量
Watch  监控变量
```

内存视图

```text
Debug -> Windows -> Memory
F10 下一行
F11 下一行（进函数）
```

26. `nullptr` 空指针

27. loops

```cpp
for (;;)
{
}

while ()
{
}

do
{
}
while ();
```

`for` 循环

```cpp
for (int i = 0; i < 5; i++)
{
}
```

`continue`：跳过这一次循环

```cpp
for (int i = 1; i < 5; i++)
{
    if (i == 3)
    {
        continue;
    }
    cout << i << endl;
}
```

```text
1 2 4
```

`break`：直接结束循环

`return`：结束函数

29. pointer 指针

```cpp
int main()
{
    int var = 8;
    int* ptr = &var;
    *ptr = 10;
}
```

`ptr` 为 `var` 的地址

由 `ptr` 追踪 `var`，`var` 为 10

30. 值传递、引用

```cpp
#define LOG(x) std::cout << x << std::endl

void Increment(int value)
{
    value++;
}

int main()
{
    int a = 5;
    Increment(a);
    LOG(a);
    std::cin.get();
}
```

```text
传的是复制值
value++ 只改变 value
a 还是 5
```

```cpp
void Increment(int* value)
{
    (*value)++;
}

int main()
{
    int a = 5;
    Increment(&a);
    LOG(a);
    std::cin.get();
}
```

传的是地址

```cpp
void Increment(int& value)
{
    value++;
}

int main()
{
    int a = 5;
    Increment(a);
    LOG(a);
    std::cin.get();
}
```

引用：要改变值

```cpp
int a = 5;
int& ref = a;
```

引用相当于给变量起另外一个名字

```cpp
int a = 5;
int* ptr = &a;
*ptr = 10;

*ref = 2;
```

`&` 取地址，`*` 解引用

31. 类

```cpp
class Player
{
public:
    int x, y;
};
```

`public`：或者公有

32. 结构

```cpp
struct Player
{
    int speed;

    void Move(int xa, int ya)
    {
        x += xa * speed;
        y += ya * speed;
    }
};
```

`xa`：沿 x 方向移动的量，x 速度

33. Log

```text
debug
info
warning
error
trace / message
```

```cpp
class Log
{
public:
    const int LogLevelError = 0;
    const int LogLevelWarning = 1;
    const int LogLevelInfo = 2;

private:
    int m_LogLevel = LogLevelInfo;

public:
    void SetLevel(int level)
    {
        m_LogLevel = level;
    }

    void Warn(const char* message)
    {
        if (m_LogLevel >= LogLevelWarning)
            std::cout << "[WARNING]: " << message << std::endl;
    }

    void Error(const char* message)
    {
        if (m_LogLevel >= LogLevelError)
            std::cout << "[ERROR]: " << message << std::endl;
    }

    void Info(const char* message)
    {
        if (m_LogLevel >= LogLevelInfo)
            std::cout << "[INFO]: " << message << std::endl;
    }
};

int main()
{
    Log log;
    log.SetLevel(log.LogLevelWarning);
    log.Error("hello");
    log.Warn("hello");
    log.Info("hello");
    std::cin.get();
}
```

Log 类：一般总是函数，不执行

构造函数在对象使用时自动执行

静态：`static` 对象内部静态

34. `extern`

声明另一个文件有

```cpp
class Log
{
public:
    static void Error(const char* message)
    {
        std::cout << message << std::endl;
    }
};

int main()
{
    Log::Error("hello");
}
```

通过类调用静态函数

35. 单例模式

```cpp
#include <iostream>

class singleton
{
public:
    static singleton& Get()
    {
        static singleton instance;
        return instance;
    }

    void hello()
    {
    }
};

int main()
{
    singleton::Get().hello();
    std::cin.get();
}
```

单例：只能存在一个文件对象

36. 函数

```cpp
void hello(int a)
{
}

int hello()
{
}

hello(1, 2);
```

37. 成员、对象、参数、变量

成员变量根据类型创建，赋予数据时调用函数

38. `::` 作用域

```cpp
Player::Move();
```

在类中声明，在类外调用

39. `enum` 枚举

指定值，类型为 `int`

```cpp
enum State
{
    Idle,
    Walking,
    Attack
};
```

`Attack` 自动加 1

40. 构造函数

特点

1. 函数名与类名一致

2. 不返回类型

3. 创建对象时自动调用

```cpp
#include <iostream>

class Entity
{
public:
    float X, Y;

    Entity()
    {
        X = 0.0f;
        Y = 0.0f;
    }

    void Print()
    {
        std::cout << X << ", " << Y << std::endl;
    }
};

int main()
{
    Entity e;
    std::cout << e.X << std::endl;
    e.Print();
    std::cin.get();
}
```

41. 析构函数

特点

1. 名与类名一致

2. 无返回类型

3. 有相同参数

4. 创建对象时自动调用

42. 类的继承

定义：让继承类有其的成员函数，成为所有类型的对象

`class Player : public Entity`

注意：`public` 和 `protected`，多继承，不能存在某中 `private`

继承方式

```text
public
private
protected
```

子类中会继承父类的所有成员

`private` 继承的成员不能在类外面使用

多态：根据对象类型创建不同的变量，调用不同的函数

43. 多态

定义：同样的调用代码，操作不同类型的对象，执行不同的函数

44. 虚函数

在父类中使用 `virtual` 的函数

作用：当通过类指针 / 引用调用时，C++ 会根据对象的实际类型决定调用父类还是子类版本

```cpp
#include <iostream>
#include <string>

class Entity
{
public:
    virtual std::string GetName()
    {
        return "Entity";
    }
};

class Player : public Entity
{
private:
    std::string m_Name;

public:
    Player(const std::string& name)
        : m_Name(name)
    {
    }

    std::string GetName() override
    {
        return m_Name;
    }
};

void PrintName(Entity* entity)
{
    std::cout << entity->GetName() << std::endl;
}

int main()
{
    Entity* e = new Entity();
    PrintName(e);

    Player* p = new Player("Cherno");
    PrintName(p);

    std::cin.get();
}
```

45. `final`

父类 / 终类，不允许子类再继承

46. 生成随机数

```cpp
srand((unsigned int)time(NULL));
```

47. 对象获取随机

48. 纯虚函数

```cpp
virtual void Speak() = 0;
```

父类不能实例化，必须由子类实现

抽象类不能直接创建对象

49. 隐式转换、显式转换

```cpp
int a = 5;
int b = 2;

double c = a / b;
```

```text
5 / 2 = 2
```

```cpp
double c = (double)a / b;
double c = static_cast<double>(a) / b;
```

```text
5.0 / 2 = 2.5
```

`static_cast<目标类型>(表达式)`
