#include "RainMonitor.h"
#include "SharedResources.h"
#include "mynetwork.h"

namespace RainMonitor {
unsigned long rainDuration = 0;  // 累计下雨时间 Time of rain (ms)
int rainPin = -1;                // 雨感应引脚 Rain sensor pin
unsigned long startTime = 0;     // 下雨开始时间 Start time of rain
bool isRaining = false;          // 当前是否下雨 Is it raining now?
char buffer[128] = ""; 

// FreeRTOS 任务：监控雨感应引脚状态 Task to monitor the rain sensor pin status
void monitorRainTask(void *parameter) {
  while (true) {
    int raining = !digitalRead(rainPin);  // 雨感应状态 Rain sensor status
    if (xSemaphoreTake(serialMutex, portMAX_DELAY)) {
        sprintf(buffer, "[Rain]%d", raining);
        network::mqtt_publish("iot/devices/MH-RD",buffer);

        sprintf(buffer, "Raining: %d", raining);
        Serial.println(buffer);

      xSemaphoreGive(serialMutex);  // 释放互斥量 Release the mutex
    }


    if (raining && !isRaining) {
      // 开始下雨，记录起始时间 Start raining, record start time
      startTime = millis();
      isRaining = true;
    } else if (!raining && isRaining) {
      // 停止下雨，累计时间 Stop raining, accumulate time
      rainDuration = millis() - startTime;
      isRaining = false;

      // 输出累计时间 Output accumulated time
      if (xSemaphoreTake(serialMutex, portMAX_DELAY)) {
        sprintf(buffer, "[RainDuration]%lu", rainDuration);
        network::mqtt_publish("iot/devices/MH-RD",buffer);

        sprintf(buffer, "Rain Duration: %lu ms", rainDuration);
        Serial.println(buffer);

        xSemaphoreGive(serialMutex);  // 释放互斥量 Release the mutex
      }
    }

    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

// 初始化雨监控 Initialize rain monitoring
void setupRainMonitor(int pin) {
  rainPin = pin;
  pinMode(rainPin, INPUT);

  // 初始化任务 Initialize task
  xTaskCreate(
    monitorRainTask,      // 任务函数 Task function
    "Monitor Rain Task",  // 任务名称 Task name
    4096,                 // 栈大小 Stack size
    NULL,                 // 无参数 No parameter
    1,                    // 优先级 Priority
    NULL                  // 无需任务句柄 No task handle
  );
}

// 启动雨监控任务（备用） Start rain monitoring task (backup)
void startRainMonitorTask() {
  Serial.println("Rain Monitor Task is running...");
}
}
