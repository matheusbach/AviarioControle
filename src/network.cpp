#include "headers.h"

float RSSI = 100;
const char *ssid = "TendaRouter";
const char *password = "1234567890";

void StartNetWorkConnection()
{
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid, password);
  WiFi.setAutoConnect(true);
//  WiFi.setAutoReconnect(true);
}

void CheckNetworkConnection(void *parameter)
{
  while (true)
  {
    if (!WiFi.isConnected())
    {
    //  Serial.println("Wifi está desconectado");
      WiFi.reconnect();
    }
    else{
    //  Serial.println("Wifi conectado");
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