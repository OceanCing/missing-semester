# map/multimap
 - **概念**：map 中所有元素都是 pair，pair 中第一个元素为 key（键值）起索引作用，第二个元素为 value（实值）
 - **特点**：所有元素都会根据元素的键值自动排序
 - **本质**：map/multimap 属于关联式容器，底层结构是用二叉树实现
 - **优点**：可以根据 key 值快速找到 value 值
 - **区别**：
	1. map 不允许容器中有重复 key 值元素
	2. multimap 允许容器中有重复 key 值元素
 - **注意**：插入数据时要使用对组 pair，map 中所有元素都是成对出现的
### map构造和赋值
 - ==`map<T1, T2> mp;`==                                //map默认构造函数
 - ==`map(const map& mp);`==                          //拷贝构造函数
 - ==`map& operator=(const map& mp);`==      //重载等号操作符
### map大小和交换
 - ==`size();`==                                              //返回容器中元素的数目
 - ==`empty();`==                                            //判断容器是否为空
 - ==`swap(st);`==                                           //交换两个集合容器
### map插入和删除
 - ==`insert(elem);`==                                   //在容器中插入元素
 - ==`clear();`==                                              //清除所有元素
 - ==`erase(pos);`==                                        //删除pos迭代器所指的元素，返回下一个元素的迭代器
 - ==`erase(beg, end);`==                                //删除区间\[beg,end)的所有元素，返回下一个元素的迭代器
 - ==`erase(key);`==                                        //删除容器中键值为key的元素
 - **四种插入方式**（记住其一即可）：
	 1. ==`m.insert(pair<int, int>(1, 10));`==
	 2. ==`m.insert(make_pair(2, 20));`==
	 3. ==`m.insert(map<int, int>::value_type(3, 30));`==
	 4. ==`m[4] = 40;`== —— 不建议用来插入，但可以利用 key 访问到 value
### map查找和统计
 - ==`find(key);`==                                          //查找key是否存在，存在返回该键的元素的迭代器，不存在返回map.end()
 - ==`count(key);`==                                        //统计key的元素个数（map的结果为0或1，multimap可以大于1）

示例：构造、赋值、插入与删除
```C++
#include <map>

void printMap(map<int, int>& m)
{
	for (map<int, int>::iterator it = m.begin(); it != m.end(); it++)
	{
		cout << "key = " << it->first << " value = " << it->second << endl;
	}
	cout << endl;
}

void test01()
{
	//插入
	map<int, int> m;
	m.insert(pair<int, int>(1, 10));            //第一种插入方式
	m.insert(make_pair(2, 20));                 //第二种插入方式
	m.insert(map<int, int>::value_type(3, 30)); //第三种插入方式
	m[4] = 40;                                  //第四种插入方式
	printMap(m);

	//删除
	m.erase(m.begin());
	printMap(m);

	m.erase(3);                                 //按照key删除
	printMap(m);

	//清空
	m.erase(m.begin(), m.end());
	m.clear();
	printMap(m);
}

int main() {

	test01();

	system("pause");

	return 0;
}
```

示例：查找和统计
```C++
#include <map>

void test01()
{
	map<int, int> m;
	m.insert(pair<int, int>(1, 10));
	m.insert(pair<int, int>(2, 20));
	m.insert(pair<int, int>(3, 30));

	//查找，返回的是迭代器
	map<int, int>::iterator pos = m.find(3);
	if (pos != m.end())
	{
		cout << "找到了元素 key = " << (*pos).first << " value = " << (*pos).second << endl;
	}
	else
	{
		cout << "未找到元素" << endl;
	}

	//统计，对于map结果为0或者1
	int num = m.count(3);
	cout << "num = " << num << endl;
}
```

### map容器排序
 - **技术点**：利用仿函数，可以改变排序规则，默认按照 key 值从小到大排序
 - **注意**：对于自定义数据类型，map 必须要指定排序规则，同 set 容器

示例：利用仿函数实现从大到小排序
```C++
#include <map>

class MyCompare {
public:
	bool operator()(int v1, int v2) {
		return v1 > v2;
	}
};

void test01()
{
	//默认从小到大排序
	//利用仿函数实现从大到小排序
	map<int, int, MyCompare> m;

	m.insert(make_pair(1, 10));
	m.insert(make_pair(2, 20));
	m.insert(make_pair(3, 30));
	m.insert(make_pair(4, 40));
	m.insert(make_pair(5, 50));

	for (map<int, int, MyCompare>::iterator it = m.begin(); it != m.end(); it++) {
		cout << "key:" << it->first << " value:" << it->second << endl;
	}
}

int main() {

	test01();

	system("pause");

	return 0;
}
```

### 案例-员工分组
 - **案例描述**：
	1. 公司今天招聘了10个员工（ABCDEFGHIJ），需要指派员工在哪个部门工作
	2. 员工信息有：姓名、工资；部门分为：策划、美术、研发
	3. 随机给10名员工分配部门和工资
	4. 通过 multimap 进行信息的插入，key(部门编号) value(员工)
	5. 分部门显示员工信息
 - **实现步骤**：
	1. 创建10名员工，放到 vector 中
	2. 遍历 vector 容器，取出每个员工，进行随机分组
	3. 分组后，将员工部门编号作为 key，具体员工作为 value，放入到 multimap 容器中
	4. 分部门显示员工信息
 - **总结**：当数据以键值对形式存在，可以考虑用 map 或 multimap

```C++
#include<iostream>
using namespace std;
#include <vector>
#include <string>
#include <map>
#include <ctime>

#define CEHUA  0
#define MEISHU 1
#define YANFA  2

class Worker
{
public:
	string m_Name;
	int m_Salary;
};

void createWorker(vector<Worker>& v)
{
	string nameSeed = "ABCDEFGHIJ";
	for (int i = 0; i < 10; i++)
	{
		Worker worker;
		worker.m_Name = "员工";
		worker.m_Name += nameSeed[i];

		worker.m_Salary = rand() % 10000 + 10000; // 10000 ~ 19999
		//将员工放入到容器中
		v.push_back(worker);
	}
}

//员工分组
void setGroup(vector<Worker>& v, multimap<int, Worker>& m)
{
	for (vector<Worker>::iterator it = v.begin(); it != v.end(); it++)
	{
		//产生随机部门编号
		int deptId = rand() % 3; // 0 1 2

		//将员工插入到分组中
		//key部门编号，value具体员工
		m.insert(make_pair(deptId, *it));
	}
}

void showWorkerByGourp(multimap<int, Worker>& m)
{
	// 0  A  B  C   1  D  E   2  F G ...
	cout << "策划部门：" << endl;

	multimap<int, Worker>::iterator pos = m.find(CEHUA);
	int count = m.count(CEHUA); // 统计具体人数
	int index = 0;
	for (; pos != m.end() && index < count; pos++, index++)
	{
		cout << "姓名： " << pos->second.m_Name << " 工资： " << pos->second.m_Salary << endl;
	}

	cout << "----------------------" << endl;
	cout << "美术部门： " << endl;
	pos = m.find(MEISHU);
	count = m.count(MEISHU); // 统计具体人数
	index = 0;
	for (; pos != m.end() && index < count; pos++, index++)
	{
		cout << "姓名： " << pos->second.m_Name << " 工资： " << pos->second.m_Salary << endl;
	}

	cout << "----------------------" << endl;
	cout << "研发部门： " << endl;
	pos = m.find(YANFA);
	count = m.count(YANFA); // 统计具体人数
	index = 0;
	for (; pos != m.end() && index < count; pos++, index++)
	{
		cout << "姓名： " << pos->second.m_Name << " 工资： " << pos->second.m_Salary << endl;
	}
}

int main() {

	srand((unsigned int)time(NULL));

	//1、创建员工
	vector<Worker> vWorker;
	createWorker(vWorker);

	//2、员工分组
	multimap<int, Worker> mWorker;
	setGroup(vWorker, mWorker);

	//3、分组显示员工
	showWorkerByGourp(mWorker);

	system("pause");

	return 0;
}
```
