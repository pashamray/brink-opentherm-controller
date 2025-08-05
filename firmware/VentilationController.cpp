#include "VentilationController.h"

VentilationController::VentilationController(AppConfig& cfg, uint8_t rxPin, uint8_t txPin)
: config(cfg), rx(rxPin), tx(txPin), helper(rxPin, txPin) {}

void VentilationController::begin(void (*handleInterruptCallback)(void)) {
  helper.begin(handleInterruptCallback);
}

void VentilationController::loop() {
  helper.setSpeed(config.ventilation.speed);
  helper.loop();
}

void VentilationController::handleInterrupt() {
  helper.handleInterrupt();
}