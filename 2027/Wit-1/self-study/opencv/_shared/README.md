# 公共构建与素材代码

课程内容按 06～15 的编号文件夹阅读。这里复用构建、窗口、素材与检查代码，使全部后续课程可以一次编译，不必逐节搭工程。

| 文件 | 作用 |
| --- | --- |
| `CMakeLists.txt` | 建立公共库和每节独立的可执行程序 |
| `run.sh` | 选择课程、统一构建、准备素材与批量运行 |
| `src/entry.cpp` | 各节复用的命令行入口与异常处理 |
| `src/common.cpp` | 面板显示、图像保存、指标输出 |
| `src/assets.cpp` | 生成视频、真值、棋盘格等离线素材 |
| `src/detector.cpp` | 串联颜色、轮廓、单根筛选与配对 |
| `config/detector.yaml` | 教学检测参数 |
| `checks.py` | 运行实际 C++ 程序，核对输出和已知生成答案 |
| `ros-run.sh`、`check_ros.py` | 在课程专用 master 上验证 ROS 1 图像通信 |

编号目录的 `src/main.cpp` 存放本节函数；可执行程序的 `main()` 由 `entry.cpp` 提供。只看本节核心函数就能学习对应内容，公共层在需要时再读。

每节也有自己的 CMake 入口。例如从第 07 节目录手动构建：

```bash
cmake -S . -B ~/catkin_ws/.opencv-build/manual-course
cmake --build ~/catkin_ws/.opencv-build/manual-course --target lesson07_mat -j2
```

平常使用 `bash run.sh` 即可。统一构建结果默认在 `~/catkin_ws/.opencv-build/offline-course`，生成素材和运行结果默认在 `~/catkin_ws/records/opencv/offline-course`，不写回学习仓库。

构建需要容器里已有的 C++17 编译器、CMake、OpenCV 4.2 和 Noetic 图像依赖。脚本会加载 `/opt/ros/noetic/setup.bash`，无需另装 ROS 2 或下载模型。

若需要自定本地构建或结果位置，可设置 `OPENCV_COURSE_BUILD`、`OPENCV_COURSE_RECORDS`。批量检查用 `checks/` 中分开的目录，正常学习时同一实验的输出保存当前快照；带 `--variant` 的实验有独立子目录。
