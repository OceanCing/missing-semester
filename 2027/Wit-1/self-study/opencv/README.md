# OpenCV 视觉组自学记录

这里记录从颜色筛选到 ROS 图像处理的 15 节实验。学习时实际运行程序、改变参数、观察变化，再对照关键代码理解原因。

实验使用笔记本和现有的 Ubuntu 20.04 / ROS 1 Noetic 容器。图片、视频、棋盘格与相机数据由程序生成；真实相机采集在第 12 节作为后续扩展。

## 从哪里开始

截至 2026-10-31，01～15 的核心实验已逐节做过对照，ROS 图像输出也已由额外订阅者实际接收并检查。各节 README 保留了实际操作、观察数值和代码解释，也可以按下面的入口重做某一组实验。

| 章节 | 核心内容 | 在后续视觉流程中的用途 |
| --- | --- | --- |
| [01 图片与颜色](01-image-color/README.md) | BGR、像素与差值阈值 | 看懂输入图和颜色筛选 |
| [02 HSV](02-hsv/README.md) | 色相环绕、饱和度、明度 | 筛出颜色范围 |
| [03 轮廓候选](03-contours/README.md) | 面积、旋转外接矩形、长短边比 | 从白色区域找灯条候选 |
| [04 单根方向](04-light-direction/README.md) | 长轴与竖直方向的夹角 | 排除方向不合适的单根条 |
| [05 两根配对](05-light-pairing/README.md) | 方向差、长度、位置与中心 | 得到装甲板候选区域 |
| [06 相机模型](06-camera-model/README.md) | 内参、像素、视线与深度 | 理解二维坐标怎样连接三维空间 |
| [07 Mat 与 ROI](07-mat-roi/README.md) | 类型、浅拷贝、clone、步长 | 正确保存、裁剪和修改图像 |
| [08 预处理](08-preprocess/README.md) | 滤波、阈值、开闭运算 | 理解噪声、断口和光照影响 |
| [09 轮廓几何](09-contour-geometry/README.md) | 边缘、内孔、层次与几何中心 | 看懂不同边界和几何表示 |
| [10 透视 ROI](10-perspective-roi/README.md) | 四点对应、透视变换、插值 | 拉正数字区域，准备模型输入 |
| [11 离线视频](11-video-time/README.md) | 逐帧读取、暂停、帧率与耗时 | 从单张图过渡到连续处理 |
| [12 相机标定](12-camera-calibration/README.md) | 角点检测、内参、畸变、误差 | 为实际距离解算准备成像参数 |
| [13 PnP 与坐标](13-pnp-coordinates/README.md) | 三维点与像素对应、姿态、坐标转换 | 把检测结果转为相机/机器人位置 |
| [14 完整离线流程](14-offline-pipeline/README.md) | 检测、裁剪、PnP、丢失与评价 | 看懂各模块怎样串起来 |
| [15 ROS 图像](15-ros-image/README.md) | Image、cv_bridge、回调和时间戳 | 把 OpenCV 接入 ROS 1 节点 |

各节先记录实际观察，再保留运行方式、关键 API 和参考解释。精确统计值注明来自交互读取还是程序检查。

## 重新运行实验

使用当前的容器开发终端；若有旧练习窗口，先按 Q 退出。下面每个代码块都是可以单独复制执行的一条命令。

连续打开 06～15 的实验：

```bash
bash /home/rosdev/missing-semester/2027/Wit-1/self-study/opencv/run-all.sh
```

每个静态或滑块实验按 Q 进入下一组；第 08 节会依次打开滤波、阈值、形态学，第 12 节打开标定与畸变，第 13 节包含正常尺寸和错误尺寸对照。视频空格暂停/继续；自然播放完停在最后一帧，再按 Q。Ctrl+C 可结束整套运行。

只打开某一节，直接执行它的入口，例如：

```bash
bash /home/rosdev/missing-semester/2027/Wit-1/self-study/opencv/07-mat-roi/run.sh
```

一次编译并检查 06～15：

```bash
bash /home/rosdev/missing-semester/2027/Wit-1/self-study/opencv/verify-all.sh
```

验证会检查真实 C++ 输出，并启动独立的 ROS 1 发布、处理和检查订阅者。它会保存结果后退出，不会逐节打开窗口。正常结束时显示 `ALL OFFLINE OPENCV AND ROS1 CHECKS PASSED`。

当前版本已在 Noetic / OpenCV 4.2.0 容器中编译，18 组离线检查与 ROS 图像通信检查均已通过。01～05 各节另保留颜色、几何和配对检查入口，使用方法见对应 README。

首次运行统一编译全部后续例子并准备素材，之后增量构建。无需每节重新初始化 catkin，也无需安装新镜像。不要把这些命令当成宿主机命令运行；这里使用的是容器路径。

## 文件放在哪里

主机源码目录：

```text
/home/wit/Desktop/DynamicX/missing-semester/2027/Wit-1/self-study/opencv
```

容器中的同一份源码：

```text
/home/rosdev/missing-semester/2027/Wit-1/self-study/opencv
```

06～15 每个编号目录都有 `README.md`、`src/main.cpp`、`CMakeLists.txt`、`run.sh`。[_shared](_shared/README.md) 复用窗口、素材和构建代码；01～05 使用各节自己的构建入口和生成图片。

从其他位置克隆仓库时，在对应章节目录执行 `bash run.sh` 即可；各节记录中的绝对路径对应本机 Docker 挂载。构建与检查依赖为 CMake、C++ 编译器、OpenCV 开发库，以及 Noetic 的 roscpp、rospy、sensor_msgs 和 cv_bridge；已有容器已配置这些依赖。

| 本地生成内容 | 容器路径 |
| --- | --- |
| 编译后的程序 | `~/catkin_ws/.opencv-build/offline-course` |
| 180 帧视频与 PNG、20 张棋盘格 | `~/catkin_ws/records/opencv/offline-course/assets` |
| 每节运行结果 | `~/catkin_ws/records/opencv/offline-course/编号-目录名` |
| 独立自动检查结果 | `~/catkin_ws/records/opencv/offline-course/checks` |
| 构建与检查日志 | `~/catkin_ws/records/opencv/offline-course/session.log` |

带 `--variant` 的不同实验输出到对应子目录，同一实验再次运行会更新当前快照。默认启动会保存初始结果；滑块改变后按 S 保存当前结果。视频按逐帧 CSV 和标注视频保存。

完整生成素材、编译结果和日志留在本地开发目录。学习仓库保留源码、实验配置、素材生成方法和选取的结果图。

## 几个统一约定

| 内容 | 本套约定 |
| --- | --- |
| 软件环境 | Ubuntu 20.04、ROS 1 Noetic、OpenCV 4.2、C++ |
| 后续公共代码 | 使用容器现有 GCC 9 支持的 C++17，无新增编译器依赖 |
| 图像 | 一般为 BGR、8 位，后续模拟相机图为 640×480 |
| 模拟内参 | fx=fy=800、cx=320、cy=240，零畸变 |
| 模拟目标 | 灯条中心线间距 0.15 m、条长 0.12 m |
| 光学坐标 | X 右、Y 下、Z 前 |
| 机器人示例坐标 | X 前、Y 左、Z 上；安装关系明确给定 |
| 时间 | 视频内部时间、处理耗时、ROS 时间戳分别说明 |

模拟标定输出与生成真值都明确标注数据来源。它们能验证程序流程，不是笔记本真实相机的标定文件，也不是战队机器人测量值。

## 实验之间的联系

| 实际现象 | 串起来的理解 |
| --- | --- |
| 修改 ROI 后，原图与浅拷贝一起变绿 | ROI 与浅拷贝共享像素；ROS 回调画框时先 clone |
| 两根条都显示约 25°，却不平行 | 单根角度大小丢失左右信息；配对还要比较长轴方向 |
| mask 仍有红条，却没有配对框 | 颜色之后还有面积、形状、方向与配对条件，需要逐层查看 |
| 交换两个源角点后，数字 ROI 全黑 | 四点对应错误会破坏透视采样，颜色正确也不能补救 |
| 模型尺寸加倍，Z 由 2 m 变为 4 m，重投影仍对齐 | 投影吻合还需配合正确物理尺寸，才能解释米制位置 |
| 加 60 ms 延迟后，处理能力低于素材标注的 30 FPS | 文件帧率、算法耗时与实际播放速度分别理解 |
| 遮挡时 ROI 变为空白、位置字段留空 | 每帧重新判断有效性，避免沿用旧观测 |
| 输出图像保留输入时间戳 | 检测结果仍对应采集时刻，后续 TF 查询使用这个时刻 |

神经网络、卡尔曼跟踪、团队项目复现和真实相机实验，见[后续学习方向](../README.md#后续学习方向)。
