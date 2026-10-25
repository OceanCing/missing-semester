## 引用的基本使用
 - **作用**：给变量起别名
 - **语法**：==数据类型 &别名 = 原名==
 - **注意**：引用必须初始化，且不可更改引用对象
示例代码：
```C++
int main(){
	int a=10;
	int b=20;
	//int &c;//没有初始化，报错
	int &c=a;
	//c=b;//赋值操作，不是更改引用
	
	cout<<"a= "<<a<<endl;
	cout<<"b= "<<b<<endl;
	cout<<"c= "<<c<<endl;
	
	c=20;
	
	cout<<"a= "<<a<<endl;
	cout<<"b= "<<b<<endl;
	cout<<"c= "<<c<<endl;
	
	system("pause");
	
	return 0;
}
```

## 引用做函数参数
 - **作用**：函数传参时，让形参修饰实参
 - **优点**：简化指针修改实参
示例代码：
```C++
//1. 值传递,操作形参的值，实参的值不变
void f1(int a,int b){
	int temp = a;
	a=b;
	b=a;
}

//2. 地址传递,操作地址指向内存，实参的值改变
void f2(int* a,int* b){
	int temp = *a;
	*a = *b;
	*b = *a;
}

//3. 引用传递,实参的值改变
void f1(int& a,int& b){
	int temp = a;
	a = b;
	b = a;
}
```

## 引用做函数返回值
 - **作用**：引用可以作为函数的返回值存在
 - **注意**：不要返回局部变量引用（原因：函数结束后引用指向的地址被释放）
 - **用法**：函数调用作为左值
示例代码：
```C++
//返回局部变量引用
int& test1() {
	int a = 10; //局部变量
	return a;
}

//返回静态变量引用
int& test2() {
	static int a = 20;
	return a;
}

int main() {

	//不能返回局部变量的引用
	int& ref = test1();
	cout << "ref = " << ref << endl;
	cout << "ref = " << ref << endl;

	//如果函数做左值，那么必须返回引用
	int& ref2 = test2();
	cout << "ref2 = " << ref2 << endl;
	cout << "ref2 = " << ref2 << endl;

	test02() = 1000;

	cout << "ref2 = " << ref2 << endl;
	cout << "ref2 = " << ref2 << endl;

	system("pause");

	return 0;
}
```

## 引用的本质
 - **本质**：指针常量
 示例代码：
```C++
//发现是引用，转换为 int* const ref = &a;
void func(int& ref){
	ref = 100; // ref是引用，转换为*ref = 100
}
int main(){
	int a = 10;
    
    //自动转换为 int* const ref = &a; 指针常量是指针指向不可改，也说明为什么引用不可更改
	int& ref = a; 
	ref = 20; //内部发现ref是引用，自动帮我们转换为: *ref = 20;
    
	cout << "a:" << a << endl;
	cout << "ref:" << ref << endl;
    
	func(a);
	
	system("pause");
	
	return 0;
}
```

## 常量引用
 - **作用**：常量引用主要用来修饰形参，防止误操作，在函数形参列表中，可以加const修饰形参，防止形参改变实参
 示例代码：
```C++
 //引用使用的场景，通常用来修饰形参
void showValue(const int& v) {
	//v += 10;
	cout << v << endl;
}

int main() {

	//int& ref = 10;  引用本身需要一个合法的内存空间，因此这行错误
	//加入const就可以了，编译器优化代码，int temp = 10; const int& ref = temp;
	const int& ref = 10;

	//ref = 100;  //加入const后不可以修改变量
	cout << ref << endl;

	//函数中利用常量引用防止误操作修改实参
	int a = 10;
	showValue(a);

	system("pause");

	return 0;
}
```