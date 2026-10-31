# 神经网络自学记录

接续 OpenCV 的灯条检测、ROI、PnP 和 ROS 图像实验，在笔记本上完成 CPU 训练、评价、ONNX 导出、C++ 推理与 ROS 结果传递。各节按实际操作和观察整理。

## 内容

内容分为[笔记](note.md)和下面的 01～10 实验记录。

| 章节 | 记录重点 |
| --- | --- |
| [01 训练流程](01-training-recall/README.md) | backward 与 step、学习率、eval 与 no_grad |
| [02 数据划分](02-dataset-splits/README.md) | 来源分组、重复负样本、两组训练 |
| [03 ROI 输入](03-roi-preprocessing/README.md) | 灰度、归一化、反色、张量形状 |
| [04 CPU 训练](04-cpu-training/README.md) | 更新次数、小样本长训练、增强与选权重 |
| [05 错例评价](05-generalization/README.md) | 混淆矩阵、9→0、模糊与低亮度 |
| [06 置信度与拒识](06-confidence-rejection/README.md) | B→8、高分错误、提高门槛的代价 |
| [07 ONNX](07-onnx-export/README.md) | 模型包、算子与数值对齐 |
| [08 C++ 推理](08-cpp-inference/README.md) | 完整 logits 对照、反色故障、计时范围 |
| [09 视频与 ROS 1](09-video-ros/README.md) | 遮挡清空、有效标志、结果订阅 |
| [10 工程排查](10-engineering-review/README.md) | 类别表错误与分层排查 |

## 本轮实际结果

部署使用 128 张训练样本、468 次更新、训练期增强后按验证准确率选出的**第 408 轮权重**，种子 731，batch size=128，学习率 0.001。

| 对照 | 实际结果 |
| --- | --- |
| 完整 grouped / random 验证准确率 | 96.97% / 98.38% |
| 小样本增强模型验证准确率 | 82.12% |
| 同模型输入亮度乘以 0.2 | 33.23% |
| PyTorch → Python OpenCV | 12 张固定输入最大 logits 差约 0.00000763 |
| PyTorch → C++ OpenCV | 最大差约 0.00001144，门槛为 0.001 |
| 视频遮挡 | 数字与位姿无有效结果，ROI 清空 |
| ROS 接收 | 处理 210 帧，额外订阅者核对 74 组，时间戳及无效状态检查通过 |
| 错类别表 | ID=7、score=1.0 不变，标签从 7 变成 3 |

## 数据与环境

素材由 OpenCV Hershey 字体生成，0～9 和 negative 共 11 类、7920 张图，分组训练/验证/测试数量为 4950/990/1980。分组后的验证集仍有 24 张重复负样本，详见第 02 节。

本轮素材缓存记录的生成版本为 OpenCV 4.10.0；训练与部署运行在 Ubuntu 20.04 / ROS 1 Noetic 容器中，使用 PyTorch 2.4.1 CPU、Python 3.8、NumPy 1.24.4、ONNX 1.16.2、OpenCV 4.2.0 和 C++17。数值是本轮模拟实验结果，不作为真实装甲板精度或硬件性能结论。

## 源码与本机产物

目录只保留笔记、[示例源码](_shared/README.md)、CMake/ROS 构建配置和少量笔记配图。Python 示例直接以各节 `src/main.py` 为入口；第 08 节提供 C++ 程序，第 09 节包含 ROS 包。

数据、权重、ONNX、完整日志及构建产物保存在仓库之外。源码默认写入 `~/catkin_ws/records/neural-network/`，可通过 `NN_COURSE_RECORDS` 修改；预置参考模型默认从该目录下的 `reference-models/` 读取，也可通过 `NN_COURSE_MODELS` 指定。自训模型通过 `--checkpoint` 或 `--models` 显式选择。

源码保留了素材生成、训练与导出逻辑，使用方式见[共享实现说明](_shared/README.md)。本目录不再保留安装、启动包装和批量验证脚本。

## 参考

- [PyTorch 训练流程](https://docs.pytorch.org/tutorials/beginner/basics/quickstart_tutorial.html)
- [PyTorch 历史版本与 CPU 安装](https://pytorch.org/get-started/previous-versions/)
- [分组交叉验证](https://scikit-learn.org/stable/modules/cross_validation.html#group-k-fold)
- [OpenCV 4.2 DNN](https://docs.opencv.org/4.2.0/d6/d0f/group__dnn.html)
