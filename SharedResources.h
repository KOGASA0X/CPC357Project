#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>

// 声明互斥量 Declare the mutex
extern SemaphoreHandle_t serialMutex;
void initSharedResources();
