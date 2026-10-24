存数据的类，方便扩建，自动处理内存
### vector 不适合的场景

- 频繁头部插入
- 频繁中间删除
- 超大对象频繁扩容
## 用法
```cpp
#include"vector"
fum{
	vector<int> vec(个数，初始值)；    //<>是多态的内容
	vec.push_back(2);              //尾部加元素
	vec.erase(vec.begin())              //删除 vector 删除中间元素时，后面的元素会整体前移 o(n)
	vec.pop_back();//尾部删除
	vec.size()                     //获取元素个数
	
	vec[3]                          //重载了[]可以像数组一样访问
}
```
为了避免频繁扩容，可以提前分配内存：
```
std::vector<int>vec;

vec.reserve(1000000);
```


```cpp
for (auto it = vec.begin(); it != vec.end(); ++it) {
    std::cout << *it << " ";  // 统一方式，适用于所有容器
}
```
此时 `it` 是迭代器，要求它的下标
```
auto it = v.begin() + n;
int index = it - v.begin();   // 得到 n
```
## 另一个遍历
```cpp
for (int a : V)
```

### push_back 和 emplace_back

现代 C++ 更推荐使用：

emplace_back()

例如：
```
vec.push_back(Person("Tom", 20));
```

可能产生临时对象。

而：

```
vec.emplace_back("Tom", 20);
```

会直接在 vector 内部原地构造对象。

通常效率更高。

### 迭代器失效

vector 扩容后，原来的内存地址可能失效。

例如：

```
std::vector<int>vec = {1, 2, 3};

auto it = vec.begin();

vec.push_back(4);

std::cout << *it;
```
这里的 `it` 可能已经失效。

因为 push_back() 可能导致重新分配内存。

这是 vector 最常见的坑之一。
```
//可以直接
it = line.erase(it);  // 删除：erase 返回下一个有效迭代器

//如果在for中写了++it,可能导致跳过一个
```
## 批量去除
```
#include <algorithm>
string str;
std::remove_if(str.begin(),[](char c){return c ==' '})
```

## capacity
`capacity` 是 `vector` 在内存中**已经分配好的空间大小**，表示在不重新分配内存的情况下，`vector` 最多能容纳多少个元素
```
vec.claer()  //capacity可能仍然保留
```

auto it = v.begin() + n;
int index = it - v.begin();   // 得到 n