#include "SoilMoistureMonitor.h"
#include "alarm.h"
#include "SharedResources.h"
#include "mynetwork.h"

namespace SoilMoistureMonitor {

TaskHandle_t soilMoistureTaskHandle = NULL;  // 任务句柄 Task handle
int moisturePin = -1;                        // 土壤湿度传感器引脚 Soil moisture sensor pin
int minMoistureValue = 5000;                 // 默认最小湿度值 // Default minimum humidity value
int maxMoistureValue = 2000;                 // 默认最大湿度值 // Default maximum humidity value
char buffer[128] = "";

// 土壤湿度监控任务 Soil moisture monitoring task
void soilMoistureTask(void *parameter) {
  while (true) {
    // 读取并计算湿度百分比 Read and calculate humidity percentage
    int moistureValue = analogRead(moisturePin);
    int moisture = map(moistureValue, minMoistureValue, maxMoistureValue, 0, 100);

    // 打印土壤湿度 Print soil moisture
    if (xSemaphoreTake(serialMutex, portMAX_DELAY)) {
      sprintf(buffer, "[SoilMoisture]%d", moisture);
      network::mqtt_publish("iot/devices/MH-Moisture", buffer);

      sprintf(buffer, "Soil Moisture: %d%%", moisture);
      Serial.println(buffer);

      xSemaphoreGive(serialMutex);  // 释放互斥量 Release mutex
    }

    // 如果湿度超过 100%，触发报警 If the humidity exceeds 100%, trigger an alarm
    if (moisture > 100) {
      if (xSemaphoreTake(serialMutex, portMAX_DELAY)) {
        Serial.println("Warning: Soil moisture exceeds 100%!");

        xSemaphoreGive(serialMutex);  // 释放互斥量 Release mutex
      }
      alarmPlay::startAlarm();
    } else {
      if (alarmPlay::isAlarmRunning()) {
        alarmPlay::stopAlarm();
      }
    }

    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

// 初始化土壤湿度监控 Initialize soil moisture monitoring
void setupSoilMoistureMonitor(int pin, int minVal, int maxVal) {
  moisturePin = pin;
  minMoistureValue = minVal;
  maxMoistureValue = maxVal;

  pinMode(moisturePin, INPUT);

  // 创建土壤湿度监控任务 Create soil moisture monitoring task
  xTaskCreate(
    soilMoistureTask,        // 任务函数 Task function
    "Soil Moisture Task",    // 任务名称 Task name
    4096,                    // 栈大小  Stack size
    NULL,                    // 参数 Parameters
    1,                       // 优先级 Priority
    &soilMoistureTaskHandle  // 保存任务句柄 Save task handle
  );
}

void startSoilMoistureTask() {
  Serial.println("Soil Moisture Monitoring Task is running...");
}

}
