#include "WifiManager.h"

WifiManager::WifiManager(AppConfig& cfg) : config(cfg) {}

bool WifiManager::connect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(config.wifi.ssid.c_str(), config.wifi.pass.c_str());

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    attempts++;
  }

  return WiFi.status() == WL_CONNECTED;
}

IPAddress WifiManager::getIP() const {
  return WiFi.localIP();
}