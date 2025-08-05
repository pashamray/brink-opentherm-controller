#pragma once
#include <WiFi.h>
#include "AppConfig.h"

class WifiManager {
public:
  WifiManager(AppConfig& cfg);
  bool connect();
  IPAddress getIP() const;
private:
  AppConfig& config;
};