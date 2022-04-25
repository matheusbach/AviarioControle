#include "headers.h"
#include "freertos/FreeRTOS.h"

void CreateRoutineTasks()
{
    xTaskCreatePinnedToCore(CheckNetworkConnection, "CheckNetworkConnection", 2048, NULL, 4, NULL, 1);
    xTaskCreatePinnedToCore(UpdateClima, "UpdateClima", 2048, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(UpdateRSSI, "UpdateRSSI", 1024, NULL, 1, NULL, 1);
  //  xTaskCreatePinnedToCore(DoVentilacaoMinima, "VentilacaoMinima", 1024, NULL, 2, NULL, 0);
    xTaskCreatePinnedToCore(VerificarMensagensTelegram, "verificarMensagensTelegram", 8192, NULL, 5, NULL, 1);
}