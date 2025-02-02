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
      playAlarmTask,    // 任务函数 Task function
      "Alarm Task",     // 任务名称 Task name
      1000,             // 栈大小 Stack size
      NULL,             // 参数 Parameter
      1,                // 优先级 Priority
      &alarmTaskHandle  // 保存任务句柄 Save the task handle
    );
    Serial.println("Alarm started.");
  } else {
    Serial.println("Alarm is already running.");
  }
}
void stopAlarm() {
  if (alarmTaskHandle != NULL) {
    vTaskDelete(alarmTaskHandle);  // 删除任务 Delete the task
    alarmTaskHandle = NULL;
    noTone(buzzerPin);  // 确保蜂鸣器停止 Ensure the buzzer stops
    Serial.println("Alarm stopped.");
  } else {
    Serial.println("Alarm is not running.");
  }
}
bool isAlarmRunning() {
  if (alarmTaskHandle != NULL) {
    return true;
  }
  return false;  // 如果任务句柄为空，任务未运行 If the task handle is empty, the task is not running
}

}
