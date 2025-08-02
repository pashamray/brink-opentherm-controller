#include <Arduino.h>
#include <Ticker.h>
#include <EEPROM.h>

#include "OpenThermBrinkHelper.h"

#define OT_RX_PIN (21)
#define OT_TX_PIN (22)

OpenThermBrinkHelper brinkHelper(OT_RX_PIN, OT_TX_PIN);

struct VentilationSettings{
  unsigned int speed;
} ventSettings;

void IRAM_ATTR handleInterrupt()
{
    brinkHelper.handleInterrupt();
}

void brinkLoop()
{
  brinkHelper.setSpeed(ventSettings.speed);
  brinkHelper.loop();

  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

Ticker looper;

void setup() {
  EEPROM.begin(512);
  read_eeprom();

  pinMode(BUILTIN_LED, OUTPUT);

  Serial.begin(9600);
  Serial.println("Start");
  Serial.onReceive(serialReceive);

  brinkHelper.begin(handleInterrupt);
  looper.attach(1, brinkLoop);
}

void loop()
{
}

void read_eeprom() {
  EEPROM.get(0, ventSettings);

  Serial.println(ventSettings.speed);
}

void write_eeprom() {
  EEPROM.put(0, ventSettings);
  EEPROM.commit();
}

void serialReceive() 
{
  if (Serial.available() == 0) {
    return;
  }

  String command = Serial.readStringUntil('\n');
  
  if (command.equals("ping")) {
    Serial.println("pong");

    return;
  }

  if (command.startsWith("set")) {
    if (command.startsWith("speed", 4)) {
      ventSettings.speed = command.substring(9).toInt();

      write_eeprom();

      Serial.printf("set speed: %d", ventSettings.speed);
      Serial.println();

      return;
    }
  }

  if (command.startsWith("get")) {
    if (command.startsWith("speed", 4)) {
      Serial.printf("get speed: %d", ventSettings.speed);
      Serial.println();

      return;
    }
  }
}