#include "SoilMoistureMonitor.h"
#include "alarm.h"  // 引入报警模块

namespace SoilMoistureMonitor {

TaskHandle_t soilMoistureTaskHandle = NULL;  // 任务句柄
int moisturePin = -1;                        // 土壤湿度传感器引脚
int minMoistureValue = 5000;                 // 默认最小湿度值
int maxMoistureValue = 2000;                 // 默认最大湿度值

// 土壤湿度监控任务
void soilMoistureTask(void *parameter) {
  while (true) {
    // 读取并计算湿度百分比
    int moistureValue = analogRead(moisturePin);
    int moisture = map(moistureValue, minMoistureValue, maxMoistureValue, 0, 100);

    // 打印土壤湿度
    Serial.print("Soil Moisture: ");
    Serial.print(moisture);
    Serial.println("%");

    // 如果湿度超过 100%，触发报警
    if (moisture > 100) {
      Serial.println("Warning: Soil moisture exceeds 100%!");
      alarmPlay::startAlarm();
    } else {
      if (alarmPlay::isAlarmRunning()) {
        alarmPlay::stopAlarm();
      }
    }

    // 延迟 1 秒
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// 初始化土壤湿度监控
void setupSoilMoistureMonitor(int pin, int minVal, int maxVal) {
  moisturePin = pin;
  minMoistureValue = minVal;
  maxMoistureValue = maxVal;

  pinMode(moisturePin, INPUT);

  // 创建土壤湿度监控任务
  xTaskCreate(
    soilMoistureTask,        // 任务函数
    "Soil Moisture Task",    // 任务名称
    1000,                    // 栈大小
    NULL,                    // 参数
    1,                       // 优先级
    &soilMoistureTaskHandle  // 保存任务句柄
  );
}

void startSoilMoistureTask() {
  Serial.println("Soil Moisture Monitoring Task is running...");
}

}
