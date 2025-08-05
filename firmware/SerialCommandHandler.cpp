#include "SerialCommandHandler.h"

SerialCommandHandler::SerialCommandHandler(AppConfig& cfg): config(cfg) {}

void SerialCommandHandler::begin() {
  Serial.begin(9600);

  Serial.onReceive([this]() {
    serialReceive();
  });

  Serial.println("SerialCommandHandler");
}

void SerialCommandHandler::serialReceive() 
{
  if (Serial.available() == 0) {
    return;
  }

  String command = Serial.readStringUntil('\n');

  handleCommand(command);
}

void SerialCommandHandler::handleCommand(const String& cmd) {
  if (cmd == "ping") {
    Serial.println("pong");

    return;
  }

  if (cmd == "restart") {
    Serial.println("Restart...");
    ESP.restart();

    return;
  }

  if (cmd.startsWith("set speed")) {
    int value = cmd.substring(9).toInt();
    config.ventilation.speed = value;
    config.save();
    Serial.printf("set speed: %d\n", value);

    return;
  }

  if (cmd.startsWith("set wifi")) {
    String data = cmd.substring(9);
    int sep = data.indexOf('@');
    if (sep > 0) {
      config.wifi.ssid = data.substring(0, sep);
      config.wifi.pass = data.substring(sep + 1);
      config.save();
      Serial.println("set wifi:");
      Serial.println("\tssid: " + config.wifi.ssid);
      Serial.println("\tpass: " + config.wifi.pass);

      return;
    }
  }

  if (cmd == "get speed") {
    Serial.printf("get speed: %d\n", config.ventilation.speed);

    return;
  }

  if (cmd == "get wifi") {
    Serial.println("get wifi:");
    Serial.println("\tssid: " + config.wifi.ssid);

    return;
  }

  Serial.println("Unknown command");
}