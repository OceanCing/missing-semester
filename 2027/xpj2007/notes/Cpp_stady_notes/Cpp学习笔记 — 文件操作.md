  - 程序可以通过文件将数据持久化
  - 包含头文件 < fstream >
  - **文件类型**：
	  - 文本文件
	  - 二进制文件
 -  **操作文件的类**：
	1. ofstream  写操作
	2. ifstream  读操作
	3. fstream    读写操作

| 打开方式        | 作用           |
| ----------- | ------------ |
| ios::in     | 为写文件而打开文件    |
| ios::out    | 为写文件而打开文件    |
| ios::ate    | 初始位置再文件末尾    |
| ios::app    | 追加方式写文件      |
| ios::trunc  | 如果文件存在先删除再创建 |
| ios::binary | 以二进制方式打开     |
**注意**：文件打开方式可以使用 | 操作符配合使用

### 文本文件
  - **写文件步骤**
	1. 头文件：        `#include <fstream>`
	2. 创建流对象     `ofstream ofs;`
	3. 打开文件        `ofs.open("文件路径" , 打开方式);`
	4. 写数据           `ofs<<"写入的数据;"`
	5. 关闭文件        `ofs.close();`

  - **读文件文件步骤**
	1. 头文件：        `#include <fstream>`
	2. 创建流对象     `ifstream ifs;`
	3. 打开文件        `ifs.open("文件路径" , 打开方式);`
	4. 判断是否成功 `ifs.is_open();`
	5. 读数据           `ifs>>buf;` 或 `ifs.getline(buf,sizeof(buf));` 或 `getline(ifs,buf);` 或 `char c =ifs.get();`
	6. 关闭文件        `ofs.close();`

示例一：写文件
```C++
#include <fstream>

void test01()
{
	ofstream ofs;
	ofs.open("test.txt", ios::out);

	ofs << "姓名：张三" << endl;
	ofs << "性别：男" << endl;
	ofs << "年龄：18" << endl;

	ofs.close();
}

int main() {

	test01();

	system("pause");

	return 0;
}
```

示例二：读文件
```C++
#include <fstream>
#include <string>
void test01()
{
	ifstream ifs;
	ifs.open("test.txt", ios::in);

	if (!ifs.is_open())
	{
		cout << "文件打开失败" << endl;
		return;
	}

	//第一种方式
	//char buf[1024] = { 0 };
	//while (ifs >> buf)
	//{
	//	cout << buf << endl;
	//}

	//第二种
	//char buf[1024] = { 0 };
	//while (ifs.getline(buf,sizeof(buf)))
	//{
	//	cout << buf << endl;
	//}

	//第三种
	//string buf;
	//while (getline(ifs, buf))
	//{
	//	cout << buf << endl;
	//}

	char c;
	while ((c = ifs.get()) != EOF)
	{
		cout << c;
	}

	ifs.close();


}

int main() {

	test01();

	system("pause");

	return 0;
}
```

### 二进制文件

与文本文件的大致步骤相同，打开方式必须要有`ios::binary`，常用读写数据函数略有不同。

 - **写文件**
	 -  主要利用流对象调用成员函数write
	 - **函数原型**：==ostream& write(const char * buffer,int len);==
	 - **参数**：字符指针buffer指向内存中一段存储空间。len是读写的字节数

 - **读文件**
	 -  主要利用流对象调用成员函数read
	 - **函数原型**：==istream& read(char * buffer,int len);==
	 - **参数**：字符指针buffer指向内存中一段存储空间。len是读写的字节数

示例三：二进制文件写文件
```C++
#include <fstream>
#include <string>

class Person
{
public:
	char m_Name[64];
	int m_Age;
};

//二进制文件  写文件
void test01()
{
	//1、包含头文件

	//2、创建输出流对象
	ofstream ofs("person.txt", ios::out | ios::binary);
	
	//3、打开文件
	//ofs.open("person.txt", ios::out | ios::binary);

	Person p = {"张三"  , 18};

	//4、写文件
	ofs.write((const char *)&p, sizeof(p));

	//5、关闭文件
	ofs.close();
}

int main() {

	test01();

	system("pause");

	return 0;
}
```

示例四：二进制文件读文件
```C++
#include <fstream>
#include <string>

class Person
{
public:
	char m_Name[64];
	int m_Age;
};

void test01()
{
	ifstream ifs("person.txt", ios::in | ios::binary);
	if (!ifs.is_open())
	{
		cout << "文件打开失败" << endl;
	}

	Person p;
	ifs.read((char *)&p, sizeof(p));

	cout << "姓名： " << p.m_Name << " 年龄： " << p.m_Age << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```
