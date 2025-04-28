#include <Arduino.h>
#include <Ticker.h>

#include "OpenThermBrinkHelper.h"

#define OT_RX_PIN (21)
#define OT_TX_PIN (22)

OpenThermBrinkHelper brinkHelper(OT_RX_PIN, OT_TX_PIN);

void IRAM_ATTR handleInterrupt()
{
    brinkHelper.handleInterrupt();
}

void brinkLoop()
{
  brinkHelper.loop();
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

Ticker looper;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);

  brinkHelper.begin(handleInterrupt);
  looper.attach(1, brinkLoop);

  Serial.begin(9600);
  Serial.println("Start");
  Serial.onReceive(serialReceive);
}

void loop()
{
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

      Serial.printf("set speed: %d", speed);
      Serial.println();

      return;
    }
  }

  if (command.startsWith("get")) {
    if (command.startsWith("speed", 4)) {
      unsigned int speed = brinkHelper.getSpeed();

      Serial.printf("get speed: %d", speed);
      Serial.println();

      return;
    }
  }
}