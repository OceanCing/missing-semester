# 01 训练流程与梯度更新

使用预置参考模型，分别检查反向传播、参数更新和推理模式。

## 实验记录

| 操作 | 权重最大绝对变化量 |
| --- | --- |
| 学习率 0.001，只执行 backward | 0.0 |
| 学习率 0.001，再执行一步 SGD | 约 0.0003335 |
| 学习率改为 0.01，执行一步 SGD | 约 0.00335 |

反向传播后权重没有变化，执行 `step()` 后才更新。相同输入、初始权重和梯度下，学习率增大十倍，这一步 SGD 的更新量也约增大十倍。

```python
optimizer.zero_grad()
logits = model(images)
loss = cross_entropy(logits, labels)
loss.backward()
optimizer.step()
```

`backward()` 计算损失对参数的梯度，学习率由优化器在更新时使用。SGD 可以理解为 `参数 -= 学习率 × 梯度`；Adam 还维护梯度统计量，完整训练不能直接套用一步 SGD 的比例关系。

| 推理模式检查 | 实际读数 |
| --- | --- |
| 只调用 eval，输出仍记录求导计算 | true |
| 在 no_grad 内前向，输出记录求导计算 | false |

`eval()` 切换 Dropout 等层的行为；`no_grad()` 关闭作用域内的求导记录。常规推理配合使用：

```python
model.eval()
with torch.no_grad():
    logits = model(images)
```

## 对应源码

[实验代码](src/main.py)；[共享实现](../_shared/core.py)中的 `TinyCNN`、`train()`。
