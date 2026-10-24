**YAML** = **Y**AML **A**in't **M**arkup **L**anguage

## 将键值对存入参数服务器
	如：publish_rate: 50
嵌套表示层级
	如：
```
gazebo_ros_control:  
  pid_gains:  
    LF_WHEEL_JOINT:  
      p: 100.0  
    RF_WHEEL_JOINT:  
      p: 100.0  
    LH_WHEEL_JOINT:  
      p: 100.0  
    RH_WHEEL_JOINT:  
      p: 100.0
```
		相当与文件与文件夹，p:100是文件，其他是文件夹
