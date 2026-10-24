线性代数的库


## 声明矩阵

```cpp
// 1. 声明矩阵和向量
    Eigen::Matrix2d m;                    // 2x2 双精度矩阵
    Eigen::Vector2d v(1.0, 2.0);          // 2维向量（列向量）
    
    m << 1.0, 2.0,
         3.0, 4.0;
```

```cpp
 // 3. 基本运算
    Eigen::Vector2d result = m * v;       // 矩阵-向量乘法
    std::cout << "矩阵 m =\n" << m << std::endl;
    std::cout << "向量 v = " << v.transpose() << std::endl;
    std::cout << "m * v = " << result.transpose() << std::endl;
    // 4. 其他常用操作
    std::cout << "转置:\n" << m.transpose() << std::endl;
    std::cout << "行列式: " << m.determinant() << std::endl;
```

## 使用
