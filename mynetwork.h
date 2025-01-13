#pragma once

#include <PubSubClient.h> // 包含用于 MQTT 客户端的库 // Include library for MQTT client
#include <WiFi.h> // 包含用于 ESP32 WiFi 功能的库 // Include library for ESP32 WiFi functionality

namespace network{
  extern const char* WIFI_SSID;
  extern const char* WIFI_PASSWORD;
  extern const char* MQTT_SERVER;
  extern const char* MQTT_TOPIC;
  extern const int MQTT_PORT;

  extern WiFiClient espClient; // 声明 WiFi 客户端对象
  extern PubSubClient client;  // 声明 MQTT 客户端对象

  void setup_wifi(void *parameter);
  void reconnect();
  void setup_network();
  void mqtt_publish(const char* MQTT_TOPIC, const char* buffer);
  
}
