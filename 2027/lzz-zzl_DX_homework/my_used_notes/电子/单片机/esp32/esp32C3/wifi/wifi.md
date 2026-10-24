### wifi工作流程
![[Pasted image 20260513155101.png]]



![[Pasted image 20260517225146.png]]


### wifi模块的模式

| 模式名称  | 英文代号    | 核心角色       | 典型应用               |
| :---- | :------ | :--------- | :----------------- |
| 站点模式  | STA     | 连别人的 Wi-Fi | 智能插座、温湿度计上报数据      |
| 接入点模式 | AP      | 自己发射 Wi-Fi | 手机直连 ESP32 进行控制、配网 |
| 共存模式  | APSTA   | 既连别人又发射    | Wi-Fi 信号扩展器、配网过渡   |
| 嗅探模式  | Sniffer | 只偷听不联网     | 无线抓包、信号强度分析        |

### WIFI-AP启动
![[Pasted image 20260613151318.png]]



## WIFI -STA启动
app_main() 开始
           │
           ├── nvs_flash_init()            ← 准备存储（固定套路，照抄就行）
           ├── OLED_Init()                 ← 点亮屏幕
           │
           ├── wifi_init_sta()             ← ★ 配置WiFi并启动
           │    ├── esp_wifi_set_mode()    ← 设为STA模式
           │    ├── esp_wifi_set_config()  ← 填SSID和密码
           │    └── esp_wifi_start()       ← ★ 启动（开始扫描连接）
           │
           ├── xEventGroupWaitBits()       ← ★ 停在这等！
           │    │                             等 WiFi 成功或失败
           │    │                            （不往下走）
           │    ├── 事件：WIFI_EVENT_STA_START
           │    │    └→ esp_wifi_connect() ← 发起连接
           │    │
           │    ├── 事件：WIFI_EVENT_STA_DISCONNECTED
           │    │    └→ 重试 esp_wifi_connect()
           │    │
           │    └── 事件：IP_EVENT_STA_GOT_IP  ← ★ 终于连上了！
           │         └→ 设置标志位，app_main继续往下走
           │
           ├── start_webserver()           ← WiFi好了，启动网页服务器
           ├── xTaskCreate(counter_task)   ← 创建一个后台任务（每秒+1）
           ├── xTaskCreate(display_task)   ← 创建另一个后台任务（刷OLED）
           │
           └── while(1){ delay(1秒) }      ← 占位，不让程序退出

