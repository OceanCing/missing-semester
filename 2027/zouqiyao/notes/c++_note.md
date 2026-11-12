# Day 6

## 类的继承
1. 为了 防止代码重复出现
2. 继承有父子关系 子类 拥有 父类 的东西 
3. 子类有 父类没有的东西 


![图片](photo/inherit.png)

动物就是 基类 猫是 派生类 


猫拥有有动物的成员    猫有动物没有的成员 

猫是动物的一个超集合

```cpp
class Animal{
public:
    int blood;
    int bone;
};
class Cat : public Animal{ //继承
public:
   int num_ear;
   int num_feet;
   void printBlood_num_ear()
   {
       std:: cout<< blood <<" "<< num_ear <<std:: endl;
   }   // 调用 了基类的成员 blood 还有 派生类的num_ear
};
```

### 三种继承方法

1. public 继承  继承的数据权限保持不变 private:都不可继承
```cpp
class Father {
public:
    int F_a;
protected:
    int F_b;
private:
    int F_c;
};
class Son : public Father{};
int main(){
    Son a;
    a.F_a = 10; //可以访问 对于 Son 来说 F_a 任然是 public
    a.F_b = 10;//错误 F_b 是 protected 
    //在 Son 类中没有 F_c 它独属于 Father
}
```
2. protected 继承 父类的public 和 protected 权限数据 在 子类里都变成 protected 权限数据 (private 权限是不继承的)
```cpp
class Father
{
public: 
    int m_a;
protected:
    int m_b;
private:
    int m_c;
};
class Son : protected Father
{

};
int main()
{
    Son a;
    a.m_a;//错误 在a中 m_a 数据成为 protected 权限
    a.m_b;//错误 在b中 m_b 数据为protected 权限
    a.m_c;//绝对的错误 private 数据不继承
}
```
3. private 继承 父类的 public 和 protected 权限都在子类变成 private 权限 数据断代 孙类无法继承
```cpp
class Father
{
public: 
    int m_a;
protected:
    int m_b;
private:
    int m_c;
};
class Son : private Father{};//这里所有的Father里的数据都变成了private 权限
class Grandson : public Son{};//无法继承任何的Father 里的数据
```

### 总结 
1. 继承只对 public 和 protected 权限数据有效
2. public 继承 不改变数据权限
3. protected 继承 将数据全部改成 protected 
4. private 继承 将数据全部改成 private

![图片](photo/继承.png)
## 继承 构造 析构顺序
1. 先父类构造再子类构造
2. 先子类析构再父类析构

## 继承中同名成员处理 
1. 优先找本类内的成员 再找父类成员
```cpp
class Base
{
public: 
    int m_a;
    void getM_a(){
        std:: cout<<"Base_ma"<<std::endl;
    }
};
class Subase : public Base
{
public:
    int m_a;
    void getM_a(){
        std:: cout<<"Subase_ma"<<std::endl;
    }
};
int main()
{
    Subase a;
    a.m_a; //访问Subase 的 m_a;
    a.Base::m_a;//访问 Base 的 m_a
    a.getM_a();//访问 Subase 的 getM_a()
    a.Base::getM_a();//访问 Base 的 getM_a()
}
```
2. 当你想访问父类的同名成员 加上Base:: 作用域

## 继承中同名静态成员的处理
1. 编译器会隐藏所有父类同名成员 
2. 需要调用父类同名成员 就要声明作用域
3. 子类作用域包含父类作用域
```cpp
class Base{
public:
    static int a;
    static void func(){
        std:: cout<< "Base_func"<< std:: endl;
    }
};
class Subase: public: Base{
public:
    static int a;
    static void func(){
        std:: cout<< "Subase_func"<< std:: endl;
    }
};

int main()
{
 
   std:: cout<< Subase:: a <<std:: endl;//直接访问是 子类的成员
   std:: cout<< Subase:: Base::a <<std:: endl;// 声明Base:: 调用父类成员
   Subase:: func();//函数也一样
   Subase::Base::func(); 
}
```
总结 ： 同名成员 编译器隐藏 父类成员 只有加作用域 才可调用父类成员

## 多继承语法
1. 一个派生类认多个基类 
```cpp
class Father1{
public:
    int a;
};

class Father2{
public:
    int a;
};
class son: public Father1, public Father2 //派生类可以继承多个基类
{
public:
    int b;
};
int main()
{
    son first;
    first.a;//错误 二义性 这个a 到底是哪里来的 是Father1 还是 Father2
    first.Father1:: a; //需要加一个作用域来明确 到底是哪一个基类
    first.Father2:: a;
}
```
## 菱形继承 和 虚继承
![图片](photo/%E8%8F%B1%E5%BD%A2%E7%BB%A7%E6%89%BF.png)
这就是菱形继承

1. 在实体化Class D 的时候 会实体化 Class B 和 Class C  这俩会分别实体化两个Class A
2. 所以，在Class D 里 有两个实体化的 ClassA 数据
3. 我们想调用 B中的 A中数据   或者   C中的 A 中的数据   就要写 作用域
```cpp
class A
{
public: 
    int a;
};
class B : public A{};
class C : public A{};
class D : public B ,public C{};
int main()
{
    D example;//在我们实体化一个D类的时候 D类里会有两 个A类的数据
    D.a;//错误 不知道是那个a
    D.B::a;//可以 原来是B类中的A类中的a
    D.C::a;//可以 同理
}
```
### 这样会造成资源的浪费 所以要用虚继承
1. 当这个类有俩个派生类(都是虚继承)的时候，它只会实体化一次
```cpp
class A
{
public: 
    int a;
};
class B : virtual public A{};
class C : virtual public A{};
class D : public B ,public C{};
```
2. 类B 中的A 的部分存储的是一个指针 指向唯一的一个实体A类 (其实存储的是一个虚值表，里面有距离唯一的实体A类的距离)
3. 类C 中的A 的部分存储的是一个指针 指向唯一的一个实体A类
```cpp
class A
{
public: 
    int a;
};
class B : virtual public A{};
class C : virtual public A{};
class D : public B ,public C{};
int main()
{
    D example;
    example.a;//正确 取出了唯一的一个a
}
```
![图片](photo/668b4771c13cec262007eb6c25675cce.jpg)
![图片](photo/0c3c312b8fdda76ab3bc070a6764ffdb.jpg)
# Day7
## 多态
### 用基类对象 接收 一个派生类对象
```cpp
class Base{
public:
    void function(){}
};
class Subase : public Base
{
public:
    int a,int b;
};
int main()
{
    Base c;
    Subase a;
    c = a;//用基类对象接收 派生类对象
}
```
![图片](photo/向上切片.png)
1. 在实体化一个 Subase对象时 因为继承 这个对象的地址头 是一个不知名的Base 对象
2. 将 一个Subase 对象赋值给 一个 Base 对象就是将 Subase 对象里 这个不知名的Base 对象拿出来 赋值给 新的 Base对象
   
## 虚函数
```cpp
class Base{
public:
    virtual void function(){} //这里存储的是 指向真正函数实现地址 的指针
};
class Subase : public Base
{
pubilc:
    void function() override{  // 这里才是真实函数实现的地方
        std::cout<<"Subase"<<<std:: endl;
    }
public:
    int a,int b;
};
void test01(Base &ptr)
{
    ptr.function();
}
int main()
{
    Subase a;
    test01(a);
}
```
![图片](photo/虚函数接口.png)

总结： 你传进来什么类 我就用你那个类里重载过的函数  在 基类里面 只起到一个声明函数存在的作用

注意： 只能用指针或者引用对子类进行切片 不然v-ptr 无法被创建 也就无法找到重载的函数了

## 纯虚函数 和 抽象类
1. 纯虚函数 没有实现 仅靠子类里面 重载
```cpp
class Base
{
public:
    virtual void getworld() = 0; //纯虚函数 
};
class Subase : public Base
{
public:
    void getworld() override { //必须靠子类里的实现
        std:: cout<<"Hello world"<<std:: endl;
    }
};
```
2. 拥有纯虚函数的类叫抽象类 不可以 实体化
```cpp
class Base
{
public:
    virtual void getworld() = 0; //纯虚函数 
};
class Subase : public Base
{
public:
    void getworld() override { //必须靠子类里的实现
        std:: cout<<"Hello world"<<std:: endl;
    }
};
void test01(Base* ptr) //一个基类指针指向一个子类  这里会进行切片
{
    ptr->getworld();// 虚函数调用 在子类实现
}
int main()
{
    Base*a = new Base;//错误！！无法实体化
    Subase*a = new Subase;//正确
    test01(a);
}
```
## 虚析构函数
1. 解决了什么问题 ？ 
2. 用基类指针来释放子类中数据
我们来看一个例子:
```cpp
class Base
{
public:
    virtual void getchar() = 0;
};
class Subase : public Base
{
public:
    Subase()
    {
        a = (char*)malloc(sizeof(char)); //这里创建了一个堆区的数据
    }
    ~Subase()
    {
        delete a;//堆区释放
    }
public:
    void getchar(){std::cout<<"gerchar work"<<std::endl;}
private:
    char* a;
}
void test01()
{
    Base* ptr = new Subase; //这里实体化了一个 Subase 对象
    ptr->getchar();//用基类指针
    delete ptr;//这里 ptr 用完了 ptr上的数据会被释放掉  因为是一个Base指针就会调用 Base的析构函数 ***
     // 问题就出现了！！我只调用了Base的析构函数  但是 Subase 的析构函数没有被调用啊!!
    // b 实体上的 *a 也就是堆区上的数据没有被释放 这就内存泄漏了
}
int main()
{
    test01();
}
```
这下怎么办呢？ 我想在我释放基类指针的时候 同时也调用子类的析构函数 
就是想在释放b中的不知名的Base实体时  将整一个 b给释放掉

虚析构函数派上用场
```cpp
class Base
{
public:
    virtual ~Base(){cout<<"Base析构">>} //虚的析构函数 在调用这个函数时会直接跳到子类的虚构函数上去
                      // 原理同 虚函数
public:
    virtual void getchar() = 0;
};
class Subase : public Base
{
public:
    Subase()
    {
        a = (char*)malloc(sizeof(char)); //这里创建了一个堆区的数据
    }
    ~Subase()
    {
        delete a;//堆区释放
    }
public:
    void getchar(){std::cout<<"gerchar work"<<std::endl;}
private:
    char* a;
}
void test01()
{
    Base* ptr = new Subase;
    ptr->getchar();//用基类指针
    delete ptr; //哦！在调用 Base类的析构函数时 跳到了 Subase 类的析构函数上 b中的a被释放了 一切安好
                //注意 ： 在Subase 析构函数运行完之后 Base的析构函数（如果你写了的话）会再运行一次
}
int main()
{
    test01();
}
```
OK                   这样就一切安好了

## This指针
1. This指针是一个实体对象的地址
```cpp
class Student
{
public:
    void SetA(int a)
    {
        A = a;
    }
private:
    int A;
};
int main()
{
    Student zhangsan; //在这里实例化了一个对象
    // this = &zhangsan; this 指针相当于 这个
}
```
2. 但是this之指针只能在类里卖使用,类中可直接写this
```cpp
class Student
{
public:
    void SetA(int a)
    {
        A = a;
    }
    void Getadd()
    {
        return static_cast<long long> this;//可以直接写这个关键字
    }
private:
    int A;
};
```
3. 成员函数的实现都离不开this指针
```cpp
class Student
{
public:
    void SetA(/*Student*this*/,int a) //每个成员函数在实现的时候，c++都会把一个this指针传进函数里
    {
        A = a;//这里的A是谁的A呢？ c++底层默认 this->A;
              // 所以相当于 this->A = a;
    }
private:
    int A;
};
```
4. 那它有什么作用呢 区分同名的局部变量和成员变量
```cpp
class Student
{
public:
    void SetA(int a)
    {
        this->a = a;//声明我这个a是一个类对象的a  而不是局部变量a
    }
private:
    int a;
};
```