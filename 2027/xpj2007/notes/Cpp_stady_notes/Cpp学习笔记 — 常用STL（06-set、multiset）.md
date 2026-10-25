# set/multiset
 - **概念**：所有元素都会在插入时自动被排序，属于关联式容器
 - **本质**：底层结构是用二叉树实现
 - **区别**：
	1. set 不允许容器中有重复的元素
	2. multiset 允许容器中有重复的元素
 - **注意**：set 插入数据只能用 ==`insert`==，插入后自动排序；存放自定义数据类型时必须指定排序规则
### set构造和赋值
 - ==`set<T> st;`==                                    //默认构造函数
 - ==`set(const set& st);`==                        //拷贝构造函数
 - ==`set& operator=(const set& st);`==      //重载等号操作符
### set大小和交换
 - ==`size();`==                                              //返回容器中元素的数目
 - ==`empty();`==                                            //判断容器是否为空
 - ==`swap(st);`==                                           //交换两个集合容器
### set插入和删除
 - ==`insert(elem);`==                                   //在容器中插入元素
 - ==`clear();`==                                              //清除所有元素
 - ==`erase(pos);`==                                        //删除pos迭代器所指的元素，返回下一个元素的迭代器
 - ==`erase(beg, end);`==                                //删除区间\[beg,end)的所有元素，返回下一个元素的迭代器
 - ==`erase(elem);`==                                      //删除容器中值为elem的元素
### set查找和统计
 - ==`find(key);`==                                          //查找key是否存在，存在返回该键的元素的迭代器，不存在返回set.end()
 - ==`count(key);`==                                        //统计key的元素个数（set的结果为0或1）
### pair对组创建
 - **功能**：成对出现的数据，利用对组可以返回两个数据，map 中的元素就是 pair
 - ==`pair<type, type> p (value1, value2);`==
 - ==`pair<type, type> p = make_pair(value1, value2);`==
 - **取值**：==`p.first`== 取第一个值，==`p.second`== 取第二个值

示例：构造、赋值与遍历
```C++
#include <set>

void printSet(set<int> & s)
{
	for (set<int>::iterator it = s.begin(); it != s.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

void test01()
{
	set<int> s1;          //默认构造
	s1.insert(10);
	s1.insert(30);
	s1.insert(20);
	s1.insert(40);
	printSet(s1);         //输出 10 20 30 40，自动排序

	set<int> s2(s1);      //拷贝构造
	printSet(s2);

	set<int> s3;
	s3 = s2;              //赋值
	printSet(s3);
}

int main() {

	test01();

	system("pause");

	return 0;
}
```

示例：查找和统计
```C++
#include <set>

void test01()
{
	set<int> s1;
	s1.insert(10);
	s1.insert(30);
	s1.insert(20);
	s1.insert(40);

	//查找，返回的是迭代器
	set<int>::iterator pos = s1.find(30);
	if (pos != s1.end())
	{
		cout << "找到了元素： " << *pos << endl;
	}
	else
	{
		cout << "未找到元素" << endl;
	}

	//统计，对于set结果为0或者1
	int num = s1.count(30);
	cout << "num = " << num << endl;
}
```

### set和multiset区别
 - set 插入数据时会返回插入结果 ==`pair<set<T>::iterator, bool>`==，==`second`== 表示插入是否成功
 - multiset 不会检测数据，因此可以插入重复数据
 - 不允许插入重复数据用 set，需要插入重复数据用 multiset

示例：区分 set 与 multiset
```C++
#include <set>

void test01()
{
	set<int> s;
	pair<set<int>::iterator, bool> ret = s.insert(10);
	if (ret.second) {
		cout << "第一次插入成功!" << endl;
	}
	else {
		cout << "第一次插入失败!" << endl;
	}

	ret = s.insert(10);   //重复插入
	if (ret.second) {
		cout << "第二次插入成功!" << endl;
	}
	else {
		cout << "第二次插入失败!" << endl;   //set 插入失败
	}

	//multiset 允许重复
	multiset<int> ms;
	ms.insert(10);
	ms.insert(10);

	for (multiset<int>::iterator it = ms.begin(); it != ms.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}
```

### set容器排序
 - **技术点**：利用仿函数，可以改变排序规则，默认排序规则为从小到大
 - **内置数据类型**：==`set<int, MyCompare> s;`==
 - **自定义数据类型**：set 必须指定排序规则才可以插入数据

示例一：set 存放内置数据类型，改为从大到小
```C++
#include <set>

class MyCompare
{
public:
	bool operator()(int v1, int v2) {
		return v1 > v2;
	}
};

void test01()
{
	set<int> s1;
	s1.insert(10);
	s1.insert(40);
	s1.insert(20);
	s1.insert(30);
	s1.insert(50);

	//默认从小到大
	for (set<int>::iterator it = s1.begin(); it != s1.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;

	//指定排序规则
	set<int, MyCompare> s2;
	s2.insert(10);
	s2.insert(40);
	s2.insert(20);
	s2.insert(30);
	s2.insert(50);

	for (set<int, MyCompare>::iterator it = s2.begin(); it != s2.end(); it++) {
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

示例二：set 存放自定义数据类型
```C++
#include <set>
#include <string>

class Person
{
public:
	Person(string name, int age)
	{
		this->m_Name = name;
		this->m_Age = age;
	}

	string m_Name;
	int m_Age;
};

class comparePerson
{
public:
	bool operator()(const Person& p1, const Person& p2)
	{
		//按照年龄进行排序  降序
		return p1.m_Age > p2.m_Age;
	}
};

void test01()
{
	set<Person, comparePerson> s;

	Person p1("刘备", 23);
	Person p2("关羽", 27);
	Person p3("张飞", 25);
	Person p4("赵云", 21);

	s.insert(p1);
	s.insert(p2);
	s.insert(p3);
	s.insert(p4);

	for (set<Person, comparePerson>::iterator it = s.begin(); it != s.end(); it++)
	{
		cout << "姓名： " << it->m_Name << " 年龄： " << it->m_Age << endl;
	}
}

int main() {

	test01();

	system("pause");

	return 0;
}
```
