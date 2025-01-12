#include "RainMonitor.h"

namespace RainMonitor {
    unsigned long rainDuration = 0;  // 累计下雨时间
    int rainPin = -1;               // 雨感应引脚
    unsigned long startTime = 0;    // 下雨开始时间
    bool isRaining = false;         // 当前是否下雨

    // FreeRTOS 任务：监控雨感应引脚状态
    void monitorRainTask(void *parameter) {
        while (true) {
            int raining = !digitalRead(rainPin); // 雨感应状态

            if (raining && !isRaining) {
                // 开始下雨，记录起始时间
                startTime = millis();
                isRaining = true;
            } else if (!raining && isRaining) {
                // 停止下雨，累计时间
                rainDuration = millis() - startTime;
                isRaining = false;

                // 输出累计时间
                Serial.print("Rain Duration: ");
                Serial.print(rainDuration);
                Serial.println(" ms");
            }

            vTaskDelay(pdMS_TO_TICKS(1000)); // 每 100 ms 检查一次状态
        }
    }

    // 初始化雨监控
    void setupRainMonitor(int pin) {
        rainPin = pin;
        pinMode(rainPin, INPUT);

        // 初始化任务
        xTaskCreate(
            monitorRainTask,     // 任务函数
            "Monitor Rain Task", // 任务名称
            1000,                // 栈大小
            NULL,                // 无参数
            1,                   // 优先级
            NULL                 // 无需任务句柄
        );
    }

    // 启动雨监控任务（备用）
    void startRainMonitorTask() {
        Serial.println("Rain Monitor Task is running...");
    }
}
