# 常用算法
 - **概述**：算法主要是由头文件 ==`<algorithm>`==、==`<functional>`==、==`<numeric>`== 组成
	1. ==`<algorithm>`== 是所有STL头文件中最大的一个，范围涉及到比较、交换、查找、遍历操作、复制、修改等等
	2. ==`<numeric>`== 体积很小，只包括几个在序列上面进行简单数学运算的模板函数
	3. ==`<functional>`== 定义了一些模板类，用以声明函数对象

## 常用遍历算法
 - ==`for_each`==        //遍历容器
 - ==`transform`==     //搬运容器到另一个容器中

### for_each
 - **功能**：实现遍历容器，实际开发中最常用的遍历算法，需要熟练掌握
 - ==`for_each(iterator beg, iterator end, _func);`==
	 - `beg`：开始迭代器；`end`：结束迭代器；`_func`：函数或者函数对象，对每个元素进行的操作

示例：
```C++
#include <algorithm>
#include <vector>

//普通函数
void print01(int val)
{
	cout << val << " ";
}
//函数对象
class print02
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{
	vector<int> v;
	for (int i = 0; i < 10; i++)
	{
		v.push_back(i);
	}

	//遍历算法，第三个参数既可以是普通函数，也可以是函数对象
	for_each(v.begin(), v.end(), print01);
	cout << endl;

	for_each(v.begin(), v.end(), print02());
	cout << endl;
}
```

### transform
 - **功能**：搬运容器到另一个容器中
 - ==`transform(iterator beg1, iterator end1, iterator beg2, _func);`==
	 - `beg1`：源容器开始迭代器；`end1`：源容器结束迭代器；`beg2`：目标容器开始迭代器；`_func`：函数或者函数对象，对每个元素进行的操作
 - **注意**：搬运的目标容器必须要**提前开辟空间**，否则无法正常搬运

示例：
```C++
#include <vector>
#include <algorithm>

class TransForm
{
public:
	int operator()(int val)
	{
		return val;
	}
};

class MyPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{
	vector<int> v;
	for (int i = 0; i < 10; i++)
	{
		v.push_back(i);
	}

	vector<int> vTarget;          //目标容器
	vTarget.resize(v.size());     //目标容器需要提前开辟空间

	transform(v.begin(), v.end(), vTarget.begin(), TransForm());

	for_each(vTarget.begin(), vTarget.end(), MyPrint());
}
```

## 常用查找算法
 - ==`find`==                     //查找元素
 - ==`find_if`==                //按条件查找元素
 - ==`adjacent_find`==     //查找相邻重复元素
 - ==`binary_search`==     //二分查找法
 - ==`count`==                  //统计元素个数
 - ==`count_if`==             //按条件统计元素个数

### find
 - **功能**：查找指定元素，找到返回第一个指定元素的迭代器且不进行后续查找，找不到返回结束迭代器 end()
 - ==`find(iterator beg, iterator end, value);`==
	 - 按值查找元素，找到返回指定位置迭代器，找不到返回结束迭代器位置
 - **注意**：查找自定义数据类型时，需要重载 ==`operator==`==

示例：
```C++
#include <algorithm>
#include <vector>
#include <string>

class Person {
public:
	Person(string name, int age)
	{
		this->m_Name = name;
		this->m_Age = age;
	}
	//重载==
	bool operator==(const Person& p)
	{
		if (this->m_Name == p.m_Name && this->m_Age == p.m_Age)
		{
			return true;
		}
		return false;
	}

public:
	string m_Name;
	int m_Age;
};

void test02() {

	vector<Person> v;

	//创建数据
	Person p1("aaa", 10);
	Person p2("bbb", 20);
	Person p3("ccc", 30);
	Person p4("ddd", 40);

	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);

	vector<Person>::iterator it = find(v.begin(), v.end(), p2);
	if (it == v.end())
	{
		cout << "没有找到!" << endl;
	}
	else
	{
		cout << "找到姓名:" << it->m_Name << " 年龄: " << it->m_Age << endl;
	}
}
```

### find_if
 - **功能**：按条件查找元素，使查找更加灵活，提供的仿函数可以改变不同的策略，其他与find相同
 - ==`find_if(iterator beg, iterator end, _Pred);`==
	 - `_Pred`：函数或者谓词（返回 bool 类型的仿函数）,查找条件

示例：
```C++
#include <algorithm>
#include <vector>
#include <string>

//内置数据类型
class GreaterFive
{
public:
	bool operator()(int val)
	{
		return val > 5;
	}
};

void test01() {

	vector<int> v;
	for (int i = 0; i < 10; i++) {
		v.push_back(i + 1);
	}

	vector<int>::iterator it = find_if(v.begin(), v.end(), GreaterFive());
	if (it == v.end()) {
		cout << "没有找到!" << endl;
	}
	else {
		cout << "找到大于5的数字:" << *it << endl;
	}
}

//自定义数据类型
class Greater20
{
public:
	bool operator()(Person& p)
	{
		return p.m_Age > 20;
	}
};

void test02() {

	vector<Person> v;
	Person p1("aaa", 10);
	Person p2("bbb", 20);
	Person p3("ccc", 30);
	Person p4("ddd", 40);

	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);

	vector<Person>::iterator it = find_if(v.begin(), v.end(), Greater20());
	if (it == v.end())
	{
		cout << "没有找到!" << endl;
	}
	else
	{
		cout << "找到姓名:" << it->m_Name << " 年龄: " << it->m_Age << endl;
	}
}
```

### adjacent_find
 - **功能**：查找相邻重复元素，返回相邻元素的第一个位置的迭代器
 - ==`adjacent_find(iterator beg, iterator end);`==
 - **注意**：面试题中如果出现查找相邻重复元素，记得用 STL 中的 adjacent_find 算法

示例：
```C++
#include <algorithm>
#include <vector>

void test01()
{
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(5);
	v.push_back(2);
	v.push_back(4);
	v.push_back(4);
	v.push_back(3);

	//查找相邻重复元素
	vector<int>::iterator it = adjacent_find(v.begin(), v.end());
	if (it == v.end()) {
		cout << "找不到!" << endl;
	}
	else {
		cout << "找到相邻重复元素为:" << *it << endl;
	}
}
```

### binary_search
 - **功能**：查找指定元素是否存在，查到返回 true 否则 false
 - ==`bool binary_search(iterator beg, iterator end, value);`==
 - **注意**：在**无序序列中不可用**，二分查找法查找效率很高，但要求容器中元素必须是有序序列

示例：
```C++
#include <algorithm>
#include <vector>

void test01()
{
	vector<int> v;

	for (int i = 0; i < 10; i++)
	{
		v.push_back(i);
	}
	//二分查找
	bool ret = binary_search(v.begin(), v.end(), 2);
	if (ret)
	{
		cout << "找到了" << endl;
	}
	else
	{
		cout << "未找到" << endl;
	}
}
```

### count
 - **功能**：统计元素出现次数
 - ==`count(iterator beg, iterator end, value);`==
 - **注意**：统计自定义数据类型时候，需要配合重载 ==`operator==`==

示例：
```C++
#include <algorithm>
#include <vector>

class Person
{
public:
	Person(string name, int age)
	{
		this->m_Name = name;
		this->m_Age = age;
	}
	bool operator==(const Person& p)
	{
		if (this->m_Age == p.m_Age)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	string m_Name;
	int m_Age;
};

void test02()
{
	vector<Person> v;

	Person p1("刘备", 35);
	Person p2("关羽", 35);
	Person p3("张飞", 35);
	Person p4("赵云", 30);
	Person p5("曹操", 25);

	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);
	v.push_back(p5);

	Person p("诸葛亮", 35);

	int num = count(v.begin(), v.end(), p);   //只比较年龄，结果为3
	cout << "num = " << num << endl;
}
```

### count_if
 - **功能**：按条件统计元素出现次数
 - ==`count_if(iterator beg, iterator end, _Pred);`==
	 - `_Pred`：谓词，统计条件
 - **注意**：按值统计用 count，按条件统计用 count_if

示例：
```C++
#include <algorithm>
#include <vector>

class Greater4
{
public:
	bool operator()(int val)
	{
		return val >= 4;
	}
};

//内置数据类型
void test01()
{
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(4);
	v.push_back(5);
	v.push_back(3);
	v.push_back(4);
	v.push_back(4);

	int num = count_if(v.begin(), v.end(), Greater4());

	cout << "大于4的个数为： " << num << endl;
}

//自定义数据类型
class AgeLess35
{
public:
	bool operator()(const Person& p)
	{
		return p.m_Age < 35;
	}
};
void test02()
{
	vector<Person> v;

	Person p1("刘备", 35);
	Person p2("关羽", 35);
	Person p3("张飞", 35);
	Person p4("赵云", 30);
	Person p5("曹操", 25);

	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);
	v.push_back(p5);

	int num = count_if(v.begin(), v.end(), AgeLess35());
	cout << "小于35岁的个数：" << num << endl;
}
```

## 常用排序算法
 - ==`sort`==                     //对容器内元素进行排序
 - ==`random_shuffle`==    //洗牌，指定范围内的元素随机调整次序
 - ==`merge`==                  //容器元素合并，并存储到另一容器中
 - ==`reverse`==                //反转指定范围的元素

### sort
 - **功能**：对容器内元素进行排序，属于开发中最常用的算法之一，需熟练掌握
 - ==`sort(iterator beg, iterator end, _Pred);`==
	 - `_Pred`：谓词，排序方式，不填则默认从小到大排序

示例：
```C++
#include <algorithm>
#include <vector>

void myPrint(int val)
{
	cout << val << " ";
}

void test01() {
	vector<int> v;
	v.push_back(10);
	v.push_back(30);
	v.push_back(50);
	v.push_back(20);
	v.push_back(40);

	//sort默认从小到大排序
	sort(v.begin(), v.end());
	for_each(v.begin(), v.end(), myPrint);
	cout << endl;

	//从大到小排序，使用内建关系仿函数
	sort(v.begin(), v.end(), greater<int>());
	for_each(v.begin(), v.end(), myPrint);
	cout << endl;
}
```

### random_shuffle
 - **功能**：洗牌，指定范围内的元素随机调整次序
 - ==`random_shuffle(iterator beg, iterator end);`==
 - **注意**：使用时记得加随机数种子 ==`srand((unsigned int)time(NULL));`==

示例：
```C++
#include <algorithm>
#include <vector>
#include <ctime>

class myPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{
	srand((unsigned int)time(NULL));
	vector<int> v;
	for (int i = 0; i < 10; i++)
	{
		v.push_back(i);
	}
	for_each(v.begin(), v.end(), myPrint());
	cout << endl;

	//打乱顺序
	random_shuffle(v.begin(), v.end());
	for_each(v.begin(), v.end(), myPrint());
	cout << endl;
}
```

### merge
 - **功能**：两个容器元素合并，并存储到另一容器中
 - ==`merge(iterator beg1, iterator end1, iterator beg2, iterator end2, iterator dest);`==
	 - dest：目标容器开始迭代器
 - **注意**：两个容器必须是**有序的**，目标容器需要提前开辟空间

示例：
```C++
#include <algorithm>
#include <vector>

class myPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{
	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10; i++)
	{
		v1.push_back(i);
		v2.push_back(i + 1);
	}

	vector<int> vtarget;
	//目标容器需要提前开辟空间
	vtarget.resize(v1.size() + v2.size());
	//合并  需要两个有序序列
	merge(v1.begin(), v1.end(), v2.begin(), v2.end(), vtarget.begin());
	for_each(vtarget.begin(), vtarget.end(), myPrint());
	cout << endl;
}
```

### reverse
 - **功能**：将容器内元素进行反转
 - ==`reverse(iterator beg, iterator end);`==
 - **注意**：reverse 反转区间内元素，面试题可能涉及到

示例：
```C++
#include <algorithm>
#include <vector>

class myPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(30);
	v.push_back(50);
	v.push_back(20);
	v.push_back(40);

	cout << "反转前： " << endl;
	for_each(v.begin(), v.end(), myPrint());
	cout << endl;

	cout << "反转后： " << endl;

	reverse(v.begin(), v.end());
	for_each(v.begin(), v.end(), myPrint());
	cout << endl;
}
```

## 常用拷贝和替换算法
 - ==`copy`==              //容器内指定范围的元素拷贝到另一容器中
 - ==`replace`==         //将容器内指定范围的旧元素修改为新元素
 - ==`replace_if`==    //容器内指定范围满足条件的元素替换为新元素
 - ==`swap`==              //互换两个容器的元素

### copy
 - **功能**：容器内指定范围的元素拷贝到另一容器中
 - ==`copy(iterator beg, iterator end, iterator dest);`==
	 - dest：目标起始迭代器
 - **注意**：利用 copy 算法在拷贝时，目标容器记得提前开辟空间

示例：
```C++
#include <algorithm>
#include <vector>

class myPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{
	vector<int> v1;
	for (int i = 0; i < 10; i++) {
		v1.push_back(i + 1);
	}
	vector<int> v2;
	v2.resize(v1.size());
	copy(v1.begin(), v1.end(), v2.begin());

	for_each(v2.begin(), v2.end(), myPrint());
	cout << endl;
}
```

### replace
 - **功能**：将区间内旧元素替换成新元素
 - ==`replace(iterator beg, iterator end, oldvalue, newvalue);`==

示例：
```C++
#include <algorithm>
#include <vector>

void test01()
{
	vector<int> v;
	v.push_back(20);
	v.push_back(30);
	v.push_back(20);
	v.push_back(40);
	v.push_back(50);
	v.push_back(10);
	v.push_back(20);

	//将容器中的20 替换成 2000
	replace(v.begin(), v.end(), 20, 2000);
	for_each(v.begin(), v.end(), myPrint());
	cout << endl;
}
```

### replace_if
 - **功能**：将区间内满足条件的元素，替换成指定元素，可以利用仿函数灵活筛选满足的条件
 - ==`replace_if(iterator beg, iterator end, _pred, newvalue);`==
	 - `_pred`：谓词，替换条件；`newvalue`：替换的新元素

示例：
```C++
#include <algorithm>
#include <vector>

class ReplaceGreater30
{
public:
	bool operator()(int val)
	{
		return val >= 30;
	}
};

void test01()
{
	vector<int> v;
	v.push_back(20);
	v.push_back(30);
	v.push_back(20);
	v.push_back(40);
	v.push_back(50);
	v.push_back(10);
	v.push_back(20);

	//将容器中大于等于的30 替换成 3000
	replace_if(v.begin(), v.end(), ReplaceGreater30(), 3000);
	for_each(v.begin(), v.end(), myPrint());
	cout << endl;
}
```

### swap
 - **功能**：互换两个容器的元素
 - ==`swap(container c1, container c2);`==
 - **注意**：swap 交换容器时，注意交换的容器要同种类型

示例：
```C++
#include <algorithm>
#include <vector>

void test01()
{
	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10; i++) {
		v1.push_back(i);
		v2.push_back(i + 100);
	}

	cout << "交换前： " << endl;
	for_each(v1.begin(), v1.end(), myPrint());
	cout << endl;
	for_each(v2.begin(), v2.end(), myPrint());
	cout << endl;

	cout << "交换后： " << endl;
	swap(v1, v2);
	for_each(v1.begin(), v1.end(), myPrint());
	cout << endl;
	for_each(v2.begin(), v2.end(), myPrint());
	cout << endl;
}
```

## 常用算术生成算法
 - **注意**：算术生成算法属于小型算法，使用时包含的头文件为 ==`#include <numeric>`==
 - ==`accumulate`==    //计算容器元素累计总和
 - ==`fill`==                //向容器中添加元素

### accumulate
 - **功能**：计算区间内容器元素累计总和
 - ==`accumulate(iterator beg, iterator end, value);`==
	 - value：起始值
 - **注意**：头文件是 numeric，这个算法很实用

示例：
```C++
#include <numeric>
#include <vector>

void test01()
{
	vector<int> v;
	for (int i = 0; i <= 100; i++) {
		v.push_back(i);
	}

	int total = accumulate(v.begin(), v.end(), 0);

	cout << "total = " << total << endl;   //5050
}
```

### fill
 - **功能**：向容器中填充指定的元素
 - ==`fill(iterator beg, iterator end, value);`==
	 - value：填充的值
 - **注意**：利用 fill 可以将容器区间内元素全部填充为指定的值

示例：
```C++
#include <numeric>
#include <vector>
#include <algorithm>

class myPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{
	vector<int> v;
	v.resize(10);
	//填充
	fill(v.begin(), v.end(), 100);

	for_each(v.begin(), v.end(), myPrint());
	cout << endl;
}
```

## 常用集合算法
 - ==`set_intersection`==    //求两个容器的交集
 - ==`set_union`==                //求两个容器的并集
 - ==`set_difference`==        //求两个容器的差集
 - **共同注意**：求集合的两个容器必须是**有序序列**，目标容器需要提前开辟空间

### set_intersection
 - **功能**：求两个集合的交集
 - ==`set_intersection(iterator beg1, iterator end1, iterator beg2, iterator end2, iterator dest);`==
 - **注意**：
	1. 求交集的两个集合必须是有序序列
	2. 目标容器开辟空间需要从**两个容器中取小值** ==`min(v1.size(), v2.size())`==
	3. 返回值既是交集中最后一个元素的位置，遍历时用返回值作为结束迭代器

示例：
```C++
#include <vector>
#include <algorithm>

void test01()
{
	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10; i++)
	{
		v1.push_back(i);
		v2.push_back(i + 5);
	}

	vector<int> vTarget;
	//取两个里面较小的值给目标容器开辟空间
	vTarget.resize(min(v1.size(), v2.size()));

	//返回目标容器的最后一个元素的迭代器地址
	vector<int>::iterator itEnd =
		set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), vTarget.begin());

	for_each(vTarget.begin(), itEnd, myPrint());
	cout << endl;
}
```

### set_union
 - **功能**：求两个集合的并集
 - ==`set_union(iterator beg1, iterator end1, iterator beg2, iterator end2, iterator dest);`==
 - **注意**：
	1. 求并集的两个集合必须是有序序列
	2. 目标容器开辟空间需要**两个容器相加** ==`v1.size() + v2.size()`==
	3. 返回值既是并集中最后一个元素的位置

示例：
```C++
#include <vector>
#include <algorithm>

void test01()
{
	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10; i++) {
		v1.push_back(i);
		v2.push_back(i + 5);
	}

	vector<int> vTarget;
	//取两个容器的和给目标容器开辟空间
	vTarget.resize(v1.size() + v2.size());

	//返回目标容器的最后一个元素的迭代器地址
	vector<int>::iterator itEnd =
		set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), vTarget.begin());

	for_each(vTarget.begin(), itEnd, myPrint());
	cout << endl;
}
```

### set_difference
 - **功能**：求两个集合的差集
 - ==`set_difference(iterator beg1, iterator end1, iterator beg2, iterator end2, iterator dest);`==
 - **注意**：
	1. 求差集的两个集合必须是有序序列
	2. 目标容器开辟空间需要从**两个容器取较大值** ==`max(v1.size(), v2.size())`==
	3. 返回值既是差集中最后一个元素的位置
	4. v1 与 v2 的差集 和 v2 与 v1 的差集 结果不同

示例：
```C++
#include <vector>
#include <algorithm>

void test01()
{
	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10; i++) {
		v1.push_back(i);
		v2.push_back(i + 5);
	}

	vector<int> vTarget;
	//取两个里面较大的值给目标容器开辟空间
	vTarget.resize(max(v1.size(), v2.size()));

	//返回目标容器的最后一个元素的迭代器地址
	cout << "v1与v2的差集为： " << endl;
	vector<int>::iterator itEnd =
		set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), vTarget.begin());
	for_each(vTarget.begin(), itEnd, myPrint());
	cout << endl;

	cout << "v2与v1的差集为： " << endl;
	itEnd = set_difference(v2.begin(), v2.end(), v1.begin(), v1.end(), vTarget.begin());
	for_each(vTarget.begin(), itEnd, myPrint());
	cout << endl;
}
```
