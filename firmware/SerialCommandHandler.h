#pragma once
#include "AppConfig.h"
#include <HardwareSerial.h>

class SerialCommandHandler {
public:
  SerialCommandHandler(AppConfig& cfg);
  void begin();

private:
  AppConfig& config;
  void serialReceive();
  void handleCommand(const String& cmd);
};