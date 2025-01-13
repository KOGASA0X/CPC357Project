#include "mynetwork.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace network {
const char *WIFI_SSID = "0x0";              // 定义 WiFi SSID // Define the WiFi SSID
const char *WIFI_PASSWORD = "133064869";    // 定义 WiFi 密码 // Define the WiFi password
const char *MQTT_SERVER = "34.44.247.111";  // 定义 MQTT 服务器的 IP 地址 // Define the IP address of the MQTT server
const int MQTT_PORT = 1883;                 // 定义 MQTT 的非 TLS 通信端口 // Define the non-TLS communication port for MQTT

WiFiClient espClient;            // 创建 WiFi 客户端对象 // Create a WiFi client object
PubSubClient client(espClient);  // 创建 MQTT 客户端对象，使用 WiFi 客户端连接 // Create an MQTT client object using the WiFi client

TaskHandle_t wifiTaskHandle = NULL;

void setup_wifi(void *parameter) {
  delay(10);                       // 延迟 10 毫秒，用于初始化 // Delay for 10 milliseconds for initialization
  Serial.println();                // 打印空行，改善输出格式 // Print an empty line for better output formatting
  Serial.print("Connecting to ");  // 打印正在连接的提示 // Print "Connecting to" message
  Serial.println(WIFI_SSID);       // 打印 WiFi SSID // Print the WiFi SSID

  WiFi.mode(WIFI_STA);                   // 将 WiFi 模式设置为 STA（客户端模式） // Set WiFi mode to STA (Station mode)
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  // 开始连接 WiFi // Start connecting to WiFi

  while (WiFi.status() != WL_CONNECTED) {  // 循环检查 WiFi 连接状态 // Loop to check WiFi connection status
    delay(5000);                            // 每 500 毫秒检查一次 // Check every 500 milliseconds
    Serial.print("WiFi connecting...");                     // 打印连接进度的点 // Print a dot to show connection progress
  }

  Serial.println("");                // 打印空行，改善输出格式 // Print an empty line for better output formatting
  Serial.println("WiFi connected");  // 打印 WiFi 已连接提示 // Print WiFi connected message
  Serial.print("IP address: ");      // 打印 IP 地址提示 // Print IP address prompt
  Serial.println(WiFi.localIP());    // 打印分配给 ESP32 的 IP 地址 // Print the IP address assigned to ESP32
}

void reconnect() {

  Serial.println("Attempting MQTT connection...");  // 打印尝试连接的提示 // Print a message about attempting MQTT connection

  if (client.connect("ESP32Client")) {           // 尝试连接到 MQTT 服务器 // Attempt to connect to the MQTT server
    Serial.println("Connected to MQTT server");  // 如果连接成功，打印成功提示 // Print success message if connected
  }

  else {                           // 如果连接失败 // If the connection fails
    Serial.print("Failed, rc=");   // 打印失败原因 // Print the failure reason
    Serial.print(client.state());  // 打印客户端的状态代码 // Print the client's state code
  }
}

void setup_network() {
  xTaskCreate(
    setup_wifi,      // 任务函数
    "WiFi Task",     // 任务名称
    4096,            // 任务堆栈大小
    NULL,            // 任务参数
    1,               // 任务优先级
    &wifiTaskHandle  // 任务句柄
  );
  client.setServer(MQTT_SERVER, MQTT_PORT);  // 设置 MQTT 服务器和端口 // Set the MQTT server and port
}

void mqtt_publish(const char *MQTT_TOPIC, const char *buffer) {
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {  // 检查 MQTT 客户端是否已连接 // Check if the MQTT client is connected
      reconnect();              // 如果未连接，调用 reconnect 函数重新连接 // Call reconnect function if not connected
    } else {
      client.loop();                       // 处理 MQTT 客户端的后台任务 // Process MQTT client's background tasks
      client.publish(MQTT_TOPIC, buffer);  // 发布温度数据到指定的 MQTT 主题 // Publish temperature data to the specified MQTT topic
    }
  }
}
}
