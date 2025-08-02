#include <Arduino.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <WiFi.h>

#include "OpenThermBrinkHelper.h"

#define OT_RX_PIN (21)
#define OT_TX_PIN (22)

OpenThermBrinkHelper brinkHelper(OT_RX_PIN, OT_TX_PIN);

struct VentilationSettings{
  unsigned int speed;
} ventSettings;

struct WifiSettings{
  char ssid[50];
  char pass[50];
} wifiSettings;

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

  // connect to WiFi
  WiFi.begin(wifiSettings.ssid, wifiSettings.pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("Connected to WiFi!");
  Serial.println(WiFi.localIP());
}

void loop()
{
}

void read_eeprom() {
  EEPROM.get(0, ventSettings);
  EEPROM.get(sizeof(ventSettings), wifiSettings);
}

void write_eeprom() {
  EEPROM.put(0, ventSettings);
  EEPROM.put(sizeof(ventSettings), wifiSettings);

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

    if (command.startsWith("wifi", 4)) {
      String wifiDsn = command.substring(8);
      String wifiSsid = wifiDsn.substring(1, wifiDsn.indexOf('@'));
      String wifiPass = wifiDsn.substring(wifiDsn.indexOf('@') + 1);
      
      wifiSsid.toCharArray(wifiSettings.ssid, sizeof(wifiSettings.ssid));
      wifiPass.toCharArray(wifiSettings.pass, sizeof(wifiSettings.pass));
      
      write_eeprom();
      
      Serial.print("set wifi");
      Serial.println();
      
      Serial.printf("\tssid: %s", wifiSettings.ssid);
      Serial.println();
      
      Serial.printf("\tpass: %s", wifiSettings.pass);
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

    if (command.startsWith("wifi", 4)) {
      Serial.print("get wifi");
      Serial.println();

      Serial.printf("\tssid: %s", wifiSettings.ssid);
      Serial.println();
    }
  }
}