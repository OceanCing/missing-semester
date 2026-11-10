# **c++**

## **变量与基础**

一些常用的

int a = 1; （整型）

double b= 0.0；（双精度）

float c = 0.0；（单精度）

char d = ‘a’;（字符）

auto e = 1;(自定类型)

每一个语句后面都要加；

= 是赋值

默认为0

### **变量的生命周期**

自动：出 `{}` 就死
静态：程序跑完才死（`static`）
动态：手动 `new/delete`

### **using 声明**

引入命名空间或起别名

`using namespace std;`

``using namespace std;`
`using MyInt = int;`

const 加在前面

### **输入输出**

cin>>x

cout<<a<<"   "<<endl

endl刷新和换行

### **类型转化**

**隐式类型转换**
`double a = 3.14; int b = a;` 

**static_cast 显式类型转换**

强行要求转
`double a = 3.14; int b = static_cast<int>(a);`

---

## **流程**

**for 循环** 知道循环次数 for (int i = 0; i < 5; ++i) {   }，for (auto v : arr) {} 
**while 循环** 看条件循环 while (i < 5) { i++; } 别忘写 i++，否则死循环 ，do-while是先运行一遍

**分支语句** 根据具体值选路 switch (变量) { case 常量: break; } 括号里必须是整型/char，必须写 break 防穿透
**条件语句** 根据真假选路 if (条件) {} else if (条件) {} else {} == 别写成 =

---

## **运算**

| 名             | 大白话理解       | 格式                                                         | tips                         |
| :------------- | :--------------- | :----------------------------------------------------------- | :--------------------------- |
| **算术运算符** | 加减乘除取余     | `+ - * / %（余数）`                                          | `/` 两边都是整数，结果砍小数 |
| **逻辑与关系** | 比大小和组合条件 | `> < == != &&（两个都要） ||（有一个就行） !`                | `a > b && c > d` 的短路求值  |
| **递增递减**   | 自己加1或减1     | 前置 `++i`（先加后用） 后置 `i++`（先用后加）                | 前置效率比后置高             |
| **条件运算符** | 唯一的三元符     | `条件 ? 真值 : 假值`                                         | 别嵌套太多层，可读性差       |
| **成员访问**   | 访问类里的东西   | 对象 `.`（`s1.age`） 指针 `->`（`p->age`） 类名 `::`（`Student::total`） | 对象用点，指针用箭头         |
|                |                  |                                                              |                              |

---

## **容器**

## **vector**

1. 动态数组 vector<int> v = {1,2,3};或不定义vector<int> v
2. v.push_back(4); v.push_back = 2；
3. 下标遍历 像数组一样 for (int i = 0; i < v.size(); ++i) cout << v[i]; 
4. 迭代器遍历 STL 标准范式 for (auto it = v.begin(); it != v.end(); ++it) cout << *it; 遍历时删除元素会导致出错
5. for (const auto& val : v) cout << val; 想修改元素用 auto&，不想改加 const
6. for_each 算法 函数式编程 std::for_each(v.begin(), v.end(), [](int val){...}); 想改元素，Lambda 参数要写 int& val

---

## **函数**

```  
#include <iostream>
using namespace std;

// 1. 函数声明 
// 有叫 add 的函数，收两个 int，返回一个 int

int add(int a, int b); 
              ^
             形参
// 2. 主函数 
int main() {
    int x = 5;（实参）
    int y = 10;
    
    int result = add(x, y); 
    
    cout << "结果是: " << result << endl; // 输出：15
    return 0;
}

void swapNumbers(int& a, int& b);
          这样的是把 上面这 ^  是传真身，不是数值
void 不返回任何值
```

**内联函数**     原地展开
`inline int max(int a, int b) { return a > b ? a : b; }`

## **类和对象**

类相当于图纸

对象相当于由类做出来的实体

格式

```
#include <string>
#include <iostream>

class Person {
    std::string name;
    int age;

public:
    // 构造函数(用初始化列表)
    Person(const std::string& name, int age)
        : name(name), age(age) {}

    // 成员函数
    std::string greet() const {
        return "你好,我是" + name;
    }

    // getter / setter
    const std::string& getName() const { return name; }
    void setName(const std::string& n) { name = n; }
    int getAge() const { return age; }
    void setAge(int a) { age = a; }
};

int main() {
    Person p("张三", 25);
    std::cout << p.greet() << std::endl;
}
```

1. **void Student::print() {}** 类外定义必须加 类名::
2. **this 指针** 我是谁（指向当前对象） this->age = age; 静态函数没有 this
3. **初始化列表** 出生前就把家具摆好 Student(int a) : age(a) {} const 和引用成员必须用它；顺序看声明顺序
4. **常成员与常函数** 常变量刻死不能改，常函数发誓不改 const int id;
5. **void print() const {}** 常量对象只能调常函数
6. **类的作用域** 类是一个独立王国 Student::total = 100; 类外访问必须加类名或对象名
7. **继承** 儿子继承爹的财产 class Dog : public Animal {} 冒号前是儿子，后面是爹
8. **向上转型** 儿子可以当爹用 Animal* p = &d; 爹指针只能看爹那部分
9. **动态绑定** 运行时才决定调谁 爹函数加 virtual，儿子写 override 没加 virtual 就永远是静态绑定
10. **纯虚函数**  virtual void speak() = 0; ，抽象基类 有纯虚函数的类 class Shape { virtual void area() = 0; }; 不能创建对象，必须被继承
11. **虚析构函数**`virtual ~Animal() {}`
12. **动态绑定的前提**必须通过基类的指针或引用（如 `Animal* p = &dog; p->speak();`）去调用虚函数，才能触发动态绑定。如果直接用对象 ，永远是静态绑定，不看虚函数表。

## **数组**

**数组**
`int arr[5] = {1, 2, 3, 4, 5};`

**指针**（存地址的变量，门牌号）
`int a = 10; int* p = &a; *p = 20;`
坑点：指针的加减移动距离由“类型”决定（`int* p++` 移动 4 字节）
坑点：指针可以为空（`nullptr`），也可以中途改指向别人

**引用**（变量的别名，共用内存）
`int a = 10; int& ref = a;`
坑点：必须初始化，不能改绑，不能为空。指针可以空，引用绝不行

**传引用参数**（函数里）
`void swap(int& a, int& b)` 传的是真身，不是拷贝
