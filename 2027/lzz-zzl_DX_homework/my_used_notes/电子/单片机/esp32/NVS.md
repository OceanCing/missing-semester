
## 需要的库
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"

## 使用流程

#### 总结
句柄只是开启用关闭的，不通类似通道，创建时和区域绑定
nvs_open的第一个参数如"storage"，是区域的名字，一个区域里放多个键值对
同区域同名覆盖

### 初始化
  ```
nvs_flash_init();
  ```

### 储存
#### 1·创建句柄然后打开
```
nvs_handle_t handle;
//打开
 esp_err_t err = nvs_open("storage", NVS_READWRITE, &handle);//注意NVS_READWRITE
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "打开NVS失败: %s", esp_err_to_name(err));
        return;
    }
```
#### 2·把数据加入句柄
```
    nvs_set_i32(handle, "counter", 100);       //第二个参数是键
    nvs_set_str(handle, "username", "esp32_user");  //本质键值对
    nvs_set_u8(handle, "level", 5);
```
#### 提交 关闭
```
    nvs_commit(handle);
    nvs_close(handle);
```

### 读取
```
//依旧打开
nvs_handle_t handle;
esp_err_t err = nvs_open("storage", NVS_READONLY, &handle); //注意NVS_READONLY
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "打开NVS失败");
        return;
    }
//读取
nvs_get_i32(handle, "counter", &counter);     //放在第三个参数里
关
nvs_close(handle);
```
### 删除
```
nvs_handle_t handle;
nvs_open("storage", NVS_READWRITE, &handle);
    
// 删除单个键
nvs_erase_key(handle, "counter");
    
// 删除整个命名空间
// nvs_erase_all(handle);
    
nvs_commit(handle);  //记得提交
nvs_close(handle);
```


## 📊 常用 API 速查表

### 写入操作

|函数|用途|
|---|---|
|`nvs_set_i8(handle, key, value)`|存储 8位整数|
|`nvs_set_i16(handle, key, value)`|存储 16位整数|
|`nvs_set_i32(handle, key, value)`|存储 32位整数|
|`nvs_set_i64(handle, key, value)`|存储 64位整数|
|`nvs_set_u8(handle, key, value)`|存储 无符号8位|
|`nvs_set_str(handle, key, value)`|存储 字符串|
|`nvs_set_blob(handle, key, value, len)`|存储 二进制数据|

### 读取操作

|函数|用途|
|---|---|
|`nvs_get_i8(handle, key, &out)`|读取 8位整数|
|`nvs_get_str(handle, key, buf, &len)`|读取 字符串|
|`nvs_get_blob(handle, key, buf, &len)`|读取 二进制数据|

### 管理操作

|函数|用途|
|---|---|
|`nvs_erase_key(handle, key)`|删除单个键|
|`nvs_erase_all(handle)`|删除整个命名空间|
|`nvs_commit(handle)`|提交到Flash|
|`nvs_close(handle)`|关闭句柄|