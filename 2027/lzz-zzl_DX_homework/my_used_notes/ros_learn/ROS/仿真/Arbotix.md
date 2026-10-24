根据输入大致仿真返回数据

## 使用
#### launch 文件中配置 arbotix 节点

**launch 示例代码**

```
<node name="arbotix" pkg="arbotix_python" type="arbotix_driver" output="screen">
     <rosparam file="$(find my_urdf05_rviz)/config/hello.yaml" command="load" />
     <param name="sim" value="true" />
</node>
```

代码解释:

\<node> 调用了 arbotix_python 功能包下的 arbotix_driver 节点

\<rosparam> arbotix 驱动机器人运行时，需要获取机器人信息，可以通过 file 加载配置文件
<param> 在仿真环境下，需要配置 sim 为 true

## yaml配置
#### base_controller
```
  base_controller: {  
    #类型: 差速控制器  
    type: diff_controller,  
    #参考坐标  
    base_frame_id: base_footprint,  
    #两个轮子之间的间距  
    base_width: 0.2,  
    #控制频率  
    ticks_meter: 2000,  
    #PID控制参数，使机器人车轮快速达到预期速度  
    Kp: 12,  
    Kd: 12,  
    Ki: 0,  
    Ko: 50,  
    #加速限制  
    accel_limit: 1.0  
  }  
}
```


