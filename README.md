# 二维码识别

此示例演示如何解码1D/2D条形码。目前，esp_code_scanner可以解码的格式包括：
- 1D 条形码:
	- code39
	- code128
- 2D 条形码:
	- QR code

## 如何使用

在项目配置和构建之前，使用idf.py set-target <chip_name>设置正确的芯片目标，这里我的<chip_name>是esp32s3

### 所需硬件

* 带有ESP32/ESP32-S2/ESP32-S3 SoC的开发板（例如，ESP-EYE，ESP-WROVER-KIT，ESPS3-EYE等）
* 用于供电和编程的USB线
* 相机模块：OV2640/OV3660/GC0308/GC032A图像传感器（推荐焦距：5cm-20cm）
* LCD（可选）：ST7789等等

### 所需软件

需要创建阿里云平台产品、设备，获取三元组（一型一密），并且在code_recognition/main/app_main.c/app_main()中配置
```
	HAL_SetProductKey("YourProductKey");

	HAL_SetDeviceName("YourDeviceName");
	HAL_SetDeviceSecret("YourDeviceSecret");
```
在code_recognition/main/app_main.c中26、27行配置wifi_ssid和wifi_pass
```
	#define wifi_ssid "your wifi name"
	#define wifi_pass "your wifi password"
```

### 配置项目

根据开发板使用sdkconfig.defaults.<chip_name>已经设置了一些默认设置。

### 构建和烧录（Build and Flash）

运行idf.py -p PORT flash monitor来构建，刷新和监视项目。（要退出串行监视器，请输入Ctrl-]。）

请参阅[Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html)以获取配置和使用ESP-IDF构建项目的完整步骤。

## 输出示例

请确保代码清晰完整的被摄像头拍到。
如果成功解码一个二维码，你将能够看到如下显示的信息：

```
I (48641) APP_CODE_SCANNER: Decode time in 65 ms.
I (48641) APP_CODE_SCANNER: Decoded QR-Code symbol "测试"

gate_publish|316 :: publish success
[dbg] iotx_mc_cycle(1533): PUBACK
example_event_handle|328 :: msg->event_type : 9
example_event_handle|360 :: msg->event_type : 9, IOTX_MQTT_EVENT_PUBLISH_SUCCESS
```


## 故障排查

如有任何技术疑问，请在GitHub上开启一个[issue]我将尽快回复您。
