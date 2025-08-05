#pragma once
#include <OpenThermBrinkHelper.h>
#include "AppConfig.h"

class VentilationController {
public:
  VentilationController(AppConfig& cfg, uint8_t rxPin, uint8_t txPin);
  void begin(void (*handleInterruptCallback)(void));
  void loop();
  void handleInterrupt();

private:
  AppConfig& config;
  OpenThermBrinkHelper helper;
  uint8_t rx, tx;
};