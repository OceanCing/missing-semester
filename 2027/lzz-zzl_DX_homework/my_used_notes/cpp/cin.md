读一行
```
#include <iostream>
#include <string>
using namespace std;
int main() {
    string line;
    getline(cin, line);   // 读一整行，包括空格
    cout << line << endl;
}
```

## 问题
cin 和getline一起用
1. **`cin >> n`** 读取了 `3`，但**把回车符 `\n` 留在了输入缓冲区中**。
2. 接下来的 **`getline(cin, a)`** 会读到缓冲区里残留的那个 `\n`，于是它认为"读到了一整行（空行）"，立刻返回，`a` 就是一个空字符串。
3. 程序继续执行 `cout << a << endl;`，输出一个空行。

所以看起来就像"输入数字后直接返回了"
## 解决
```cpp
#include <limits>
cin.ignore(numeric_limits<streamsize>::max(), '\n');
```
