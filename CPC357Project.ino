#include "DHT.h"  // 包含用于 DHT 传感器的库 // Include library for DHT sensor
#include "alarm.h"
#include "mynetwork.h"
#include "RainMonitor.h"
#include "SoilMoistureMonitor.h"
#include "SharedResources.h"

#define DHTTYPE DHT11  // 定义传感器类型为 DHT11 // Define the sensor type as DHT11

const int dht11Pin = 42;
const int rainPin = 5;
const int moisturePin = 4;

const int MinMoistureValue = 5000;
const int MaxMoistureValue = 2000;

char buffer[128] = "";       // 创建一个文本缓冲区，用于存储消息 // Create a text buffer for storing messages
DHT dht(dht11Pin, DHTTYPE);  // 初始化 DHT 传感器 // Initialize the DHT sensor

const bool enableNetwork = false;

void setup() {
  Serial.begin(115200);
  dht.begin();  // 初始化 DHT 传感器 // Initialize the DHT sensor

  initSharedResources();
  RainMonitor::setupRainMonitor(rainPin);                                                          // 初始化雨监控 // Initialize rain monitoring
  SoilMoistureMonitor::setupSoilMoistureMonitor(moisturePin, MinMoistureValue, MaxMoistureValue);  // 初始化土壤湿度监控 // Initialize soil moisture monitoring
  network::setup_network();
}

void loop() {
  delay(3000);
  float temperature = dht.readTemperature();  // 从 DHT 传感器读取温度值 // Read temperature value from the DHT sensor
  float humidity = dht.readHumidity();

  sprintf(buffer, "Temperature: %.2f degree Celsius, Humidity: %.2f%%", temperature, humidity);
  Serial.println(buffer);  // 打印发布的温度数据 // Print the published temperature data
}
