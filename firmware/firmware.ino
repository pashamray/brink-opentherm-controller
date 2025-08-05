#include "AppConfig.h"
#include "VentilationController.h"
#include "WifiManager.h"
#include "WebApi.h"
#include "SerialCommandHandler.h"
#include <Ticker.h>

AppConfig config;
VentilationController ventilation(config, 21, 22);
WifiManager wifi(config);
WebApi web(config);
SerialCommandHandler serialHandler(config);

Ticker ventilationTicker;
Ticker webServerTicker;

void IRAM_ATTR handleInterrupt()
{
  ventilation.handleInterrupt();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  config.load();
  serialHandler.begin();
  ventilation.begin(handleInterrupt);

  ventilationTicker.attach(1.0, []() {
    ventilation.loop();

    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  });

  Serial.println("");

  if (wifi.connect() == false) {
    Serial.println("Wifi not connected");

    return;
  }

  Serial.print("Wifi connected, ");
  Serial.println("IP: " + wifi.getIP().toString());

  web.begin();
  webServerTicker.attach(0.5, []() {
    web.handleClient();
  });

  Serial.println("Web server started");
}

void loop() {
}