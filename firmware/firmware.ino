#include <Arduino.h>
#include <Ticker.h>
#include <WiFi.h>
#include <Preferences.h>
#include <WebServer.h>

#include "OpenThermBrinkHelper.h"

#define OT_RX_PIN (21)
#define OT_TX_PIN (22)

OpenThermBrinkHelper brinkHelper(OT_RX_PIN, OT_TX_PIN);

struct VentilationSettings{
  unsigned int speed;
} ventSet;

struct WifiSettings{
  String ssid;
  String pass;
} wifiSet;

Preferences preferences;

WebServer server(80);

void IRAM_ATTR handleInterrupt()
{
    brinkHelper.handleInterrupt();
}

void brinkLoop()
{
  brinkHelper.setSpeed(ventSet.speed);
  brinkHelper.loop();

  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

Ticker brinkHelperLooper;
Ticker webServerLooper;

void setup() {
  read_preferences();

  pinMode(BUILTIN_LED, OUTPUT);

  Serial.begin(9600);
  Serial.println("Start");
  Serial.onReceive(serialReceive);

  brinkHelper.begin(handleInterrupt);
  brinkHelperLooper.attach(1, brinkLoop);

  // connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSet.ssid, wifiSet.pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("Connected to WiFi!");
  Serial.println(WiFi.localIP());

  // web server
  server.on("/", []() {
    server.send(200, "text/plain", "hello");
  });
  server.on("/ping", []() {
    server.send(200, "text/plain", "pong");
  });
  server.on("/api/speed", HTTP_GET, []() {
    server.send(200, "text/plain", "get speed: " + String(ventSet.speed));
  });
  server.on("/api/speed", HTTP_POST, []() {
    if (server.argName(0) != "value") {
      return;
    }

    ventSet.speed = server.arg(0).toInt();
    write_preferences();

    server.send(200, "text/plain", "set speed: " + String(ventSet.speed));
  });

  server.begin();
  webServerLooper.attach(1, webserverHandle);
  Serial.println("WEB server started");
}

void loop()
{
}

void read_preferences() {
  if (preferences.begin("my-app", false) == false) {
    Serial.println("Preferences open failed.");

    return;
  }

  ventSet.speed = preferences.getUInt("ventSet.speed", 1);

  wifiSet.ssid = preferences.getString("wifiSet.ssid", "ssid");
  wifiSet.pass = preferences.getString("wifiSet.pass", "pass");

  // Close the Preferences
  preferences.end();
}

void write_preferences() {
  if (preferences.begin("my-app", false) == false) {
    Serial.println("Preferences open failed.");

    return;
  }

  preferences.putUInt("ventSet.speed", ventSet.speed);

  preferences.putString("wifiSet.ssid", wifiSet.ssid);
  preferences.putString("wifiSet.pass", wifiSet.pass);

  // Close the Preferences
  preferences.end();
}

void serialReceive() 
{
  if (Serial.available() == 0) {
    return;
  }

  String command = Serial.readStringUntil('\n');
  
  if (command.equals("ping")) {
    Serial.println("pong");

    return;
  }

    if (command.equals("restart")) {
    Serial.println("Restart...");

    ESP.restart();

    return;
  }

  if (command.startsWith("set")) {
    if (command.startsWith("speed", 4)) {
      ventSet.speed = command.substring(9).toInt();

      write_preferences();

      Serial.printf("set speed: %d", ventSet.speed);
      Serial.println();

      return;
    }

    if (command.startsWith("wifi", 4)) {
      String wifiDsn = command.substring(8);
      wifiSet.ssid = wifiDsn.substring(1, wifiDsn.indexOf('@'));
      wifiSet.pass = wifiDsn.substring(wifiDsn.indexOf('@') + 1);
      
      write_preferences();
      
      Serial.print("set wifi");
      Serial.println();
      
      Serial.printf("\tssid: %s", wifiSet.ssid);
      Serial.println();
      
      Serial.printf("\tpass: %s", wifiSet.pass);
      Serial.println();

      Serial.printf("\tlocal IP: %s", WiFi.localIP());
      Serial.println();      
      
      return;
    }
  }

  if (command.startsWith("get")) {
    if (command.startsWith("speed", 4)) {
      Serial.printf("get speed: %d", ventSet.speed);
      Serial.println();

      return;
    }

    if (command.startsWith("wifi", 4)) {
      Serial.print("get wifi");
      Serial.println();

      Serial.printf("\tssid: %s", wifiSet.ssid);
      Serial.println();
    }
  }
}

void webserverHandle()
{
  server.handleClient();
}