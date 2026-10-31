# 共享源码

| 源码 | 职责 |
| --- | --- |
| [core.py](core.py) | 字体素材、数据划分、TinyCNN、训练、评价、预处理、模型导出和显示 |
| [DigitNet.hpp](cpp/DigitNet.hpp) | C++ 张量处理、DNN 推理、分数、拒识、ROI 与 PnP 组合 |
| [CMakeLists.txt](cpp/CMakeLists.txt) | 构建第 08 节，复用相邻 OpenCV 目录的检测实现 |
| [ROS 包](../09-video-ros/ros1/package.xml) | 第 09 节的自定义消息、图像发布与处理回调 |

## Python 源码入口

在已具备依赖的环境中，从 neural-network 目录直接调用源码，例如：

```bash
python3 04-cpu-training/src/main.py --epochs=468 --limit=128 --augment
```

其他 Python 章节同样使用各自的 `src/main.py`，参数由 `--help` 查看。`run_experiment()` 只负责参数、结果保存与显示，实验逻辑留在各节的 `run()` 中。

预置参考模型、固定图片和生成数据位于仓库之外。需要重新生成参考素材与模型时，可执行共享源码中的对应功能：

```bash
python3 _shared/core.py --chapter=prepare
python3 _shared/core.py --chapter=bundle --epochs=16
```

默认产物目录是 `~/catkin_ws/records/neural-network/`，参考模型包位于其 `reference-models/`；环境变量 `NN_COURSE_RECORDS`、`NN_COURSE_MODELS` 可以覆盖路径。生成参考模型与第 04 节的小样本训练是两组实验，各自的来源与指标分开记录。

## C++ 与 ROS 源码

C++ 构建配置位于 `cpp/`，从 neural-network 目录可用 CMake 构建到仓库外：

```bash
cmake -S _shared/cpp -B /tmp/nn-study-build
cmake --build /tmp/nn-study-build --parallel 2
```

生成的 `nn_course` 使用 `--models` 指定模型包、`--assets` 指定视频帧、`--config` 指定检测参数、`--output` 指定结果目录；模型包必须含 ONNX、metadata 和对应图片。`--mode=parity` 对照参考 logits，`--mode=video` 运行完整流程。

ROS 包是 `digit_classifier_demo`，构建入口见其 [CMakeLists.txt](../09-video-ros/ros1/CMakeLists.txt)。`digit_node` 的 publish 模式发布模拟帧，process 模式订阅图像并发布自定义结果。它们需连接同一个 ROS 1 master。输入素材、模型和输出目录均由程序参数指定。

安装、进程管理和批量检查脚本已从学习目录移除，源码不包含本机密码、Docker 操作或模型二进制。
