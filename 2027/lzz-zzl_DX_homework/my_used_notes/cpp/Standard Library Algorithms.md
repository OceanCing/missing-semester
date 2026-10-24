它们共同的设计哲学是：**算法和容器解耦**。算法只认**迭代器区间** `[first, last)`，不关心底层是 `vector`、`string`、`deque` 还是数组
	传起始迭代器和终止迭代器

下面按功能分类列举 **最常用的标准库算法**，都定义在 `<algorithm>`（少数在 `<numeric>`）。每个给一句作用 + 一个极简例子，方便你速查。

约定：`v` 是容器，`pred` 是谓词，`val` 是值。
## 一、查找类

| 函数                                     | 作用                        | 例子                                                      |
| -------------------------------------- | ------------------------- | ------------------------------------------------------- |
| `find(first, last, val)`               | 找第一个等于 `val` 的元素，返回迭代器    | `auto it = find(v.begin(), v.end(), 3);`                |
| `find_if(first, last, pred)`           | 找第一个满足条件的元素               | `find_if(v.begin(), v.end(), [](int x){return x>10;});` |
| `find_if_not(first, last, pred)`       | 找第一个**不**满足条件的元素          | `find_if_not(v.begin(), v.end(), isEven);`              |
| `adjacent_find(first, last)`           | 找第一对相邻相等元素                | `adjacent_find(v.begin(), v.end());`                    |
| `search(first, last, s_first, s_last)` | 在区间里找子序列                  | `search(v.begin(), v.end(), pat.begin(), pat.end());`   |
| `lower_bound(first, last, val)`        | 有序区间中第一个 **≥ val** 的位置    | `lower_bound(v.begin(), v.end(), 5);`                   |
| `upper_bound(first, last, val)`        | 有序区间中第一个 **> val** 的位置    | `upper_bound(v.begin(), v.end(), 5);`                   |
| `equal_range(first, last, val)`        | 返回 `[lower, upper)` 一对迭代器 | `auto [l,u] = equal_range(v.begin(), v.end(), 5);`      |
| `binary_search(first, last, val)`      | 有序区间中是否存在 `val`           | `bool b = binary_search(v.begin(), v.end(), 5);`        |

> `lower_bound` / `upper_bound` / `binary_search` 要求区间**已排序**，复杂度 O(log n)。

---

## 二、计数与判断类

| 函数                            | 作用             | 例子                                                         |
| ----------------------------- | -------------- | ---------------------------------------------------------- |
| `count(first, last, val)`     | 统计等于 `val` 的个数 | `count(v.begin(), v.end(), 3);`                            |
| `count_if(first, last, pred)` | 统计满足条件的个数      | `count_if(v.begin(), v.end(), [](int x){return x%2==0;});` |
| `all_of(first, last, pred)`   | 是否**全部**满足     | `all_of(v.begin(), v.end(), [](int x){return x>0;});`      |
| `any_of(first, last, pred)`   | 是否**至少一个**满足   | `any_of(v.begin(), v.end(), [](int x){return x<0;});`      |
| `none_of(first, last, pred)`  | 是否**一个都不**满足   | `none_of(v.begin(), v.end(), [](int x){return x==0;});`    |

---

## 三、修改类（挪动/替换/填充）

| 函数                                   | 作用                      | 例子                                                       |
| ------------------------------------ | ----------------------- | -------------------------------------------------------- |
| `remove(first, last, val)`           | 把等于 `val` 的元素挪到后面，返回新结尾 | `auto e = remove(v.begin(), v.end(), 3);`                |
| `remove_if(first, last, pred)`       | 把满足条件的挪到后面              | `auto e = remove_if(v.begin(), v.end(), isNeg);`         |
| `replace(first, last, old, new)`     | 把所有 `old` 替换成 `new`     | `replace(v.begin(), v.end(), 0, 1);`                     |
| `replace_if(first, last, pred, new)` | 满足条件的替换成 `new`          | `replace_if(v.begin(), v.end(), isNeg, 0);`              |
| `fill(first, last, val)`             | 全部填成 `val`              | `fill(v.begin(), v.end(), 7);`                           |
| `fill_n(first, n, val)`              | 从 `first` 起填 `n` 个      | `fill_n(v.begin(), 5, 0);`                               |
| `copy(first, last, dest)`            | 拷贝到 `dest`              | `copy(v.begin(), v.end(), back_inserter(w));`            |
| `copy_if(first, last, dest, pred)`   | 拷贝满足条件的                 | `copy_if(v.begin(), v.end(), back_inserter(w), isEven);` |
| `reverse(first, last)`               | 原地反转                    | `reverse(v.begin(), v.end());`                           |
| `rotate(first, middle, last)`        | 循环左移，使 `middle` 成为新首    | `rotate(v.begin(), v.begin()+2, v.end());`               |
| `swap_ranges(f1, l1, f2)`            | 交换两段区间                  | `swap_ranges(a.begin(), a.end(), b.begin());`            |
| `unique(first, last)`                | 去除**相邻**重复，返回新结尾        | `auto e = unique(v.begin(), v.end());`                   |

> `remove` / `remove_if` / `unique` 都**不真正删除**，只挪动并返回新结尾，要配合 `erase` 或 `erase_if`。

---

## 四、排序类

| 函数                                  | 作用                          | 例子                                               |
| ----------------------------------- | --------------------------- | ------------------------------------------------ |
| `sort(first, last)`                 | 快速排序，不稳定，O(n log n)         | `sort(v.begin(), v.end());`                      |
| `sort(first, last, cmp)`            | 自定义比较                       | `sort(v.begin(), v.end(), greater<int>());`      |
| `stable_sort(first, last)`          | 稳定排序，相等元素保持原序               | `stable_sort(v.begin(), v.end());`               |
| `partial_sort(first, middle, last)` | 只把前 `middle-first` 个排好      | `partial_sort(v.begin(), v.begin()+3, v.end());` |
| `nth_element(first, nth, last)`     | 让 `nth` 位置放第 n 小元素，左边都小右边都大 | `nth_element(v.begin(), v.begin()+k, v.end());`  |
| `is_sorted(first, last)`            | 是否已排序                       | `bool b = is_sorted(v.begin(), v.end());`        |

> `sort` 要求**随机访问迭代器**，所以 `list` 不能用，得用 `list::sort`。

---

## 五、最值与遍历类

| 函数                                | 作用                      | 例子                                                                  |
| --------------------------------- | ----------------------- | ------------------------------------------------------------------- |
| `min_element(first, last)`        | 最小元素的位置                 | `auto it = min_element(v.begin(), v.end());`                        |
| `max_element(first, last)`        | 最大元素的位置                 | `auto it = max_element(v.begin(), v.end());`                        |
| `minmax_element(first, last)`     | 同时返回最小和最大               | `auto [mn, mx] = minmax_element(v.begin(), v.end());`               |
| `for_each(first, last, f)`        | 对每个元素调用 `f`             | `for_each(v.begin(), v.end(), [](int x){ cout<<x; });`              |
| `transform(first, last, dest, f)` | 对每个元素应用 `f`，结果写到 `dest` | `transform(v.begin(), v.end(), w.begin(), [](int x){return x*2;});` |
