#include "SharedResources.h"

// 定义互斥量 Define the mutex
SemaphoreHandle_t serialMutex = NULL;

// 初始化互斥量的函数 Function to initialize the mutex
void initSharedResources() {
  serialMutex = xSemaphoreCreateMutex();
  if (serialMutex == NULL) {
    Serial.println("Failed to create shared mutex!");
  }
}
