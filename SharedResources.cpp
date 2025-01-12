#include "SharedResources.h"

// 定义互斥量
SemaphoreHandle_t serialMutex = NULL;

// 初始化互斥量的函数
void initSharedResources() {
  serialMutex = xSemaphoreCreateMutex();
  if (serialMutex == NULL) {
    Serial.println("Failed to create shared mutex!");
  }
}
