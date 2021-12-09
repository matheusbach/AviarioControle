
#include "FreeRTOS.h"
#include "headers.h"

void CreateRoutineTasks()
{
    xTaskCreatePinnedToCore(CheckNetworkConnection, "CheckNetworkConnection", 1000, NULL, 4, NULL, 1);
    xTaskCreatePinnedToCore(UpdateClima, "UpdateClima", 2000, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(UpdateRSSI, "UpdateRSSI", 1000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(DoVentilacaoMinima, "VentilacaoMinima", 1000, NULL, 2, NULL, 0);
    xTaskCreatePinnedToCore(VerificarMensagensTelegram, "verificarMensagensTelegram", 5000, NULL, 2, NULL, 0);
}