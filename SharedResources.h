#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>

// 声明互斥量
extern SemaphoreHandle_t serialMutex;
void initSharedResources();
