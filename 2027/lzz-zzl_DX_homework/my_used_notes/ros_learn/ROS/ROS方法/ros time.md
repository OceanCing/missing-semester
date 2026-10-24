## `ros::Time`
`ros::Time` 表示**一个绝对时刻**（timestamp），内部是两个字段：

```cpp
uint32_t sec;    // 秒
uint32_t nsec;   // 纳秒 (0 ~ 999999999)
```
表示从某个时间开始计时的时间

## `ros::Time::now()`
返回当前 ROS 时间
	如果设`use_sim_time=true`，返回仿真时间  //在launch里设置
		此时时间由gazebo_ros发送


## ros::Duration
用于存储一个时间差值
### 用法：
```cpp
ros::Time t1 = ros::Time::now();      // 时刻
ros::Time t2 = ros::Time::now();      // 另一个时刻
ros::Duration dt = t2 - t1;           // 两个时刻相减 → 时长
double sec = dt.toSec();              // 转成 double 秒
```