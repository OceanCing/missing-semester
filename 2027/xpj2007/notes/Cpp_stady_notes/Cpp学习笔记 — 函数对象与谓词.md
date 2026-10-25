# 函数对象
 - **概念**：重载**函数调用操作符**的类，其对象常称为**函数对象**
 - **别名**：函数对象使用重载的 () 时，行为类似函数调用，也叫**仿函数**
 - **本质**：函数对象(仿函数)是一个**类**，不是一个函数
 - **特点**：
	1. 函数对象在使用时，可以像普通函数那样调用，可以有参数，可以有返回值
	2. 函数对象超出普通函数的概念，函数对象可以有自己的状态
	3. 函数对象可以作为参数传递
 - **总结**：仿函数写法非常灵活，可以作为参数进行传递，是 STL 中改变算法策略的主要手段

示例：函数对象的三个特点
```C++
#include <string>

//1、函数对象在使用时，可以像普通函数那样调用, 可以有参数，可以有返回值
class MyAdd
{
public:
	int operator()(int v1, int v2)
	{
		return v1 + v2;
	}
};

void test01()
{
	MyAdd myAdd;
	cout << myAdd(10, 10) << endl;
}

//2、函数对象可以有自己的状态
class MyPrint
{
public:
	MyPrint()
	{
		count = 0;
	}
	void operator()(string test)
	{
		cout << test << endl;
		count++; //统计使用次数
	}

	int count; //内部自己的状态
};
void test02()
{
	MyPrint myPrint;
	myPrint("hello world");
	myPrint("hello world");
	myPrint("hello world");
	cout << "myPrint调用次数为： " << myPrint.count << endl;
}

//3、函数对象可以作为参数传递
void doPrint(MyPrint& mp, string test)
{
	mp(test);
}

void test03()
{
	MyPrint myPrint;
	doPrint(myPrint, "Hello C++");
}

int main() {

	//test01();
	//test02();
	test03();

	system("pause");

	return 0;
}
```

# 谓词
 - **概念**：返回 bool 类型的仿函数称为**谓词**
 - **分类**：
	1. 如果 operator() 接受一个参数，那么叫做**一元谓词**
	2. 如果 operator() 接受两个参数，那么叫做**二元谓词**

## 一元谓词
示例：用一元谓词配合 find_if 查找
```C++
#include <vector>
#include <algorithm>

//1.一元谓词
struct GreaterFive {
	bool operator()(int val) {
		return val > 5;
	}
};

void test01() {

	vector<int> v;
	for (int i = 0; i < 10; i++)
	{
		v.push_back(i);
	}

	vector<int>::iterator it = find_if(v.begin(), v.end(), GreaterFive());
	if (it == v.end()) {
		cout << "没找到!" << endl;
	}
	else {
		cout << "找到:" << *it << endl;
	}
}

int main() {

	test01();

	system("pause");

	return 0;
}
```

## 二元谓词
示例：用二元谓词改变 sort 的排序策略
```C++
#include <vector>
#include <algorithm>

//二元谓词
class MyCompare
{
public:
	bool operator()(int num1, int num2)
	{
		return num1 > num2;
	}
};

void test01()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(40);
	v.push_back(20);
	v.push_back(30);
	v.push_back(50);

	//默认从小到大
	sort(v.begin(), v.end());
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
	cout << "----------------------------" << endl;

	//使用函数对象改变算法策略，排序从大到小
	sort(v.begin(), v.end(), MyCompare());
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```

# 内建函数对象
 - **概念**：STL 内建了一些函数对象
 - **分类**：
	1. **算术仿函数**
	2. **关系仿函数**
	3. **逻辑仿函数**
 - **用法**：这些仿函数所产生的对象，用法和一般函数完全相同
 - **头文件**：==`#include <functional>`==

## 算术仿函数
 - **功能**：实现四则运算，其中 negate 是一元运算，其他都是二元运算
 - ==`template<class T> T plus<T>`==             //加法仿函数
 - ==`template<class T> T minus<T>`==           //减法仿函数
 - ==`template<class T> T multiplies<T>`==   //乘法仿函数
 - ==`template<class T> T divides<T>`==         //除法仿函数
 - ==`template<class T> T modulus<T>`==         //取模仿函数
 - ==`template<class T> T negate<T>`==           //取反仿函数

示例：
```C++
#include <functional>

//negate
void test01()
{
	negate<int> n;
	cout << n(50) << endl;   //-50
}

//plus
void test02()
{
	plus<int> p;
	cout << p(10, 20) << endl;   //30
}

int main() {

	test01();
	test02();

	system("pause");

	return 0;
}
```

## 关系仿函数
 - **功能**：实现关系对比
 - ==`template<class T> bool equal_to<T>`==               //等于(a == b)
 - ==`template<class T> bool not_equal_to<T>`==        //不等于(a != b)
 - ==`template<class T> bool greater<T>`==                 //大于(a > b)
 - ==`template<class T> bool greater_equal<T>`==      //大于等于(a >= b)
 - ==`template<class T> bool less<T>`==                      //小于(a < b)
 - ==`template<class T> bool less_equal<T>`==           //小于等于(a <= b)
 - **注意**：关系仿函数中最常用的就是 ==`greater<>`== 大于

示例：用 greater 替代自己写的仿函数
```C++
#include <functional>
#include <vector>
#include <algorithm>

void test01()
{
	vector<int> v;

	v.push_back(10);
	v.push_back(30);
	v.push_back(50);
	v.push_back(40);
	v.push_back(20);

	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;

	//自己实现仿函数
	//sort(v.begin(), v.end(), MyCompare());
	//STL内建仿函数  大于仿函数
	sort(v.begin(), v.end(), greater<int>());

	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```

## 逻辑仿函数
 - **功能**：实现逻辑运算（实际应用较少，了解即可）
 - ==`template<class T> bool logical_and<T>`==     //逻辑与
 - ==`template<class T> bool logical_or<T>`==       //逻辑或
 - ==`template<class T> bool logical_not<T>`==     //逻辑非

示例：用 logical_not 配合 transform 取反
```C++
#include <vector>
#include <functional>
#include <algorithm>

void test01()
{
	vector<bool> v;
	v.push_back(true);
	v.push_back(false);
	v.push_back(true);
	v.push_back(false);

	for (vector<bool>::iterator it = v.begin(); it != v.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;

	//逻辑非  将v容器搬运到v2中，并执行逻辑非运算
	vector<bool> v2;
	v2.resize(v.size());
	transform(v.begin(), v.end(), v2.begin(), logical_not<bool>());
	for (vector<bool>::iterator it = v2.begin(); it != v2.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```
