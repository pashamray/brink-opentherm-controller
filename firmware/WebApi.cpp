#include "WebApi.h"

WebApi::WebApi(AppConfig& cfg) : server(80), config(cfg) {}

void WebApi::begin() {
  server.on("/", [this]() {
    server.send(200, "text/plain", "hello");
  });

  server.on("/ping", [this]() {
    server.send(200, "text/plain", "pong");
  });

  server.on("/api/speed", HTTP_GET, [this]() {
    server.send(200, "text/plain", "get speed: " + String(config.ventilation.speed));
  });

  server.on("/api/speed", HTTP_POST, [this]() {
    if (server.argName(0) != "value") {
      server.send(400, "text/plain", "Missing 'value'");
      return;
    }

    config.ventilation.speed = server.arg(0).toInt();
    config.save();

    server.send(200, "text/plain", "set speed: " + String(config.ventilation.speed));
  });

  server.begin();
}

void WebApi::handleClient() {
  server.handleClient();
}