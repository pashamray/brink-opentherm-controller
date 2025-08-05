#pragma once
#include <WebServer.h>
#include "AppConfig.h"

class WebApi {
public:
  WebApi(AppConfig& cfg);
  void begin();
  void handleClient();

private:
  WebServer server;
  AppConfig& config;
};