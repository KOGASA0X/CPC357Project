#pragma once

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace SoilMoistureMonitor {
    extern TaskHandle_t soilMoistureTaskHandle; // 任务句柄
    extern int moisturePin;                    // 土壤湿度传感器引脚
    extern int minMoistureValue;               // 最小湿度值（校准值）
    extern int maxMoistureValue;               // 最大湿度值（校准值）

    void setupSoilMoistureMonitor(int pin, int minVal, int maxVal); // 初始化
    void startSoilMoistureTask();                                   // 启动任务
    void soilMoistureTask(void *parameter);                         // 任务函数
}
