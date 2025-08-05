#pragma once
#include <Arduino.h>
#include <Preferences.h>

struct WifiSettings {
  String ssid;
  String pass;
};

struct VentilationSettings {
  unsigned int speed;
};

class AppConfig {
public:
  WifiSettings wifi;
  VentilationSettings ventilation;

  bool load();
  bool save();

private:
  const char* ns = "my-app";
  Preferences prefs;
};