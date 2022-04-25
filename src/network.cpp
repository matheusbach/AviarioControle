#include "headers.h"

float RSSI = -100;
const char *ssid = "TendaRouter";
const char *password = "1234567890";

bool wifiIsConnected = false;

void StartNetWorkConnection()
{
  WiFi.mode(WIFI_MODE_STA);

  // WiFi.setHostname("Aviario Controle"); // define hostname

  WiFi.begin(ssid, password);

  wifi_country_t country = {
      .cc = "IN",
      .schan = 1,
      .nchan = 13,
      .max_tx_power = 20,
      .policy = WIFI_COUNTRY_POLICY_AUTO,
  };
  esp_wifi_set_country(&country);

  WiFi.setAutoConnect(true);

  // WiFi.setTxPower(WIFI_POWER_19_5dBm);
  // WiFi.setAutoReconnect(true);

  Serial.write("wifi tx power: ", WiFi.getTxPower());
}

void CheckNetworkConnection(void *parameter)
{
  while (true)
  {
    if (!WiFi.isConnected())
    {
      Serial.println("Wifi desconectado: tentando reconectar");
      wifiIsConnected = false;
      WiFi.reconnect();
    }
    else{
      if (!wifiIsConnected)
      {
        wifiIsConnected = true;
        Serial.println("Conectado ao WiFi");
      }
    }

    vTaskDelay(20000 / portTICK_PERIOD_MS);
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