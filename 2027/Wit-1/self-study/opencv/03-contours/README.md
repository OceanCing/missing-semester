# OpenCV 03：轮廓与灯条候选

HSV 筛选回答的是“哪些像素符合颜色条件”。本课从 mask 提取白色区域的边界，再按面积和形状筛选灯条候选。示意图含两根亮红条、较宽的红块、小型细长红色噪点，以及蓝、黄、灰白干扰。

## 实验记录

保持长短边比下限为 3.0，将面积下限从 200 逐渐降低，候选数从 2 变成 3，最后变成 4。两处细长小噪点的轮廓面积分别为 51 和 26：面积下限降到 51 时，第一处通过；降到 26 时，第二处也通过。

它们的长短边比分别约为 5.67 和 6.5，已经满足细长条件。默认条件下是面积门槛将它们排除。这说明颜色、面积和形状条件需要共同使用，单凭“红色且细长”仍可能选入噪点。

将面积下限恢复到约 200（运行日志中为 199），再把长短边比下限从 3.0 降到 1.0，观察到右下方两个暗红方块出现绿框，候选数变为 4。两个红块的轮廓面积均为 3213，长短边比约为 1.24：它们满足面积条件，但默认不满足细长条件；放宽形状条件后便进入候选。颜色较暗不代表会被当前 HSV 规则排除。

当前绿框标记的是单个灯条候选，程序尚未比较方向，也未将两根候选配成装甲板。提高面积下限到 4000 的结果已通过程序自动检查，交互观察尚未记录。

横放一根细长红条不会改变其面积和长短边比，因此仍能通过当前规则。长边和短边是按长度取最大值、最小值，不是固定的图像水平方向与竖直方向。方向条件在[下一课](../04-light-direction/README.md)单独加入。

## 从像素到轮廓

本课继续使用上一课的 HSV 红色规则，直接复用 [hsv_filter.hpp](../02-hsv/src/hsv_filter.hpp)，得到单通道二值 mask。

```cpp
std::vector<std::vector<cv::Point>> contours;
cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
```

`contours` 是一组轮廓，每个轮廓由边界点组成。`RETR_EXTERNAL` 取外层边界，`CHAIN_APPROX_SIMPLE` 简化边界点的存储。本课各红色区域分离，轮廓可以分别标号；它们还只是满足颜色条件的区域，不等同于已经识别出灯条。

## 用面积和长宽比筛选

核心处理在 [contour_filter.hpp](src/contour_filter.hpp)：

```cpp
info.area = cv::contourArea(points);
info.rectangle = cv::minAreaRect(points);
```

面积用于排除太小的区域；`minAreaRect` 给轮廓拟合一个可旋转的最小外接矩形。取矩形较长边除以较短边，得到长短边比：

```text
ratio = 长边 / 短边
```

正方形附近的比值接近 1，细长条的比值较大。本课使用：

```text
轮廓面积 >= Min area，并且长短边比 >= Min ratio
```

默认面积下限为 200、长短边比下限为 3.0。单像素等退化轮廓没有有效的矩形短边，程序直接忽略，避免除以零。

`contourArea` 计算边界围成的几何面积，与 mask 中白色像素的数量不必相等。面积门槛与图像分辨率有关，本课数值用于这张 640×360 示意图。当前筛选的是候选，后续会结合方向、位置关系和真实图像继续验证。

## 运行

在已有的容器开发终端运行：

```bash
bash /home/rosdev/missing-semester/2027/Wit-1/self-study/opencv/03-contours/run.sh --verify
```

脚本自动编译、检查并打开窗口。重开时可去掉 `--verify`；从其他位置克隆仓库时，在本节目录运行 `bash run.sh`。

保持学习目录的整体结构，使本课能找到上一课的 HSV 头文件。构建结果位于 `~/catkin_ws/.opencv-build/03-contours`，运行日志和输出位于 `~/catkin_ws/records/opencv/03-contours`。

## 窗口与观察步骤

| 区域或滑块 | 作用 |
| --- | --- |
| 左上 Original | 原始图片，包括小红噪点和宽红块 |
| 右上 Red mask | 上一课 HSV 红色规则筛出的白色区域 |
| 左下 All outer contours | 画出所有外轮廓并标上编号 |
| 右下 Light candidates | 保留原图背景，用绿框标出通过条件的候选 |
| Min area | 面积下限，初始为 200 |
| Min ratio x10 | 长短边比下限的十倍，30 表示 3.0 |

第一组实验只改面积：

1. 保持 `Min ratio x10=30`，观察默认两个绿框。
2. 将 `Min area` 从 200 降到 0，观察小红噪点是否也出现绿框，底部 Candidates 是否增加。
3. 恢复 200，再将面积下限提高到 4000，观察两根亮红条是否也被排除。

第二组实验只改长宽比：恢复 `Min area=200`，将 `Min ratio x10` 从 30 降到 10，观察较宽的红块是否进入候选；再恢复 30。

按 S 保存当前图像与 `contours.csv`；按 Q、Esc 或关闭窗口退出。

## 文件与检查

| 文件 | 用途 |
| --- | --- |
| `src/contour_filter.hpp` | 提取轮廓、计算几何特征、判断候选 |
| `src/main.cpp` | 复用 HSV、绘制结果、滑块与输出 |
| `assets/contour_scene.png` | 包含灯条形状、红块和噪点的教学图 |
| `scripts/make_sample.py` | 重新生成教学图 |
| `scripts/check_lesson.py` | 调用实际 C++ 程序检查候选数量、边界和退化轮廓 |
| `run.sh` | 自动编译并按需验证、运行 |

`contours.csv` 记录编号、矩形中心、面积、长短边长度、长短边比和是否通过。容器中已完成编译与自动检查：默认提取 7 个外轮廓、接受 2 个候选；面积下限为 0 时接受 4 个，为 4000 时接受 0 个；面积下限为 200、长短边比下限为 1.0 时接受 4 个。检查还覆盖面积边界、退化轮廓与非法参数。

## 参考

- [OpenCV 4.2 轮廓入门](https://docs.opencv.org/4.2.0/d4/d73/tutorial_py_contours_begin.html)
- [轮廓、面积、最小外接矩形接口](https://docs.opencv.org/4.2.0/d3/dc0/group__imgproc__shape.html)
