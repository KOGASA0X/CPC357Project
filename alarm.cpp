#include "alarm.h"

namespace alarmPlay{
  int buzzerPin = 12;
  TaskHandle_t alarmTaskHandle = NULL;

  void playMidi(int pin, const int notes[][3], size_t len){
  for (int i = 0; i < len; i++) {
      tone(pin, notes[i][0]);
      delay(notes[i][1]);
      noTone(pin);
      delay(notes[i][2]);
    }
  }

  void playAlarm(int buzzerPin){
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
    while(true){
      playAlarm(buzzerPin);
    }
  }
}
