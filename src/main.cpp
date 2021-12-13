#include "headers.h"

const char* versionLabel = "1.0";

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -10800;
const int daylightOffset_sec = 0;

void setup()
{
  Serial.begin(9600);

  SPIFFS.begin(true);

  configRead();

  digitalWrite(ventiladorPin, HIGH);
  pinMode(ventiladorPin, OUTPUT);

  StartSensores();

  StartNetWorkConnection();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  setupWebServer();

  setupTelegramBot();

  CreateRoutineTasks();
}

void loop()
{
}