# 09 视频流水线与 ROS 1 结果

将灯条检测、配对、透视 ROI、数字分类和 PnP 串起来，继续使用第 07/08 节的自训模型，随后通过 ROS 1 传递结构化结果。

## 离线观察

生成序列共 180 帧，每 18 帧更换数字，帧 80～94 遮挡一根灯条。播放设置为每帧等待约 120 ms。

遮挡期间实际看到：**数字和深度没有有效结果，ROI 变为空白**，未保留上一帧。对应代码路径为配对失败、没有有效唯一候选与四角点，后续不执行分类和 PnP。

退出时已处理 124 帧，其中 109 帧有有效候选并通过分类接受规则，15 帧属于遮挡。这里记录的是退出前的处理范围，没有把它写成完整播放 180 帧。

`roi_valid`、`classified`、`has_pose` 分别表示候选、分类与位姿是否有效。有 ROI 但数字被拒绝时，几何位姿仍可能有效；没有 ROI 时，则不等同于分类器输出 negative。

## ROS 消息

| 话题 | 类型 | 内容 |
| --- | --- | --- |
| /digit_course/image_raw | sensor_msgs/Image | 20 Hz 模拟图像 |
| /digit_course/prediction | digit_classifier_demo/DigitPrediction | 数字、分数、位置及有效标志 |
| /digit_course/image_debug | sensor_msgs/Image | 带检测注释的图像 |

结果保留输入的时间戳和 `camera_optical_frame` 标识，位置属于相机光学坐标系。没有候选时：

```text
detected=false, classified=false, class_id=-1
class_name="", has_pose=false
```

接收者应先看有效标志，位置字段的默认零值不能当成测量到了原点。

## 实际订阅结果

额外订阅者按时间戳匹配输入、预测和调试图：

| 检查项 | 实际结果 |
| --- | --- |
| 处理节点完成帧数 | 210 |
| 回调异常 | 0 |
| 匹配消息组 | 74 |
| 其中分类通过规则 | 73 |
| 其中无有效检测候选 | 1 |
| 时间戳、坐标系标识保持 | true |
| 丢失时无效状态清理 | true |

订阅检查覆盖正常与丢失状态后先行结束，处理节点继续到 210 帧。74 是核对的消息组数，不用于推算丢帧率；73 表示通过接受规则，不是核对真实标签后的正确数。

保留图像采集时间是后续查询 TF 或融合其他传感器数据的基础。本例逐帧处理，尚未加入跟踪和运动预测。

## 对应源码

[视频处理入口](../08-cpp-inference/src/main.cpp)；[ROS 发布与回调](ros1/src/main.cpp)；[自定义消息](ros1/msg/DigitPrediction.msg)；[构建配置](ros1/CMakeLists.txt)。
