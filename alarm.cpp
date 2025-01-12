#include "alarm.h"

namespace alarmPlay {
int buzzerPin = 12;
TaskHandle_t alarmTaskHandle = NULL;

void playMidi(int pin, const int notes[][3], size_t len) {
  for (int i = 0; i < len; i++) {
    tone(pin, notes[i][0]);
    delay(notes[i][1]);
    noTone(pin);
    delay(notes[i][2]);
  }
}

void playAlarm(int buzzerPin) {
  tone(buzzerPin, 1000);
  delay(200);
  noTone(buzzerPin);
  delay(100);

  tone(buzzerPin, 500);
  delay(200);
  noTone(buzzerPin);
  delay(100);
}

void playAlarmTask(void *parameter) {
  while (true) {
    playAlarm(buzzerPin);
  }
}

void setupAlarm(int pin) {
  buzzerPin = pin;
  pinMode(buzzerPin, OUTPUT);
}
void startAlarm() {
  if (alarmTaskHandle == NULL) {
    xTaskCreate(
      playAlarmTask,    // 任务函数
      "Alarm Task",     // 任务名称
      1000,             // 栈大小
      NULL,             // 参数
      1,                // 优先级
      &alarmTaskHandle  // 保存任务句柄
    );
    Serial.println("Alarm started.");
  } else {
    Serial.println("Alarm is already running.");
  }
}
void stopAlarm() {
  if (alarmTaskHandle != NULL) {
    vTaskDelete(alarmTaskHandle);  // 删除任务
    alarmTaskHandle = NULL;
    noTone(buzzerPin);  // 确保蜂鸣器停止
    Serial.println("Alarm stopped.");
  } else {
    Serial.println("Alarm is not running.");
  }
}
bool isAlarmRunning() {
  if (alarmTaskHandle != NULL) {
    eTaskState state = eTaskGetState(alarmTaskHandle);  // 获取任务状态
    return (state == eRunning || state == eReady);      // 判断任务是否处于运行或就绪状态
  }
  return false;  // 如果任务句柄为空，任务未运行
}

}
