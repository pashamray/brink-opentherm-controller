#include "AppConfig.h"

bool AppConfig::load() {
  if (!prefs.begin(ns, true)) return false;

  ventilation.speed = prefs.getUInt("vent.speed", 1);
  wifi.ssid = prefs.getString("wifi.ssid", "defaultSSID");
  wifi.pass = prefs.getString("wifi.pass", "defaultPass");

  prefs.end();

  return true;
}

bool AppConfig::save() {
  if (!prefs.begin(ns, false)) return false;

  prefs.putUInt("vent.speed", ventilation.speed);
  prefs.putString("wifi.ssid", wifi.ssid);
  prefs.putString("wifi.pass", wifi.pass);

  prefs.end();

  return true;
}