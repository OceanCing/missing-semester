C++面向对象的三大特性为:封装、继承、多态
C++认为万事万物都皆为对象，对象上有其属性和行为

# 封装
## 封装的意义

 - 1. 将属性和行为作为一个整体，表现生活中的事物
 - 2. 将属性和行为加以权限控制
###### 封装的意义一：
 - 在设计类的时候，属性和行为写在一起，表现事物
 - **语法**：==class 类名{访问权限：属性 / 行为};==

 一、设计一个圆类，求圆的周长：
```C++
 //圆周率
const double PI = 3.14;

//1、封装的意义
//将属性和行为作为一个整体，用来表现生活中的事物

//封装一个圆类，求圆的周长
//class代表设计一个类，后面跟着的是类名
class Circle
{
public:  //访问权限  公共的权限

	//属性
	int m_r;//半径

	//行为
	//获取到圆的周长
	double calculateZC()
	{
		//2 * pi  * r
		//获取圆的周长
		return  2 * PI * m_r;
	}
};

int main() {

	//通过圆类，创建圆的对象
	// c1就是一个具体的圆
	Circle c1;
	c1.m_r = 10; //给圆对象的半径 进行赋值操作

	//2 * pi * 10 = = 62.8
	cout << "圆的周长为： " << c1.calculateZC() << endl;

	system("pause");

	return 0;
}
```

二、设计一个学生类，属性有姓名和学号，可以给姓名和学号赋值，可以显示学生的姓名和学号：

```C++
//学生类
class Student {
public:
	void setName(string name) {
		m_name = name;
	}
	void setID(int id) {
		m_id = id;
	}

	void showStudent() {
		cout << "name:" << m_name << " ID:" << m_id << endl;
	}
public:
	string m_name;
	int m_id;
};

int main() {

	Student stu;
	stu.setName("德玛西亚");
	stu.setID(250);
	stu.showStudent();

	system("pause");

	return 0;
}
```

###### 封装的愿意二：
 - 类在设计时，可以把属性和行为放在不同的权限下，加以控制
 - 访问权限有三种:
	1. public       保护权限    (类内可以访问，类外可以访问)
	2. protected 保护权限    (类内可以访问，类外不可以访问)
	3. private      私有权限    (类内可以访问，类外不可以访问)
 - 成员属性设置为私有的优点：
	 1. 将所有成员属性设置为私有，可以自己控制读写权限
	 2. 写入时，我们可以检测数据的有效性