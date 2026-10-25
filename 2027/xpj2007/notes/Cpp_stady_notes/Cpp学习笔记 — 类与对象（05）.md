# 多态
### 多态的基本概念
 - **分类**：
	 1. **静态多态**：函数重载、运算符重载，复用函数名
	 2. **动态多态**：派生类和虚函数实现运行时多态
	 - **区别**：
		1. 静态多态的函数地址早绑定 - 编译阶段确定函数地址
		2. 静态多态的函数地址晚绑定 - 运行阶段确定函数地址
 - **多态满足的条件**：
	 1. 有继承关系
	 2. 子类中重写父类的虚函数（重写：函数返回值类型 函数名参数列表完全一致）
- **多态使用的条件**：父类指针或引用指向子类对象
示例一：
```C++
class Animal
{
public:
	//Speak函数就是虚函数
	//函数前面加上virtual关键字，变成虚函数，那么编译器在编译的时候就不能确定函数调用了。
	virtual void speak()
	{
		cout << "动物在说话" << endl;
	}
};

class Cat :public Animal
{
public:
	void speak()
	{
		cout << "小猫在说话" << endl;
	}
};

class Dog :public Animal
{
public:

	void speak()
	{
		cout << "小狗在说话" << endl;
	}

};
//我们希望传入什么对象，那么就调用什么对象的函数
//如果函数地址在编译阶段就能确定，那么静态联编
//如果函数地址在运行阶段才能确定，就是动态联编

void DoSpeak(Animal & animal)
{
	animal.speak();
}
//
//多态满足条件： 
//1、有继承关系
//2、子类重写父类中的虚函数
//多态使用：
//父类指针或引用指向子类对象

void test01()
{
	Cat cat;
	DoSpeak(cat);


	Dog dog;
	DoSpeak(dog);
}


int main() {

	test01();

	system("pause");

	return 0;
}
```
### 纯虚函数和抽象类
 - **纯虚函数**：
	  - **语法**：==virtual 返回值类型函数名(参数列表)= 0;==
	  - **作用**：在多态中，通常父类中虚函数的实现是毫无意义的，主要都是调用子类重写的内容。
 - **抽象类**：
	1. 类中含有纯虚函数
	2. 无法实例化对象
	3. 子类中必须重写抽象类中的纯虚函数，否则也是抽象类
示例二：
```C++
class Base
{
public:
	//纯虚函数
	//类中只要有一个纯虚函数就称为抽象类
	//抽象类无法实例化对象
	//子类必须重写父类中的纯虚函数，否则也属于抽象类
	virtual void func() = 0;
};

class Son :public Base
{
public:
	virtual void func() 
	{
		cout << "func调用" << endl;
	};
};

void test01()
{
	Base * base = NULL;
	//base = new Base; // 错误，抽象类无法实例化对象
	base = new Son;
	base->func();
	delete base;//记得销毁
}

int main() {

	test01();

	system("pause");

	return 0;
}
```

### 虚析构和纯虚函数
 - **问题**：多态使用时，如果子类中有属性开辟到堆区，那么父类指针在释放时无法调用到子类的析构代码
 - **解决方式**：将父类中的析构函数改为虚析构或者纯虚析构
 - **虚析构和纯虚析构共性**：
	 1. 可以解决父类指针释放子类对象
	 2. 都需要有具体的函数实现
 - **虚析构和纯虚析构区别**：如果是纯虚析构，该类属于抽象类，无法实例化对象
 - **语法**：
	 - **虚析构**：==virtual ~类名() {}==
	 - **纯虚析构**：==virtual ~类名() = 0;==   ==类名::~类名() {}==
 - **总结**:
	1. 虚析构或纯虚析构就是用来解决通过父类指针释放子类对象
	2. 如果子类中没有堆区数据，可以不写为虚析构或纯虚析构
	3. 拥有纯虚析构函数的类也属于抽象类

示例：
```C++
class Animal {
public:

	Animal()
	{
		cout << "Animal 构造函数调用！" << endl;
	}
	virtual void Speak() = 0;

	//析构函数加上virtual关键字，变成虚析构函数
	//virtual ~Animal()
	//{
	//	cout << "Animal虚析构函数调用！" << endl;
	//}


	virtual ~Animal() = 0;
};

Animal::~Animal()
{
	cout << "Animal 纯虚析构函数调用！" << endl;
}

//和包含普通纯虚函数的类一样，包含了纯虚析构函数的类也是一个抽象类。不能够被实例化。

class Cat : public Animal {
public:
	Cat(string name)
	{
		cout << "Cat构造函数调用！" << endl;
		m_Name = new string(name);
	}
	virtual void Speak()
	{
		cout << *m_Name <<  "小猫在说话!" << endl;
	}
	~Cat()
	{
		cout << "Cat析构函数调用!" << endl;
		if (this->m_Name != NULL) {
			delete m_Name;
			m_Name = NULL;
		}
	}

public:
	string *m_Name;
};

void test01()
{
	Animal *animal = new Cat("Tom");
	animal->Speak();

	//通过父类指针去释放，会导致子类对象可能清理不干净，造成内存泄漏
	//怎么解决？给基类增加一个虚析构函数
	//虚析构函数就是用来解决通过父类指针释放子类对象
	delete animal;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```

