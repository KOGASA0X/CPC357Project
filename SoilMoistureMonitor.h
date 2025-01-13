#pragma once

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace SoilMoistureMonitor {
    extern TaskHandle_t soilMoistureTaskHandle; // 任务句柄 Task handle
    extern int moisturePin;                    // 土壤湿度传感器引脚 Soil moisture sensor pin
    extern int minMoistureValue;               // 最小湿度值（校准值） Minimum moisture value (calibrated value)
    extern int maxMoistureValue;               // 最大湿度值（校准值） Maximum moisture value (calibrated value)

    void setupSoilMoistureMonitor(int pin, int minVal, int maxVal); 
    void startSoilMoistureTask();                                   
    void soilMoistureTask(void *parameter);                        
}
