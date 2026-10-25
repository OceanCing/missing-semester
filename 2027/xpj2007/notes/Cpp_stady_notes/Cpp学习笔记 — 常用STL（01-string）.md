# string
 - **本质**：string是C++风格的字符串，而string本质上是一个类
### 构造函数
 - ==`string();`==                                                      //创建一个空的字符串
 - ==`string(const char* s);`==                               //使用字符串s初始化
 - ==`string(const string& str);`==                        //使用一个string对象初始化另一个string对象
 - ==`string(int n，char c);`==                               //使用n个字符c初化
### 赋值操作
 - ==`string& operator=(const char* s);`==            //char*类型字符串赋值给当前的字符串
 - ==`string& operator=(const string&s);`==          //把字符串s赋给当前的字符串
 - ==`string& operator=(char c);`==                        //字符赋值给当前的字符串
 - ==`string& assign(const char* s);`==                 //把字符串s赋给当前的字符串
 - ==`string& assign(const char* s,int n);`==      //把字符串s的前n个字符赋给当前的字符串
 - ==`string& assign(const string&s);`==               //把字符串s赋给当前字符串
 - ==`string& assign(int n，char c);`==                 //用n个字符c赋给当前字符串
### 字符串拼接
 - ==`string& operator+=(const char* str);`==       //重载+=操作符
 - ==`string& operator+=(const char c);`==            //重载+=操作符
 - ==`string& operator+=(const string& str);`==   //重载+=操作符
 - ==`string& append(const char* s);`==                  //把字符串s连接到当前字符串结尾
 - ==`string& append(const char* s,int n);`==      //把字符串s的前n个字符连接到当前字符串结尾
 - ==`string& append(const string &s);`==             //同operator+=(const string& str)
 - //字符串s中从pos开始的n个字符连接到字符串结尾
 - ==`string& append(const string &s, int pos, int n);`==  
### string查找和替换
 - //查找str第一次出现位置,从pos开始查找
   ==`int find(const string& str,int pos = 0) const;`== 
 - //查找s第一次出现位置,从pos开始查找
   ==`int find(const char* s,int pos=0) const;`==
 - //从pos位置查找s的前n个字符第一次位置
   ==`int find(const char* s,int pos,int n) const;`==
 - //查找字符c第一次出现位置
   ==`int find(const char c，int pos = 0)const;`==
 - //查找str最后一次位置,从pos开始查找
   ==`int rfind(const string& str,int pos = npos) const;`==
 - //查找s最后一次位置,从pos开始查找
   ==`int rfind(const char* s,int pos = npos) const;`==
 - //从pos查找s的前n个字符最后一次位置
   ==`int rfind(const char* s, int pos, int n)const;`==
 - //查找字符c最后一次出现位置
   ==`int rfind(const char c, int pos = 0)const;`==
 - //替换从pos开始n个字符为字符串str
   ==`string& replace(int pos, int n, const string& str);`==
 - //替换从pos开始的n个字符为字符串s
   ==`string& replace(int pos, int n,const char* s);`==

### 字符串比较
 - 字符串比较是按字符的ASCIl码进行对比：
	 1. =返回0
	 2. >返回1
	 3. <返回-1

 - ==`int compare(const string &s) const;`==        //与字符串s比较
 - ==`int compare(const char *s) const;`==           //与字符串s比较
### 字符串存取
 - ==`char& operator[](int n);`==                          //通过[ ]方式取字符
 - ==`char& at(int n);`==                                        //通过at方法获取字符
### 字符串插入和删除
 - ==`string& insert(int pos,const char* s);`==   //插入字符串
 - ==`string& insert(int pos,const string& str);`== //插入字符串
 - ==`string& insert(int pos,int n,char c);`==     //在指定位置插入n个字符c
 - ==`string&erase(int pos,int n = npos);`==        //删除从Pos开始的n个字符
### 字符串子串
 -  //返回由pos开始的n个字符组成的字符串
    ==`string substr(int pos = 0, int n = npos)const;`== 


