#pragma once

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace RainMonitor {
    extern unsigned long rainDuration; // 累计下雨时间 (ms)
    extern int rainPin;                // 雨感应引脚

    void setupRainMonitor(int pin);    // 初始化雨监控
    void startRainMonitorTask();       // 启动雨监控任务
}