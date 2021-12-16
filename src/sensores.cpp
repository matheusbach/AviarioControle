#include "DHT.h"
#include "headers.h"

float temperatura = 0;
float umidade = 0;

float sensorTemperatureOffsetBias = 1.1;
float sensorHumidityOffsetBias = 0;

#define DHTPIN 23 // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void StartSensores()
{
  dht.begin();
}

void UpdateClima(void *parameter)
{
  while (true)
  {
  //  Serial.println("lendo sensores");
    float tempUmidade = dht.readHumidity();
    float tempTemperatura = dht.readTemperature();

    if (!isnan(tempTemperatura))
    {
      temperatura = tempTemperatura - sensorTemperatureOffsetBias;
    }

    if (!isnan(tempUmidade))
    {
      umidade = tempUmidade - sensorHumidityOffsetBias;
    }

    TransmitInfoToWebServerClients();

    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}