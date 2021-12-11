#include "Arduino.h"
#include "Wire.h"
#include "headers.h"
#include "time.h"
#include "SPIFFS.h"

// Replace with your network credentials

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -10800;
const int daylightOffset_sec = 0;

// Create AsyncWebServer object on port 80

// Replaces placeholder with DHT values

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(9600);

  SPIFFS.begin(true);

  configRead();

  digitalWrite(ventiladorPin, HIGH);
  pinMode(ventiladorPin, OUTPUT);

  // Connect to Wi-Fi

  StartSensores();

  StartNetWorkConnection();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Route for root / web page

  setupWebServer();

  setupTelegramBot();

  CreateRoutineTasks();
}

void loop()
{
}
