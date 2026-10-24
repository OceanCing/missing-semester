tf存坐标系之间的变化

## 原理

tf变换： 4*4矩阵
```
T = [ R  t ]      R: 3×3 旋转矩阵
    [ 0  1 ]      t: 3×1 平移向量
```

表示 子坐标系的原点和朝向，相对于父坐标系是什么样

实际用三个平移分量和四元数存储

| 分量  | 存储形式               | 含义                   |
| --- | ------------------ | -------------------- |
| 平移  | `(x, y, z)`        | 子坐标系原点在父坐标系中的位置      |
| 旋转  | `(qx, qy, qz, qw)` | 子坐标系朝向相对父坐标系的旋转（四元数） |
即是
## `geometry_msgs/TransformStamped`
```yaml
header:
  stamp: 时间戳
  frame_id: "map"          # 父坐标系
child_frame_id: "base_link" # 子坐标系
transform:
  translation: {x, y, z}
  rotation:    {x, y, z, w}  # 四元数
```
# 监听tf
## buffer
用来存多条TransformStamped，记录时间，避免时间不匹配

### buffer初始化
```cpp
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>

tf2_ros::Buffer tfBuffer;
tf2_ros::TransformListener tfListener(tfBuffer);  // 构造即开始订阅 /tf
```
- `Listener`：订阅者，负责把话题数据喂给 Buffe(是/tf` 和 `/tf_static`
- 两者生命周期要一致，通常都作为成员变量或局部变量一起存在
- `TransformListener` 构造后会**立刻**开始订阅，不需要手动sub

### 查看变换
```cpp
geometry_msgs::TransformStamped transform =
    tfBuffer.lookupTransform(
        "map",              // target_frame：要转到哪个坐标系
        "laser",            // source_frame：从哪个坐标系转
        ros::Time(0),       // 查询时刻
        ros::Duration(1.0)  // 超时等待
    );
```
其中time
- ros::Time(0)              // 取最新可用的变换（最常用）
- ros::Time::now()          // 取当前时刻（可能还没到，会等）
- msg.header.stamp          // 取传感器数据那一刻（时间同步的关键）
- buffer默认缓存10s  tf2_ros::Buffer tfBuffer(ros::Duration(10.0));  这样自定义

### lookupTransform异常情况

| `LookupException`        | 两个 frame 不在同一棵树里，或 frame 不存在 |
| ------------------------ | ---------------------------- |
| `ExtrapolationException` | 请求的时刻早于 Buffer 最老 / 晚于最新     |
| `ConnectivityException`  | 两个 frame 之间没有路径              |
| `TimeoutException`       | 超时还没等到                       |
##  应用变换：doTransform


# 发布
```cpp
#include <tf2_ros/transform_broadcaster.h>

tf2_ros::TransformBroadcaster br;

geometry_msgs::TransformStamped tf_msg;
tf_msg.header.stamp = time;
tf_msg.header.frame_id = "odom";
tf_msg.child_frame_id  = "base_link";
// ... 填 translation / rotation

br.sendTransform(tf_msg);   // 发到 /tf，完事
```

# `time`
`time` 是 **`ros::Time` 类型**，表示**当前这一控制周期的时刻**

```cpp
void StandardController::update(const ros::Time& time,
                                const ros::Duration& period) {
    // time   : 当前周期的时刻（绝对时间）
    // period : 距离上一个周期的时间差（相对时长）
}
```
- `time` 是**绝对时刻**，比如 `1712345678.123456789`
- `period` 是**时间间隔**，比如 `0.01`（10ms，对应 100Hz）



## 查看
```
rosrun rqt_tf_tree rqt_tf_tree
```
