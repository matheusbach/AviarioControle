#include "headers.h"

float RSSI = 100;
const char *ssid = "TendaRouter";
const char *password = "1234567890";

void StartNetWorkConnection()
{
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid, password);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
}

void CheckNetworkConnection(void *parameter)
{
  while (true)
  {
    if (!WiFi.isConnected())
    {
      Serial.println("Wifi está desconectado");
     // WiFi.reconnect();
    }

    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

void UpdateRSSI(void *parameter)
{
  while (true)
  {
    RSSI = WiFi.RSSI();

    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

const char *toStr(wl_status_t status)
{
  switch (status)
  {
  case WL_NO_SHIELD:
    return "No shield";
  case WL_IDLE_STATUS:
    return "Idle status";
  case WL_NO_SSID_AVAIL:
    return "No SSID avail";
  case WL_SCAN_COMPLETED:
    return "Scan compleded";
  case WL_CONNECTED:
    return "Connected";
  case WL_CONNECT_FAILED:
    return "Failed";
  case WL_CONNECTION_LOST:
    return "Connection lost";
  case WL_DISCONNECTED:
    return "Disconnected";
  }
  return "Unknown";
}