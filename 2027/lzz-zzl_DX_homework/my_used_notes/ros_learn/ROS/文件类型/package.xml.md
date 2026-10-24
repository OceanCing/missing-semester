## 文件结构解析

### 1. 包的基本信息

xml
```
<name>smb_description</name>          <!-- 包名 -->
<version>0.1.1</version>              <!-- 版本号 -->
<description>Super mega bot description</description>  <!-- 描述 -->
<maintainer email="...">Koen Kraemer</maintainer>  <!-- 维护者 -->
<license>BSD</license>                <!-- 许可证 -->
<author email="...">Koen Kraemer</author>  <!-- 作者 -->
```

| 标签            | 含义                                    |
| ------------- | ------------------------------------- |
| `name`        | 包的名字，**必须**与文件夹名一致（`smb_description`） |
| `version`     | 版本号，用于版本管理                            |
| `description` | 包的简要描述                                |
| `maintainer`  | 维护者（谁负责这个包）                           |
| `license`     | 开源许可证（BSD 是宽松的许可证）                    |
| `author`      | 作者                                    |

---

### 2. 构建工具依赖

xml

```
<buildtool_depend>catkin</buildtool_depend>
```

|标签|含义|
|---|---|
|`buildtool_depend`|构建这个包**需要**的工具|
|`catkin`|ROS 1 的构建系统（相当于 ROS 2 的 `ament`）|

表示这个包是用 `catkin` 系统编译的。

---

### 3. 运行时依赖

xml

```
<depend>xacro</depend>                           <!-- Xacro 宏语言 -->
<depend>urdf</depend>                            <!-- URDF 机器人描述 -->
<depend>roslib</depend>                          <!-- ROS 基础库 -->
<depend>hector_gazebo_plugins</depend>           <!-- Hector 的 Gazebo 插件 -->
<exec_depend>velodyne_description</exec_depend>   <!-- Velodyne 雷达描述 -->
<exec_depend>velodyne_gazebo_plugins</exec_depend><!-- Velodyne 的 Gazebo 插件 -->
```

|标签|含义|作用|
|---|---|---|
|`depend`|**构建**和**运行**都需要的依赖|编译时需要，运行时也需要|
|`exec_depend`|**仅运行时**需要的依赖|编译时不需要，运行时需要|
|`build_depend`|**仅编译时**需要的依赖|编译时需要，运行时不需要|

---

## 具体依赖的作用