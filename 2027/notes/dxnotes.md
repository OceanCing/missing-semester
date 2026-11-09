# 培训第一课笔记

## Linux基础
- Linux的图形化较弱，但系统运行透明，后台操作可查看，多数操作需终端完成（可以把终端当作一个文件管理器）
- 机器人开发中的Linux：开发电脑通过网络或者SSH连接机器人主机，需要用到C++工具链、SSH远程开发、服务自动启动
### 文件系统结构
- Linux的目录从 / 开始：
	- /home/user 用户文
	- /etc 系统配置
	- /usr 程序与库
	- /var 日志等可变数据
	- /dev 设备文件

### 基本命令
- 目录（文件夹）与文件：
	- pwd：显示当前路径
	- ls：列出目录内容（-l 详细信息，-a 包含隐藏文件）
	- cd：切换目录（cd ~ 回家，cd .. 上级）
	- mkdir：创建目录
	- rm：删除文件或目录（-r 递归，-f 强制）
	- cp：复制
	- mv：移动或重命名
	- touch：创建空文件
	- cat、less、head、tail：查看文件内容
- 陌生命令的帮助入口：
	- ls --help （快速查看选项和用法）
	- man ls （打开手册，按q推出）
	- Tab （补全命令或路径）
	- 上下方向键 （调出历史命令）

### 文本编辑器
- nano：简单易用
- vim：功能强大（插入，命令）

## shell

### 什么是脚本
- 脚本本质上是一个文本文件，里面按顺序写了一系列命令，交给解释器（如 bash）自动执行。它的作用是把重复、多步的操作“打包”成一条命令，避免每次手动输入。比如每天备份日志、批量重命名文件，都可以写成脚本一键完成
- Shell 脚本就是写给 Shell（命令行解释器）执行的脚本，Ubuntu 默认用 bash

### shell的四种常见组合
- 变量：
	-name="RM" 这里name就是一个变量，保存了RM （赋值时=两边**不能有空格**）
	-echo "$name" 输出name这个变量的值 （引用变量用$变量名）
- 重定向：
	-echo "hello">hello.txt （>覆盖写入到文件）
	-echo "hello">>hello.txt （>>追加到文件末尾）
- |管道：
	-ls -a|grep txt （把前一个命令的标准输出连接到后一个命令的标准输入）
- 条件连接：
	-mkdir -p demo&&cd demo （前一条成功后再执行下一条）

## vim

### vim模式与必备案按键
- 用vim编辑文件：vim 文件名 （例：vim hello.txt）
- 普通模式 按i 进入插入模式 （按Esc返回普通模式）
- 普通模式输入:后跟命令：
	-:w 保存
	-:q 退出
	-:wq 保存并退出
	-:q! 强制退出不保存
	-:x 保存并退出
- 光标移动（普通模式）：
	- h j k l ：上 下 左 右
- 普通模式按键：
	- dd：是年初当前行
	- yy/p：复制当前行/粘贴
	- u：撤销
	- /world：搜索world

## git

### git的四个位置
1. 工作区 （正在编辑的文件）
2. 暂存区 （本次提交的内容清单）
3. 本地仓库 （已记录的提交历史）
4. github远程仓库 （远程保存的提交历史）
- commit只记录到本地仓库；push才把提交发送到远程

### git常用命令：获取与查看
- git clone 仓库地址 （下载仓库及提交历史）
- git status （查看当前分支和文件状态）
- git diff （查看尚未暂存的修改）
- git log （详细查看提交历史）
- git log --oneline  (用简短格式查看提交历史)
- git reset --hard HEAD~1 (退回到上一个版本)（若要退回第前n个版本，把1换成n）

### git常用命令：分支与提交
- git switch -c feature/hello （创建并切换到功能分支）
- git add 文件 （暂存指定文件）
- git commit -m"版本的说明信息" （记录一次本地提交）
- git push -u origin feature/文件 （推送并建立跟踪关系）
- git switch main （切换主分支）
- git pull （获取并整合当前分支的远程更）



# c++个人笔记

## 冒泡程序
```cpp
#include <iostream>
#include <algorithm> //算法库，有 sort、reverse、find、max、min 等
#include <functional> //函数对象库 有 greater、less、plus 等       
using namespace std;
int main() {
    int a[] = {3, 1, 4, 1, 5, 9, 2, 6};
    int n = sizeof(a) / sizeof(a[0]); // 求数组a的元素个数
    sort(a, a+n, greater<int>()); // 冒泡完成
}
```
### 解释：
1.sizeof 运算符，用来求某个类型或变量占多少字节（byte）：
- sizeof(int) // 4
- sizeof(char) // 1
- sizeof(double) // 8
- int x = 5;
  sizeof(x) // 4

2.int n = sizeof(a) / sizeof(a[0]);
- 其中 sizeof(a) 表示整个数组占的字节数 = 8(int) x 4(字节) = 32
- sizeof(a[0]) 表示一个元素占的字节数 = 4
- (注意，这招只对真正的数组有效，如果数组传给了函数变成指针，sizeof就失效了) (指针大小固定是8字节)

3.sort() 排序函数：
- sort(开始位置, 结束位置, 比较规则)
- 前两个参数是排序范围 (左闭右开)，第三个参数是比较规则，决定升序or降序. (不写第三个参数时默认从小到大)
- greater<int>() 大的放前；less<int>() 小的放前

4.在c++里，数组名a会变成指向第一个元素的指针：
- a ⇔ a[0]，指向第一个元素
- a + 1 ⇔ a[1]
- a+n ⇔ a[n] (不存在)     [a, a+n)

## 地址与指针
- 运算符 &：获得变量的地址，它的操作对象必须是变量
- 指针：就是保存地址的变量
```cpp
     int i;
     int *p = &i;  //*就表示p在这里是一个指针 (p指向i ⇔ p的值是i的变量的地址
```
```cpp
     int *p, q;    //*是加给后面的变量的
     //p是指针 , q是普通int变量
     //指针不会放实际数值
```
```cpp
     int *p, *q;
     //p、q都是指针
```
- *是单目运算符，用来访问指针的值所表示的地址上的变量 (的值)
```cpp
    int i = 6;
    int *p = &i;
    cout << *p << endl; // 6
```

- 指针运用场景：
    - 函数返回运算的状态，结果通过指针返回
    - 函数参数中的数组与指针等价
```cpp
        int sum(int * a, int n)
        //⇔ int sum(int *, int)
        //⇔ int sum(int a[], int n)
        //⇔ int sum(int [], int)
        //&a[1] 与 &a[0] 相差4字节，&a ⇔ a ⇔ &a[0]
```

- 数组变量本身表达地址，所以

```cpp
        int a[10];
        int *p = a;     // 无需用&取地址 (此时p指向a[0])
```

- 数组中的元素表达的是变量，需要用&取地址

```cpp
        int *p = &a[10];
       （a == &a[0]        a+1 == &a[1]）
```

- 指针与 const:
    - 默认状态 int i; int *p = &i;  
    (p后续可以指向其他变量，即后面可以有 p = &j;
    *p的值可以后续被改变，即 *p = 30 )

    - int * const p = &i  
    (此时p是const，p的地址不能再变更，比如后续不能进行 p++; (i的地址加1(四个字节))、也不能再 p = &j;
    但是 *p 不是const，可以改变. *p = 26; // OK )

    - const int *p = &i;  
    (此时 *p 是 const，不能变：*p = 13  // error！ 即不能通过访问地址来改变变量的值. 但是i是可变的：i = 26; // OK
    且p也不是const，也可变  p = &j; // OK)

- 判断哪个被 const 了的标志是 const 在 * 的前面还是后面:
```cpp
    int i;
    const int * p1 = &i;
    int const * p2 = &i;  //与上一行等价
    int * const p3 = &i;
```

- const 数组
    - const int a[] = {1, 2, 3, 4, 5, 6}
    - 数组a[]中的元素均不能再被改变，每个单元都是const int

- 保护数组值，想把数组输入函数里，又希望函数不修改数组的值
    - 可以 int sum(const int a[], int length);

- 指针的运算  （ sizeof(char) = 1, sizeof(int) = 4 ）
```
    char arr[];
    char *p = arr;
    p = p + 1; (p指向的地址+1)
    int brr[];
    int *q = brr;
    q = q + 1; (q指向的地址+4).    (q++; 也可以)
```

- 给一个指针+1 ⇒ 在地址值上加一个sizeof (把指针移到下一个单元)

- 指针打印数组值
```cpp
int a[5] = {0, 1, 2, 3, 4};
cout << *a << endl;   // 0 (a[0])
cout << *(a+1) << endl; // 1 (a[1])
```

- 当两个指针相减时，结果是地址的差除以sizeof(指针类型)

```cpp
    int arr[10];
    int *q = arr;       int *q1 = &a[6];
    cout << (q1 - q)/sizeof(int)<< endl; // 6
    //表示这两地址中间，有几个这样类型的东西
```
## 动态内存分配
- malloc()函数. (给它一个参数，表示我要多大的内存)
	- 需要加头文件 #include <stdlib.h>
	- 可用于不能用变量定义数组元素个数时
```cpp
    #include <stdio.h>
    #include <stdlib.h>
    int main(void) {
        int n;
        cin >> n;
        // int a[n]; // 报错！
        int *a;
        a = (int*)malloc(number * sizeof(int))   //malloc返回的是void*，而a是int*，所以在malloc前面要加(int*)
        (接下来就能把指针a当数组用了)
```
- 注意需要在程序结束前将malloc占的空间还回去
```cpp
    free(a); // 还内存 (如果申请内存失败则返回0，或者叫做 NULL)
    return 0;
```
- free()函数: 注意 free() 里的参数是原地址，如上述例子中，不能对 a++ 后再 free(a)，需让 a 恢复原地址

## 字符串

### C++当中的string
头文件：`#include <string>`

#### 创建 string 的方式

```cpp
string s1;              // 空字符串
string s2 = "hello";    // 从字面量
string s3("hello");     // 构造函数
string s4(5, 'a');      // "aaaaa"
string s5 = s2;         // 拷贝构造
string s6(s2);          // 拷贝构造
string s7(s2, 1, 3);    // 从下标1开始取3个字符 -> "ell"
string s8 = s2 + "world"; // 拼接
```

#### 基本操作
- 赋值：
```cpp
string s;
s = "hello";
s = s2;   // 拷贝
s = 'a';  // 单个字符 (不是字符串，字符串结尾有看不见的 \0)
```

- 访问：
```cpp
string s = "hello";
s[0];       // 'h'，不检查越界
s.at(0);    // 'h'，越界会抛异常 out_of_range
s.front();  // 'h'，第一个字符 
s.back();   // 'o'，最后一个字符 
s[0] = 'H'; // 可以修改 -> "Hello"
```

- 长度：
	- s.size();   // 长度
	- s.empty();  // 是否为空，返回 bool (是返 1，否返 0)
- 比较：
```cpp
string a = "abc"; 
string b = "abd";
a == b;     // false
a < b;      // true (字典序)
a.compare(b); // 返回值<0 表示 a<b
```

#### 常用成员函数
- 查找：
	- s.find("world");   // 6 (若找不到则返回 string::npos)
	- s.find('o');       // 4
	- s.rfind('o');      // 7 (从后往前找)
	- s.find("xyz");     // string::npos
- 子串：
	- s.substr(6);       // "world" (从下标6开始取到末尾)
	- s.substr(0, 5);    // "hello" (从下标0开始取5个字符)
- 插入：
	s.insert(5, "!");  // "hello! world" (下标5插入"!") (注意：插入的是字符串，用双引号)
- 删除：
	s.erase(5, 1);     // 删除下标5开始的1个字符
- 替换：
	s.replace(0, 5, "hi"); // 把从下标0开始的5个字符换成"hi"
- 清空：
	s.clear();         // s 变为空串
- 数字转字符串：
```
	int n = 1;
	string s = to_string(n); // "1"
```
## c++的类(class)与对象(object)

### class基本概念
- 数据：可以把多个变量打包
- 成员函数：属于这个类的函数
- 访问控制：哪些东西外面能碰，哪些不能碰
- 构造/析构：对象怎么出生，怎么死亡
- 继承/多态：类与类之间的关系

### 一个最基本的类
```cpp
class Student { 
public:
    string name;
    int age;
    double score;
    // name, age, score 是成员变量 (数据成员)，不是对象

    void print() { // 成员函数
        cout << name << " " << age << " " << score << endl;
    }
};
```
- 用这个类创建一个对象：
```
	Student s; // 创建对象 (按照 Student 这个模板，造出一个学生(对象)，起名为 s)
	s.name = "张三"; // 根据成员变量对 s 进行定义
	s.age = 18;
	s.score = 90.5;
	s.print(); // 调用成员函数
```
- 可创建多个对象：
```
	Student s1;
	Student s2;
	Student s3;
```
### 访问控制：
- private：只有本类内部能访问
- protected：本类和子类能访问，外面不行
- public：公开

### 构造函数：对象怎么出生，名字和类名相同
```cpp
public:
    Student(string n, int a, double s) {
        name = n;
        age = a;
        score = s;
    }
```
- 使用：Student s("张三", 18, 90.5); （创建对象s）
- 构造函数也可以写成这样（初始化列表）：
```cpp
Student(string n, int a, double s) : name(n), age(a), score(s) {} 
```

### 析沟函数：对象怎么死亡，名字是 ~类名，比如 ~Student() { ... }
```cpp
class IntArray {
private:
    int* data;
    int size;

public:
    IntArray(int n) {
        size = n;
        data = new int[n];
    }

    ~IntArray() {
        delete[] data;
    }
};
```

### this指针
- 每个非静态成员函数里，都有一个隐藏的 this 指针，指向调用这个函数的对象
- this 的类型是 Student *，指向调用这个函数的对象
```cpp
class Student {
private:
    string name;

public:
    void setName(string n) {
        this->name = n; // this->name 是成员，n 是参数
    }
};
```
(注：p->name 等价于 (*p).name)

### 类的继承
- 例子：
```cpp
class Person {
protected:
    string name;
    int age;
public:
    void introduce() { cout << "我是" << name << endl; }
};

class Student : public Person {
    // 自拥有 name、age、introduce
};

class Teacher : public Person {
    // 自拥有 name、age、introduce
};
```
- Person 叫基类（父类），Student、Teacher 叫派生类（子类）
- 基本语法：class 派生类 : 继承方式 基类 { ... };
- 三种继承方式：public、protected、private（最常用的是 public 继承）
（注意：不管哪种继承方式，基类的 private 成员在子类中都不能直接访问）
- 子类构造与析构顺序：子类构造时，要先基类、再成员、最后自己；析构时：先自己，再成员，最后基类
	- 举个例子 基类构造：Person(int i, string n, int a) : id(i), name(n), age(a) {}
	- 子类：
```cpp
private:
    double score;
public:
    Student(int i, string n, int a, double s) :Person(i, n, a), score(s) {}
```

## 数据结构

### 一、链表

#### 1.单链表的模拟实现
- 动态实现：通过 new 动态的申请链表中的结点，通过 delete 来释放结点
- 静态实现：
    - 两个足够大的数组：其中一个数组 elem 存储数据，充当数据域；另一个数组 next 存储下一个元素的下标，充当指针域
    - 两个变量：其中一个变量 h 标记头结点的下标；另一个变量 id 标记新来结点的存储位
- 静态链表头插：
```cpp
e[id] = x;
ne[id] = ne[h];
ne[h] = id;
id++;
```

- 遍历链表：
```cpp
for (int i = ne[h]; i; i = ne[i]) { // 中间的i表示i != 0 
    cout << e[i] << endl;
}
```

- 在任意位置之后插入元素：
```cpp
id++;
e[id] = x;
ne[id] = ne[p];
ne[p] = id; // 时间复杂：O(1)
```

#### 2.双向链表的模拟实现
- 静态实现：创建三个足够大的数组。第一个数组存数据，第二个存前一个元素的下标，第三个存下一个元素的下标
- 头插：
```cpp
id++;
e[id] = x;
pre[id] = h;          // x 前指向 哨兵
ne[id] = ne[h];       // x 后指向 y
pre[ne[h]] = id;      // y 前指向 x
ne[h] = id;           // 哨兵 后指向 x
```

- 在任意位置之后插入元素:
```cpp
id++;
e[id] = x;
pre[id] = p;
ne[id] = ne[p];
pre[ne[p]] = id;
ne[p] = id;           // ne[p] 最后改
```

- 在任意位置之前插入元素:
```cpp
id++;
e[id] = x;
pre[id] = pre[p];
ne[id] = p;
ne[pre[p]] = id;
pre[p] = id;          // pre[p] 最后改
```

- 删除任意位置的元素:
```cpp
ne[pre[p]] = ne[p];
pre[ne[p]] = pre[p];
```

### 二、栈和 stack

#### 1.栈
- 模拟实现：
    - 创建：- 栈还是线性表，因此可创建一个足够大的数组，充当栈结构
            - 再定义一个变量 n，来记录元素个数，同时可标记栈顶位置
            - int stk[N], n;
    - 进栈：栈就是顺序表的尾插   stk[++n] = x;
    - 出栈：顺序表的尾删   n--;
    - 查询栈顶元素   return stk[n]; (栈要不为空)
    - 判断是否为空   bool isempty() { return n == 0; }
    - 查询有效元素个数   return n;
    - 进栈：栈就是顺序表的尾插   stk[++n] = x;
    - 出栈：顺序表的尾删   n--;
    - 查询栈顶元素： return stk[n]; (栈要不为空)
    - 判断是否为空： bool isempty() { return n == 0; }
    - 查询有效元素个数： return n;

#### 2.stack：STL里的栈
    - 创建： stack <T> st; // T 可以是任意类型的数据
    - 进栈： st.push(x);
    - 出栈： st.pop();
    - 查询栈顶元素： st.top();
    - 判断是否为空： st.empty()
    - 查询有效元素个数： st.size()
    - 与上对应，均是 O(1)

### 三、列队和queue

#### 1.列队
- 模拟实现: 
```cpp
const int N = 1e6 + 10;
int q[N]; //一个足够大的数组充当队列
int h; //一个变量 h 标记队头元素的前一个元素
int t; //一个变量 t 标记队尾元素的位置 （h, t] 左开右闭
```
- 入队： q[++t] = x;
- 出队： h++;
- 返回队列里的第一个元素： return q[h + 1];
- 返回队尾元素： return q[t];
- 判断是否为空： return t == h;
- 返回队列元素个数： return t - h;

#### 2.queue：STL当中的队列
- 创建： queue <T> q; // T 可以是任意类型的数据
- 入队： q.push(x);
- 出队： q.pop();
- 返回队头元素： q.front();
- 返回队尾元素： q.back();
- 判断是否为空： q.empty(); // 空返 true，不空返 fals
- 返回队列元素个数： q.size();
- 均与上对应，时间复杂度为 O(1)

### 四、树

#### 1.树的存储
- 用vector数组实现：
```cpp
int n;
vector <int> edges[10]; // 存储树
cin >> n;
for (int i = 1; i < n; i++) {
    int a, b; cin >> a >> b; // a 和 b 之间有一条边
    edges[a].push_back(b);
    edges[b].push_back(a);
}
```
- 链式前向星(单向链表，只不过有不同起点（节点）作哨兵，因此不止一条链):
```cpp
const int N = ...;
int h[N], e[N*2], ne[N*2], id;
void add(int a, int b) {
    id++;
    e[id] = b;
    ne[id] = h[a];
    h[a] = id;
}
int main() {
    int n; cin >> n;
    for (int i = 1; i < n; i++) {
        int a, b; cin >> a >> b;
        add(a, b); add(b, a);
    }
}
```

### 2.树的遍历
- 深度优先遍历 -DFS:
```cpp
const int N = ...;
int n;
vector <int> edges[N]; // 存储图（邻接表）
bool st[N]; // 标记哪些点已经访问过了（bool 数组）

void dfs(int start) {
    cout << start << " ";
    st[start] = true; // 当前点已访问
    for (auto v : edges[start]) { // 遍历所有孩子
        if (!st[v]) { // 如果孩子没被访问
            dfs(v);
        }
    }
}

int main() {
    cin >> n;
    for (int i = 1; i < n; i++) {
        int a, b; cin >> a >> b;
        edges[a].push_back(b);
        edges[b].push_back(a);
    } // 建树图
    
    dfs(1); // 深度优先遍历（从 1 开始遍历）
    return 0;
}
```

- 广度优先遍历 - BFS:
```cpp
bool st[N]; // 标记哪些节点已经访问了
void bfs(int start) {
    queue <int> q;
    q.push(start);
    st[start] = true;
    while (q.size()) { // 当队列不为空
        int u = q.front(); q.pop();
        cout << u << " ";
        for (auto v : edges[u]) { // 将 u 的所有孩子加入队列
            if (!st[v]) { q.push(v); st[v] = true; } // 没访问的加入队列，并标记访问
        }
    }
}
// 在主函数中 bfs(1) -> 从 1 开始遍历
```

