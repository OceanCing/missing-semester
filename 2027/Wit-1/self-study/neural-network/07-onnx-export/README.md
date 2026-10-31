# 07 ONNX 导出与数值对齐

导出的是小样本增强组保存的第 408 轮权重，来源为本机 `experiments/small-long-augment/model.pt`。

## 实际结果

| 检查项 | 结果 |
| --- | --- |
| PyTorch | 2.4.1+cpu |
| 推理后端 | Python OpenCV 4.2.0 |
| ONNX opset | 11 |
| 输入 | float32，`[1,1,32,32]` |
| 输出 | 11 类原始 logits |
| 对照样本 | 12 张，包含数字、negative 和带灯条 ROI |
| 最大绝对差 | `7.62939453125e-06`，约 0.00000763 |
| 误差门槛 | 0.001 |

误差低于门槛，同一模型、相同输入上的 PyTorch 与 OpenCV 对照通过。logits 是 softmax 前的类别分数，这个误差描述数值一致性，不是分类错误率。

ONNX 为 206031 字节，SHA256 为：

```text
e9a3fa20ce30bec66508585268c175e3ad4532deadab5febef537bb8f56116f2
```

实际文件与 metadata 记录一致。部署包中的文件名仍是 `baseline.onnx`，但内容来自这次自训权重。

## 模型包与接口

| 本机生成文件 | 用途 |
| --- | --- |
| baseline.pt | PyTorch 参数 |
| baseline.onnx | 计算图与参数 |
| metadata.json | 类别顺序、输入约定和文件 hash |
| parity.json、fixtures/ | 固定图片与当前模型的参考 logits |

导出前调用 `eval()`，固定 batch=1，使用 Conv、ReLU、MaxPool、Flatten、Gemm，以适配已有 OpenCV 4.2。

ROI 检测、灰度、缩放、归一化、softmax 和接受门槛都在模型外。第 06 节运行时设置的 0.95 不会自动写进权重；部署包默认门槛为 0.65。导出与对齐保留原有计算，不会自动改善 B→8 或低亮度识别。

## 对应源码

[导出与对照](src/main.py)；[模型定义与 export_model()](../_shared/core.py)。模型包为本机产物，不随笔记提交。
