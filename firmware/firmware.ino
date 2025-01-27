#include <Arduino.h>
// https://github.com/ihormelnyk/opentherm_library
#include <OpenTherm.h>
#include "OpenThermBrinkHelper.h"

#define OT_RX_PIN (21)
#define OT_TX_PIN (22)

OpenTherm ot(OT_RX_PIN, OT_TX_PIN);
OpenThermBrinkHelper brinkHelper(&ot);

unsigned int current_speed = 0; // presets 1,2,3 
unsigned int current_timer = 0;

void IRAM_ATTR handleInterrupt()
{
    ot.handleInterrupt();
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);

  ot.begin(handleInterrupt);

  Serial.begin(9600);
  Serial.println("Start");
  Serial.onReceive(serialReceive);
}

void loop()
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

  brinkHelper.loop();

  delay(1000);
}

void serialReceive() 
{
  if (Serial.available() == 0) {
    return;
  }

  String command = Serial.readStringUntil('\n');
  
  if (command.equals("ping")) {
    Serial.println("pong");
  }

  if (command.startsWith("set")) {
    if (command.startsWith("speed", 4)) {
      unsigned int speed = command.substring(9).toInt();

      brinkHelper.setSpeed(speed);

      current_speed = speed;

      Serial.printf("set speed: %d", speed);
      Serial.println();

      return;
    }

    if (command.startsWith("timer", 4)) {
      current_timer = command.substring(9).toInt();

      Serial.printf("set timer: %d", current_timer);
      Serial.println();
    }
  }

  if (command.startsWith("get")) {
    if (command.startsWith("speed", 4)) {
      unsigned int speed = brinkHelper.getSpeed();

      Serial.printf("get speed: %d", speed);
      Serial.println();

      return;
    }

    if (command.startsWith("timer", 4)) {
      Serial.printf("get timer: %d", current_timer);
      Serial.println();
    }
  }
}