已经是一个完善的类了
## 用法
```cpp
//创建
string s = "ddf";
string b(const char*str);
string (const string &s);

//函数
s.length();//获取长度
s1 =s2; //赋值
s1+ s2  s1+=s2;//拼接
s1 =s2;//比较
s.find(s2) //查找

//访问
s[0];
s.at[0];
```

```cpp
string s = "Hello";

s.size();        // 5，长度
s.length();      // 5，同 size()
s.empty();       // false，是否为空
s[0];            // 'H'，不检查越界
s.at(1);         // 'e'，会检查越界抛异常
s.front();       // 'H'
s.back();        // 'o'
s.c_str();       // 返回 const char*
s.data();        // 返回底层字符数组
```

```cpp
string s = "Hello";

s += " World";           // 追加 -> "Hello World"
s.append("!");           // 追加
s.push_back('?');        // 追加单字符
s.pop_back();            // 删除末尾字符
s.insert(5, ",");        // 在位置5插入
s.erase(5, 1);           // 从位置5删除1个字符
s.replace(0, 5, "Hi");   // 替换 [0,5) 为 "Hi"
s.clear();               // 清空
```
```cpp
string s = "Hello World";

s.find("World");         // 6，找不到返回 string::npos
s.rfind("l");            // 从右往左找 -> 9
s.find_first_of("aeiou"); // 找第一个元音
s.find_last_of("aeiou");  // 找最后一个元音
s.find("xyz");           // string::npos
```

## 数据流
```cpp
// 拼接
#include <sstream>
stringstream ss;
ss << "x=" << 10 << ", y=" << 20;
string result = ss.str();     // "x=10, y=20"
// 分割（按空格）
string line = "10 20 30";
istringstream iss(line);
int a, b, c;
iss >> a >> b >> c;           // a=10, b=20, c=30
```
## 按分隔符分割
```cpp
#include <sstream>
#include <vector>

vector<string> split(const string& s, char delim) {
    vector<string> res;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        res.push_back(item);
    }
    return res;
}

// split("a,b,c", ',') -> {"a","b","c"}
```
## 大小写
```cpp
#include <algorithm>
#include <cctype>

string s = "Hello";

transform(s.begin(), s.end(), s.begin(), ::tolower);  // "hello"
transform(s.begin(), s.end(), s.begin(), ::toupper);  // "HELLO"

c = static_cast<char>(toupper(static_cast<unsigned char>(c)))
```


```cpp
c = c - 'a' + 'A'; 直接移位
```

遍历字符串
```cpp
for(char &c : str){

}
```
## 结尾加n个a
```cpp
s.append(n, 'a');
```


## 按空白分割
```cpp
vector<string> split(const string& s) {
    vector<string> res;
    istringstream iss(s);
    string token;
    while (iss >> token) {        // >> 自动按空白分割
        res.push_back(token);
    }
    return res;
}
```