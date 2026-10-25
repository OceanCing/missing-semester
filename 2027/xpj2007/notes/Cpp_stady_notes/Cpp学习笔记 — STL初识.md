### STL六大组件
1. **容器**：各种数据结构，如vector、list、deque、set、map等,用来存放数据
2. **算法**：各种常用的算法，如sort、find、copy、for_each等
3. **迭代器**：扮演了容器与算法之间的胶合剂，一般用于访问容器元素
4. **仿函数**：行为类似函数，可作为算法的某种策略
5. **适配器**：一种用来修饰容器或者仿函数或迭代器接口的东西
6. **空间配置器**：负责空间的配置与管理
 
 - **容器分类**：
	1. **序列式容器**：强调值的排序，序列式容器中的每个元素均有固定的位置
	2. **关联式容器**：二叉树结构，各元素之间没有严格的物理上的顺序关系
 - **算法分类**：
	 1. **质变算法**：运算过程中会更改区间内的元素的内容。例如拷贝，替换，删除等等
	 2. **非质变算法**：是指运算过程中不会更改区间内的元素内容，例如查找、计数、遍历、寻找极值等等
 - **迭代器**：提供一种方法，使之能够依序寻访某个容器所含的各个元素，而又无需暴露该容器的内部表示方式

| 种类          | 功能                           | 支持运算                      |
| ----------- | ---------------------------- | ------------------------- |
| 输入迭代<br>器   | 对数据的只读访问                     | 只读，支持++、==、!=             |
| 输出迭代<br>器   | 对数据的只写访问                     | 只写，支持++                   |
| 前向迭代<br>器   | 读写操作，并能向前推进迭代器               | 读写，支持++、==、!=             |
| 双向迭代<br>器   | 读写操作，并能向前和向后操作               | 读写，支持++、--                |
| 随机访问<br>迭代器 | 读写操作，可以以跳跃的方式访问任意数据，功能最强的迭代器 | 读写，支持++、一、[n]、n、<、<=、>、>= |

使用示例一：
```C++
#include <vector>
#include <algorithm>

void MyPrint(int val)
{
	cout << val << endl;
}

void test01() {

	//创建vector容器对象，并且通过模板参数指定容器中存放的数据的类型
	vector<int> v;
	//向容器中放数据
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);

	//每一个容器都有自己的迭代器，迭代器是用来遍历容器中的元素
	//v.begin()返回迭代器，这个迭代器指向容器中第一个数据
	//v.end()返回迭代器，这个迭代器指向容器元素的最后一个元素的下一个位置
	//vector<int>::iterator 拿到vector<int>这种容器的迭代器类型

	vector<int>::iterator pBegin = v.begin();
	vector<int>::iterator pEnd = v.end();

	//第一种遍历方式：
	while (pBegin != pEnd) {
		cout << *pBegin << endl;
		pBegin++;
	}

	
	//第二种遍历方式：
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
		cout << *it << endl;
	}
	cout << endl;

	//第三种遍历方式：
	//使用STL提供标准遍历算法  头文件 algorithm
	for_each(v.begin(), v.end(), MyPrint);
}

int main() {

	test01();

	system("pause");

	return 0;
}
```

使用示例二：
```C++
#include <vector>

//容器嵌套容器
void test01() {

	vector< vector<int> >  v;

	vector<int> v1;
	vector<int> v2;
	vector<int> v3;
	vector<int> v4;

	for (int i = 0; i < 4; i++) {
		v1.push_back(i + 1);
		v2.push_back(i + 2);
		v3.push_back(i + 3);
		v4.push_back(i + 4);
	}

	//将容器元素插入到vector v中
	v.push_back(v1);
	v.push_back(v2);
	v.push_back(v3);
	v.push_back(v4);


	for (vector<vector<int>>::iterator it = v.begin(); it != v.end(); it++) {

		for (vector<int>::iterator vit = (*it).begin(); vit != (*it).end(); vit++) {
			cout << *vit << " ";
		}
		cout << endl;
	}

}

int main() {

	test01();

	system("pause");

	return 0;
}
```