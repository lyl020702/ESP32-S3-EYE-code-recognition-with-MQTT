// #include "mqtt_solo.h"
// #include <string.h>
// #include "esp_event.h"
// #include "esp_system.h"
// #include "driver/uart.h"
// #include "esp_log.h"
// #include <stdio.h>
// #include "freertos/queue.h"
// #include "driver/gpio.h"

// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "who_camera.h"
// #include "who_lcd.h"
// #include "who_button.h"
// #include "who_adc_button.h"
// #include "driver/uart.h"
// #include "esp_code_scanner.h"
// #include "app_peripherals.h"
// typedef struct
// typedef struct
// {
//     int flag;            //  事件ID，人脸识别1，二维码扫描2，助手3
//     int face_detection;  // 1 开启人脸识别 0 关闭
//     int face_ID;         // 人脸ID 1：超级管理员 2：普通管理员 3/4/5：
//     int scan_QR_code;    // 扫描二维码开关 1 开启二维码扫码 0 关闭
//     char QR_content[50]; // 二维码内容
//     int regulation;      // 规则 1 定时控制 2 实时控制 3 规则控制
//     int GPT_ID;          // 助手ID
// } environment_variable_t;

// char DEMO_PRODUCT_KEY[IOTX_PRODUCT_KEY_LEN + 1] = {0};
// char DEMO_DEVICE_NAME[IOTX_DEVICE_NAME_LEN + 1] = {0};
// char DEMO_DEVICE_SECRET[IOTX_DEVICE_SECRET_LEN + 1] = {0};

// extern bool rx_sta_bool;
// #define UART_NUM UART_NUM_1
// #define UART_TX_PIN 38
// #define UART_RX_PIN 39

// environment_variable_t environment_variable;


// // uart
// void uart_init()
// {

//     uart_config_t uart_config = {

//         .baud_rate = 115200,

//         .data_bits = UART_DATA_8_BITS,

//         .parity = UART_PARITY_DISABLE,

//         .stop_bits = UART_STOP_BITS_1,

//         .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,

//     };

//     uart_param_config(UART_NUM, &uart_config);

//     uart_set_pin(UART_NUM, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

//     uart_driver_install(UART_NUM, 1024, 0, 0, NULL, 0);
// }


// // 当有消息到达时的处理函数
// void example_message_arrive(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
// {
//     uart_init();
//     environment_variable_t control; // 定义一个环境变量的控制变量

//     // cJSON相关变量，用于解析JSON数据
//     cJSON *root = NULL;   // JSON根节点
//     cJSON *params = NULL; // JSON参数节点

//     // 获取MQTT的主题信息
//     iotx_mqtt_topic_info_t *topic_info = (iotx_mqtt_topic_info_pt)msg->msg;

//     // 根据MQTT消息的事件类型进行处理
//     switch (msg->event_type)
//     {
//     case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
//         // 当收到发布的消息时的处理

//         // 打印接收到的消息
//         EXAMPLE_TRACE("Message Arrived:");
//         EXAMPLE_TRACE("Topic  : %.*s", topic_info->topic_len, topic_info->ptopic);    // 打印主题信息
//         EXAMPLE_TRACE("Payload: %.*s", topic_info->payload_len, topic_info->payload); // 打印负载信息
//         EXAMPLE_TRACE("\n");

//         root = cJSON_Parse(topic_info->payload); // 解析收到的JSON数据

//         char *pstr = NULL;
//         char temp_topic[256];
//         snprintf(temp_topic, sizeof(temp_topic), "%.*s", topic_info->topic_len, topic_info->ptopic);
//         // 判断主题是否是"/a1BC3i5kPNZ/FP_box_001/user/getMsg"
//         if (strcmp(temp_topic, "/a1BC3i5kPNZ/AI_model_1/user/getMsg") == 0)
//         {
//             cJSON *event = NULL; // 定义一个JSON对象

//             pstr = cJSON_PrintUnformatted(root); // 将JSON对象转化为无格式的字符串
//             printf("%s\n\n", pstr);              // 打印字符串

//             event = cJSON_GetObjectItem(root, "flag"); // 获取JSON对象中"flag"的值

//             pstr = cJSON_PrintUnformatted(event); // 将JSON对象转化为无格式的字符串
//             printf("event: %s\n\n", pstr);        // 打印字符串

//             int flag = atoi(pstr);         // 将字符串转化为整数
//             printf("event: %d\n\n", flag); // 打印整数

//             // 根据flag的值进行不同的处理
//             switch (flag)
//             {
//             case 1:
//                 // cJSON解析，获取到人脸识别命令
//                 control.face_detection = cJSON_GetObjectItem(root, "face_detection")->valueint;
//                 break;
//             case 2:
//                 // cJSON解析，获取到对应的二维码识别命令
//                 control.scan_QR_code = cJSON_GetObjectItem(root, "scan_QR_code")->valueint;
//                 break;
//             case 3:
//                 // cJSON解析，获取GPT_ID
//                 control.GPT_ID = cJSON_GetObjectItem(root, "GPT_ID")->valueint;
//                 break;
//             default:
//                 break;
//             }

//             free(pstr); // 释放字符串内存
//             if (pstr != NULL)
//             {
//                 free(pstr);
//                 pstr = NULL; // 将 pstr 重置为 NULL，防止后续代码误用
//             }
//         }
//         else if (strcmp(temp_topic, "/sys/a19sCsAuZcM/AI_model_1/thing/service/property/set") == 0)
//         {
//             cJSON *params = NULL;                // 定义一个JSON对象
//             int face_id;                         // 定义一个JSON对象
//             pstr = cJSON_PrintUnformatted(root); // 将JSON对象转化为无格式的字符串
//             printf("%s\n\n", pstr);              // 打印字符串

//             params = cJSON_GetObjectItem(root, "params"); // 获取JSON对象中"params"的值
//             pstr = cJSON_PrintUnformatted(params);        // 将JSON对象转化为无格式的字符串
//             printf("params: %s\n\n", pstr);               // 打印字符串

//             face_id = cJSON_GetObjectItem(params, "face_id")->valueint;
//             // pstr = cJSON_PrintUnformatted(face_id); // 将JSON对象转化为无格式的字符串
//             printf("face_id: %d\n\n", face_id); // 打印字符串
//             int len;
//             switch (face_id)
//             {
//             case 1:
//                 len = uart_write_bytes(UART_NUM, "1", strlen("1"));
//                 if (len == strlen((const void *)"1"))
//                 {
//                     ESP_LOGI("UART", "Sent: %s", "1");
//                 }
//                 else
//                 {
//                     ESP_LOGE("UART", "Failed to send: %s", "1");
//                 }

//                 break;

//             default:
//                 break;
//             }
//             free(pstr); // 释放字符串内存
//         }

//         break;
//     default:
//         break;
//     }
// }

// // 消息订阅函数
// int example_subscribe(void *handle)
// {
//     int res = 0;
//     const char *fmt = "/a19sCsAuZcM/AI_model_1/user/getMsg"; // 订阅主题格式字符串
//     char *topic = NULL;                                      // 存储订阅主题字符串的指针
//     int topic_len = 0;                                       // 订阅主题字符串的长度

//     topic_len = strlen(fmt) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1; // 计算订阅主题字符串的长度
//     topic = HAL_Malloc(topic_len);                                                     // 分配内存用于存储订阅主题字符串
//     if (topic == NULL)
//     {
//         EXAMPLE_TRACE("memory not enough"); // 内存分配失败的错误提示
//         return -1;
//     }

//     memset(topic, 0, topic_len);                                                           // 清空订阅主题字符串的内存空间
//     HAL_Snprintf(topic, topic_len, fmt, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);               // 格式化订阅主题字符串
//     res = IOT_MQTT_Subscribe(handle, topic, IOTX_MQTT_QOS0, example_message_arrive, NULL); // 执行消息订阅操作
//     if (res < 0)
//     {
//         EXAMPLE_TRACE("subscribe failed"); // 订阅失败的错误提示
//         HAL_Free(topic);                   // 释放分配的内存空间
//         return -1;
//     }

//     HAL_Free(topic); // 释放分配的内存空间
//     return 0;
// }

// void gate_publish()
// {
//     char *payload = NULL; // 存储消息负载的指针
//     int payload_len = 0;  // 消息负载的长度
//     char *topic = NULL;   // 存储发布主题的指针
//     int topic_len = 0;    // 发布主题的长度

//     // 计算消息负载的长度
//     payload_len = snprintf(NULL, 0, payload_fmt, environment_variable.flag, environment_variable.face_detection,
//                            environment_variable.face_ID, environment_variable.scan_QR_code, environment_variable.QR_content,
//                            environment_variable.regulation, environment_variable.GPT_ID) +
//                   1;

//     payload = HAL_Malloc(payload_len); // 分配内存用于存储消息负载
//     if (payload == NULL)
//     {
//         EXAMPLE_TRACE("memory not enough"); // 内存分配失败的错误提示
//         return -1;
//     }

//     snprintf(payload, payload_len, payload_fmt, environment_variable.flag, environment_variable.face_detection,
//              environment_variable.face_ID, environment_variable.scan_QR_code, environment_variable.QR_content,
//              environment_variable.regulation, environment_variable.GPT_ID); // 格式化消息负载字符串

//     // 计算发布主题的长度
//     topic_len = strlen("/a19sCsAuZcM/AI_model_1/user/pushMsg") + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;
//     topic = HAL_Malloc(topic_len); // 分配内存用于存储发布主题
//     if (topic == NULL)
//     {
//         HAL_Free(payload);
//         EXAMPLE_TRACE("memory not enough"); // 内存分配失败的错误提示
//         return -1;
//     }
//     memset(topic, 0, topic_len);                                                                                // 清空发布主题的内存空间
//     HAL_Snprintf(topic, topic_len, "/a19sCsAuZcM/AI_model_1/user/pushMsg", DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME); // 格式化发布主题字符串

//     int res = IOT_MQTT_Publish_Simple(NULL, topic, IOTX_MQTT_QOS1, payload, payload_len); // 执行消息发布操作
//     if (res < 0)
//     {
//         EXAMPLE_TRACE("publish failed"); // 发布失败的错误提示
//     }
//     else
//     {
//         EXAMPLE_TRACE("publish success"); // 发布成功的提示
//     }

//     HAL_Free(payload); // 释放分配的内存空间
//     HAL_Free(topic);   // 释放分配的内存空间

//     return 0;
// }

// // MQTT事件处理函数
// void example_event_handle(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
// {
//     EXAMPLE_TRACE("msg->event_type : %d", msg->event_type); // 打印事件类型

//     switch (msg->event_type)
//     {
//     case IOTX_MQTT_EVENT_UNDEF:
//         EXAMPLE_TRACE("msg->event_type : %d, IOTX_MQTT_EVENT_UNDEF", msg->event_type); // 未定义事件类型
//         break;
//     case IOTX_MQTT_EVENT_DISCONNECT:
//         EXAMPLE_TRACE("msg->event_type : %d, IOTX_MQTT_EVENT_DISCONNECT", msg->event_type); // MQTT断开连接事件
//         break;
//     case IOTX_MQTT_EVENT_RECONNECT:
//         EXAMPLE_TRACE("msg->event_type : %d, IOTX_MQTT_EVENT_RECONNECT", msg->event_type); // MQTT重新连接事件
//         break;
//     case IOTX_MQTT_EVENT_SUBCRIBE_SUCCESS:
//         EXAMPLE_TRACE("msg->event_type : %d, IOTX_MQTT_EVENT_SUBCRIBE_SUCCESS", msg->event_type); // MQTT订阅成功事件
//         break;
//     case IOTX_MQTT_EVENT_SUBCRIBE_TIMEOUT:
//         EXAMPLE_TRACE("msg->event_type : %d, IOTX_MQTT_EVENT_SUBCRIBE_TIMEOUT", msg->event_type); // MQTT订阅超时事件
//         break;
//     case IOTX_MQTT_EVENT_SUBCRIBE_NACK:
//         EXAMPLE_TRACE("msg->event_type : %d, IOTX_MQTT_EVENT_SUBCRIBE_NACK", msg->event_type); // MQTT订阅失败事件
//         break;
//     case IOTX_MQTT_EVENT_UNSUBCRIBE_SUCCESS:
//         EXAMPLE_TRACE("msg->event_type : %d, IOTX_MQTT_EVENT_UNSUBCRIBE_SUCCESS", msg->event_type); // MQTT取消订阅成功事件
//         break;
//     case IOTX_MQTT_EVENT_UNSUBCRIBE_TIMEOUT:
//         EXAMPLE_TRACE("msg->event_type : %d, IOTX_MQTT_EVENT_UNSUBCRIBE_TIMEOUT", msg->event_type); // MQTT取消订阅超时事件
//         break;
//     case IOTX_MQTT_EVENT_UNSUBCRIBE_NACK:
//         EXAMPLE_TRACE("msg->event_type : %d, IOTX_MQTT_EVENT_UNSUBCRIBE_NACK", msg->event_type); // MQTT取消订阅失败事件
//         break;
//     case IOTX_MQTT_EVENT_PUBLISH_SUCCESS:
//         EXAMPLE_TRACE("msg->event_type : %d, IOTX_MQTT_EVENT_PUBLISH_SUCCESS", msg->event_type); // MQTT消息发布成功事件
//         break;
//     case IOTX_MQTT_EVENT_PUBLISH_TIMEOUT:
//         EXAMPLE_TRACE("msg->event_type : %d, IOTX_MQTT_EVENT_PUBLISH_TIMEOUT", msg->event_type); // MQTT消息发布超时事件
//         break;
//     case IOTX_MQTT_EVENT_PUBLISH_NACK:
//         EXAMPLE_TRACE("msg->event_type : %d, IOTX_MQTT_EVENT_PUBLISH_NACK", msg->event_type); // MQTT消息发布失败事件
//         break;
//     case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
//         EXAMPLE_TRACE("msg->event_type : %d, IOTX_MQTT_EVENT_PUBLISH_RECEIVED", msg->event_type); // MQTT消息接收事件
//         // 调用消息到达处理函数
//         example_message_arrive(pcontext, pclient, msg);
//         break;
//     case IOTX_MQTT_EVENT_BUFFER_OVERFLOW:
//         EXAMPLE_TRACE("msg->event_type : %d, IOTX_MQTT_EVENT_BUFFER_OVERFLOW", msg->event_type); // MQTT缓冲区溢出事件
//         break;
//     }
// }

// // 在您的环境变量结构定义中添加以下字段：
// // char QR_content[512]; // Assuming QR_content can be a string

// // 修改您的扫码函数以包含发布操作：
// static void decode_task()
// {
//     if(ESP_OK != app_camera_init()) {
//         vTaskDelete(NULL);
//         return;
//     }
    
//     #ifdef LCD_CONTROLLER
//     int USE_LCD = 0;
//     scr_driver_t g_lcd;
//     if (ESP_OK == app_lcd_init(&g_lcd))
//         USE_LCD = 1;
//     #endif

//     camera_fb_t *fb = NULL;
//     int64_t time1, time2;
//     while (1)
//     {
//         fb = esp_camera_fb_get();
//         if(fb == NULL){
//             ESP_LOGI("APP_CODE_SCANNER", "camera get failed\n");
//             continue;
//         }

//         time1 = esp_timer_get_time();
//         // Decode Progress
//         esp_image_scanner_t *esp_scn = esp_code_scanner_create();
//         esp_code_scanner_config_t config = {ESP_CODE_SCANNER_MODE_FAST, ESP_CODE_SCANNER_IMAGE_RGB565, fb->width, fb->height};
//         esp_code_scanner_set_config(esp_scn, config);
//         int decoded_num = esp_code_scanner_scan_image(esp_scn, fb->buf);

//         if(decoded_num){
//             esp_code_scanner_symbol_t result = esp_code_scanner_result(esp_scn);
//             time2 = esp_timer_get_time();
//             ESP_LOGI("APP_CODE_SCANNER", "Decode time in %lld ms.", (time2 - time1) / 1000);
//             ESP_LOGI("APP_CODE_SCANNER", "Decoded %s symbol \"%s\"\n", result.type_name, result.data);

//             // Store the decoded data in the environment_variable struct
//             strncpy(environment_variable.face_ID, result.data, sizeof(environment_variable.face_ID) - 1);

//             // Publish the data
//             gate_publish();
//             vTaskDelay(100 / portTICK_PERIOD_MS);
//         }
//         esp_code_scanner_destroy(esp_scn);

//         #ifdef LCD_CONTROLLER
//         if(USE_LCD){
//             g_lcd.draw_bitmap(0, 0, fb->width, fb->height, (uint16_t *)fb->buf);
//         }
//         #endif

//         esp_camera_fb_return(fb);
//         vTaskDelay(10 / portTICK_PERIOD_MS);
//     }
// }


// // MQTT主函数
// void mqtt_main(void *pvParam)
// {
//     void *pclient = NULL;          // MQTT客户端句柄
//     int res = 0;                   // 返回结果变量
//     iotx_mqtt_param_t mqtt_params; // MQTT连接参数

//     // 获取产品认证参数
//     HAL_GetProductKey(DEMO_PRODUCT_KEY);
//     HAL_GetDeviceName(DEMO_DEVICE_NAME);
//     HAL_GetDeviceSecret(DEMO_DEVICE_SECRET);

//     EXAMPLE_TRACE("mqtt example"); // 打印示例信息

//     memset(&mqtt_params, 0x0, sizeof(mqtt_params));

//     mqtt_params.handle_event.h_fp = example_event_handle;

//     // 根据参数构建MQTT客户端
//     pclient = IOT_MQTT_Construct(&mqtt_params);
//     if (NULL == pclient)
//     {
//         EXAMPLE_TRACE("MQTT construct failed"); // MQTT构建失败
//         vTaskDelete(NULL);
//     }
    
//     // 订阅消息
//     res = example_subscribe(pclient);
//     if (res < 0)
//     {
//         // 连接失败，销毁客户端
//         IOT_MQTT_Destroy(&pclient);
//         vTaskDelete(NULL);
//     }

//     // 发布消息
//     // example_publish();
//     gate_publish();

//     while (1)
//     {
//         // 定时检测
//         IOT_MQTT_Yield(pclient, 1000);
//     }
//     vTaskDelete(NULL);
// }
// // 修改您的扫码函数以包含发布操作：
// static void decode_task()
// {
//     if(ESP_OK != app_camera_init()) {
//         vTaskDelete(NULL);
//         return;
//     }
    
//     #ifdef LCD_CONTROLLER
//     int USE_LCD = 0;
//     scr_driver_t g_lcd;
//     if (ESP_OK == app_lcd_init(&g_lcd))
//         USE_LCD = 1;
//     #endif

//     camera_fb_t *fb = NULL;
//     int64_t time1, time2;
//     while (1)
//     {
//         fb = esp_camera_fb_get();
//         if(fb == NULL){
//             ESP_LOGI("APP_CODE_SCANNER", "camera get failed\n");
//             continue;
//         }

//         time1 = esp_timer_get_time();
//         // Decode Progress
//         esp_image_scanner_t *esp_scn = esp_code_scanner_create();
//         esp_code_scanner_config_t config = {ESP_CODE_SCANNER_MODE_FAST, ESP_CODE_SCANNER_IMAGE_RGB565, fb->width, fb->height};
//         esp_code_scanner_set_config(esp_scn, config);
//         int decoded_num = esp_code_scanner_scan_image(esp_scn, fb->buf);

//         if(decoded_num){
//             esp_code_scanner_symbol_t result = esp_code_scanner_result(esp_scn);
//             time2 = esp_timer_get_time();
//             ESP_LOGI("APP_CODE_SCANNER", "Decode time in %lld ms.", (time2 - time1) / 1000);
//             ESP_LOGI("APP_CODE_SCANNER", "Decoded %s symbol \"%s\"\n", result.type_name, result.data);

//             // Store the decoded data in the environment_variable struct
//             strncpy(environment_variable.face_ID, result.data, sizeof(environment_variable.face_ID) - 1);

//             // Publish the data
//             gate_publish();
//             vTaskDelay(100 / portTICK_PERIOD_MS);
//         }
//         esp_code_scanner_destroy(esp_scn);

//         #ifdef LCD_CONTROLLER
//         if(USE_LCD){
//             g_lcd.draw_bitmap(0, 0, fb->width, fb->height, (uint16_t *)fb->buf);
//         }
//         #endif

//         esp_camera_fb_return(fb);
//         vTaskDelay(10 / portTICK_PERIOD_MS);
//     }
// }


// // MQTT主函数
// void mqtt_main(void *pvParam)
// {
//     void *pclient = NULL;          // MQTT客户端句柄
//     int res = 0;                   // 返回结果变量
//     iotx_mqtt_param_t mqtt_params; // MQTT连接参数

//     // 获取产品认证参数
//     HAL_GetProductKey(DEMO_PRODUCT_KEY);
//     HAL_GetDeviceName(DEMO_DEVICE_NAME);
//     HAL_GetDeviceSecret(DEMO_DEVICE_SECRET);

//     EXAMPLE_TRACE("mqtt example"); // 打印示例信息

//     memset(&mqtt_params, 0x0, sizeof(mqtt_params));

//     mqtt_params.handle_event.h_fp = example_event_handle;

//     // 根据参数构建MQTT客户端
//     pclient = IOT_MQTT_Construct(&mqtt_params);
//     if (NULL == pclient)
//     {
//         EXAMPLE_TRACE("MQTT construct failed"); // MQTT构建失败
//         vTaskDelete(NULL);
//     }
    
//     // 订阅消息
//     res = example_subscribe(pclient);
//     if (res < 0)
//     {
//         // 连接失败，销毁客户端
//         IOT_MQTT_Destroy(&pclient);
//         vTaskDelete(NULL);
//     }

//     // 发布消息
//     // example_publish();
//     gate_publish();

//     while (1)
//     {
//         // 定时检测
//         IOT_MQTT_Yield(pclient, 1000);
//     }
//     vTaskDelete(NULL);
// }